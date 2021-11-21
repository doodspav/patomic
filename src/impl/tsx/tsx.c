#include "tsx.h"

#include <patomic/patomic.h>
#include <patomic/patomic_config.h>
#include <patomic/macros/ignore_unused.h>

static const patomic_t patomic_NULL;
static const patomic_explicit_t patomic_explicit_NULL;


#if PATOMIC_HAVE_IMMINTRIN_RTM && \
    (PATOMIC_HAVE_INTRIN_EFLAGS_CPUID || PATOMIC_HAVE_CPUID_CPUID)

#include <immintrin.h>
#include <stddef.h>
#include <string.h>

#include "repeat.h"

#if PATOMIC_HAVE_INTRIN_EFLAGS_CPUID
#include <intrin.h>

static int
patomic_tsx_supports_rtm(void)
{
    int ex[4];
    int a=0, b=1;
    long id_bit = 0x200000;
    /* need to check if cpuid is available */
    /* but can't write __readeflags' return type */
    /* depends on 32bit/64bit and would need extra checking */
    /* eok = ((r() | m) && ((w(r() ^ m), r()) | m)) ? 0 : 1; */
    int eok = ((__readeflags() | id_bit) && \
        ((__writeeflags(__readeflags() ^ id_bit), __readeflags()) | id_bit)) ? \
        0 : 1;
    /* set eflags back to original value (and check eok) */
    __writeeflags(__readeflags() ^ id_bit);
    if (!eok) { return 0; }
    /* check for rtm using cpuid */
    __cpuid(ex, 0);
    if (ex[a] < 0x7) { return 0; }
    __cpuidex(ex, 0x7, 0x0);
    return (((unsigned)ex[b]) & 0x800u) != 0;
}

#elif PATOMIC_HAVE_CPUID_CPUID
#include <cpuid.h>

static int
patomic_tsx_supports_rtm(void)
{
    unsigned int eax, ebx, ecx, edx;
    if (__get_cpuid_max(0x0, &eax) == 0) { return 0; }
    __cpuid(0, eax, ebx, ecx, edx);
    if (eax < 0x7) { return 0; }
    __cpuid_count(0x7, 0x0, eax, ebx, ecx, edx);
    return (ebx & 0x800u) != 0;
}

#else
    #error "How'd you get here"
#endif

static int
patomic_tsx_cmpxchg_weak_impl(
    volatile void *vobj,
    void *expected,
    const void *desired,
    size_t n
)
{
    unsigned int status;
    int res;
    void *obj = (void *) vobj;

    /* attempt single cmpxchg transaction */
    if (_xbegin() == _XBEGIN_STARTED)
    {
        res = memcmp(obj, expected, n);
        if (res != 0) { _xabort(0); }
        PATOMIC_IGNORE_UNUSED(memcpy(obj, desired, n));
        _xend();
        return 1;
    }
    /* infinite loop attempting to load */
    else
    {
        do {
            if ((status = _xbegin()) == _XBEGIN_STARTED)
            {
                PATOMIC_IGNORE_UNUSED(memcpy(expected, obj, n));
                _xend();
            }
        }
        while (status != _XBEGIN_STARTED);
        return 0;
    }
}

#define PATOMIC_TSX_DEFINE_WRAPPERS(n)                                 \
    static int                                                         \
    patomic_tsx_cmpxchg_weak_##n(                                      \
        volatile void *obj,                                            \
        void *expected,                                                \
        const void *desired                                            \
    )                                                                  \
    {                                                                  \
        int res;                                                       \
        unsigned char exp[n];                                          \
        unsigned char des[n];                                          \
                                                                       \
        /* store expected and desired locally */                       \
        /* makes sure no one can access them during transaction */     \
        /* also makes sure the memory is paged in */                   \
        /* prevents potential aborts */                                \
        PATOMIC_IGNORE_UNUSED(memcpy(exp, expected, n));               \
        PATOMIC_IGNORE_UNUSED(memcpy(des, desired, n));                \
                                                                       \
        /* atomic obj memory should already be paged in */             \
        /* so there should be no aborts due to page faults */          \
        res = patomic_tsx_cmpxchg_weak_impl(                           \
            obj,                                                       \
            expected,                                                  \
            desired,                                                   \
            n                                                          \
        );                                                             \
                                                                       \
        /* copy exp back on failure */                                 \
        if (!res) { PATOMIC_IGNORE_UNUSED(memcpy(expected, exp, n)); } \
        return res;                                                    \
    }                                                                  \
    static int                                                         \
    patomic_tsx_explicit_cmpxchg_weak_##n(                             \
        volatile void *obj,                                            \
        void *expected,                                                \
        const void *desired,                                           \
        int succ,                                                      \
        int fail                                                       \
    )                                                                  \
    {                                                                  \
        PATOMIC_IGNORE_UNUSED(succ);                                   \
        PATOMIC_IGNORE_UNUSED(fail);                                   \
        return patomic_tsx_cmpxchg_weak_##n(                           \
            obj,                                                       \
            expected,                                                  \
            desired                                                    \
        );                                                             \
    }
