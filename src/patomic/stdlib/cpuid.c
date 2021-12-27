#include <patomic/patomic_config.h>

#include <patomic/macros/ignore_unused.h>

#include <patomic/stdlib/cpuid.h>
#include <patomic/stdlib/string.h>


#if PATOMIC_HAVE_INTRIN_EFLAGS_CPUID
    #include <intrin.h>
#else
    #include <cpuid.h>
#endif


#define PATOMIC_CPUID_MAX_LEAF 0x80000000


#if PATOMIC_HAVE_INTRIN_EFLAGS_CPUID
    #define PATOMIC_EFLAGS_ID_BIT  0x200000
    static int
    patomic_ms_cpuid_is_supported(void)
    {
        int was_set, is_set;
        unsigned long id_bit = PATOMIC_EFLAGS_ID_BIT;
        was_set = (__readeflags() & id_bit) != 0;
        __writeeflags(__readeflags() ^ id_bit);
        is_set = (__readeflags() & id_bit) != 0;
        __writeeflags(__readeflags() ^ id_bit);
        return (was_set != is_set);
    }
#endif


int
__patomic_cpuid_sig(
    char *sig_out
)
{
    /* fill registers */
    unsigned int ex[4];
    if (!__patomic_cpuid(0, &ex[0], &ex[1], &ex[2], &ex[3])) { return 0; }

    /* swap edx and ecx */
    ex[0] = ex[3];  /* edx -> eax */
    ex[3] = ex[2];  /* ecx -> edx */
    ex[2] = ex[0];  /* eax -> ecx */

    /* copy out */
    PATOMIC_IGNORE_UNUSED(patomic_memcpy(sig_out, &ex[1], 12));
    return 1;
}


unsigned int
__patomic_cpuid_max(
    unsigned int leaf
)
{
    leaf &= PATOMIC_CPUID_MAX_LEAF;

#if PATOMIC_HAVE_INTRIN_EFLAGS_CPUID
    if (!patomic_ms_cpuid_is_supported()) { return 0; }
    else
    {
        int ex[4];
        __cpuid((int) leaf, ex);
        return (unsigned int) ex[0];
    }
#elif PATOMIC_HAVE_CPUID_CPUID
    return (unsigned int) __get_cpuid_max(leaf, 0);
#else
    return 0u;
#endif
}


int
__patomic_cpuid(
    unsigned int leaf,
    unsigned int *eax, unsigned int *ebx,
    unsigned int *ecx, unsigned int *edx
)
{
    unsigned int max_leaf = __patomic_cpuid_max(leaf & PATOMIC_CPUID_MAX_LEAF);
    if (max_leaf == 0 || max_leaf < leaf) { return 0; }

#if PATOMIC_HAVE_INTRIN_EFLAGS_CPUID
    if (!patomic_ms_cpuid_is_supported()) { return 0; }
    else
    {
        int ex[] = {
            (int) *eax, (int) *ebx,
            (int) *ecx, (int) *edx
        };
        __cpuid((int) leaf, ex);
        *eax = ex[0]; *ebx = ex[1];
        *ecx = ex[2]; *edx = ex[3];
        return 1;
    }
#elif PATOMIC_HAVE_CPUID_CPUID
    __cpuid(leaf, *eax, *ebx, *ecx, *edx);
    return 1;
#else
    return 0;
#endif
}


int
__patomic_cpuidex(
    unsigned int leaf, unsigned subleaf,
    unsigned int *eax, unsigned int *ebx,
    unsigned int *ecx, unsigned int *edx
)
{
    unsigned int max_leaf = __patomic_cpuid_max(leaf & PATOMIC_CPUID_MAX_LEAF);
    if (max_leaf == 0 || max_leaf < leaf) { return 0; }

#if PATOMIC_HAVE_INTRIN_EFLAGS_CPUID
    if (!patomic_ms_cpuid_is_supported()) { return 0; }
    else
    {
        int ex[] = {
            (int) *eax, (int) *ebx,
            (int) *ecx, (int) *edx
        };
        __cpuidex((int) leaf, (int) subleaf, ex);
        *eax = ex[0]; *ebx = ex[1];
        *ecx = ex[2]; *edx = ex[3];
        return 1;
    }
#elif PATOMIC_HAVE_CPUID_CPUID
    __cpuid_count(leaf, subleaf, *eax, *ebx, *ecx, *edx);
    return 1;
#else
    return 0;
#endif
}