PATOMIC_TSX_REPEAT(PATOMIC_TSX_DEFINE_WRAPPERS)

#define PATOMIC_TSX_ELEM(n) patomic_tsx_cmpxchg_weak_##n,
#define PATOMIC_TSX_EXP_ELEM(n) patomic_tsx_explicit_cmpxchg_weak_##n,

static patomic_opsig_cmpxchg_t
patomic_tsx_fps_cmpxchg_weak[PATOMIC_TSX_REPEAT_N_1] = {
    PATOMIC_TSX_REPEAT(PATOMIC_TSX_ELEM)
    NULL  /* C90 doesn't allow trailing commas */
};

static patomic_opsig_explicit_cmpxchg_t
patomic_tsx_fps_explicit_cmpxchg_weak[PATOMIC_TSX_REPEAT_N_1] = {
    PATOMIC_TSX_REPEAT(PATOMIC_TSX_EXP_ELEM)
    NULL  /* C90 doesn't allow trailing commas */
};

static patomic_opsig_cmpxchg_t
patomic_tsx_get_cmpxchg_fp(
    size_t width
)
{
    if (width == 0 || width > PATOMIC_TSX_REPEAT_N) { return NULL; }
    else { return patomic_tsx_fps_cmpxchg_weak[width - 1]; }
}

static patomic_opsig_explicit_cmpxchg_t
patomic_tsx_get_explicit_cmpxchg_fp(
    size_t width
)
{
    if (width == 0 || width > PATOMIC_TSX_REPEAT_N) { return NULL; }
    else { return patomic_tsx_fps_explicit_cmpxchg_weak[width - 1]; }
}

#else  /* no rtm or cpuid support */

static patomic_opsig_cmpxchg_t
patomic_tsx_get_cmpxchg_fp(
    size_t width
)
{
    PATOMIC_IGNORE_UNUSED(width);
    return NULL;
}

static patomic_opsig_explicit_cmpxchg_t
patomic_tsx_get_explicit_cmpxchg_fp(
    size_t width
)
{
    PATOMIC_IGNORE_UNUSED(width);
    return NULL;
}

#endif


patomic_t
patomic_impl_create_tsx(
    size_t byte_width,
    patomic_memory_order_t order,
    int options
)
{
    patomic_t ret;
    PATOMIC_IGNORE_UNUSED(order);
    PATOMIC_IGNORE_UNUSED(options);
    ret = patomic_NULL;
    ret.ops.xchg_ops.fp_cmpxchg_weak = \
        patomic_tsx_get_cmpxchg_fp(byte_width);
    ret.align.recommended = 1;
    ret.align.minimum = 1;
    return ret;
}

patomic_explicit_t
patomic_impl_create_explicit_tsx(
    size_t byte_width,
    int options
)
{
    patomic_explicit_t ret;
    PATOMIC_IGNORE_UNUSED(byte_width);
    PATOMIC_IGNORE_UNUSED(options);
    ret = patomic_explicit_NULL;
    ret.ops.xchg_ops.fp_cmpxchg_weak = \
        patomic_tsx_get_explicit_cmpxchg_fp(byte_width);
    ret.align.recommended = 1;
    ret.align.minimum = 1;
    return ret;
}
