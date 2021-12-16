#ifndef PATOMIC_PATOMIC_WRAPPED_FETCH_H
#define PATOMIC_PATOMIC_WRAPPED_FETCH_H

#include <patomic/types/memory_order.h>

#include <patomic/macros/force_inline.h>
#include <patomic/macros/ignore_unused.h>


#undef HIDE_P
#undef SHOW_P

#define HIDE_P(x, y)
#define SHOW_P(x, y) ,y

#undef HIDE
#undef SHOW

#define HIDE(x)
#define SHOW(x) x


/*
 * MACRO PARAMETERS
 *
 * - do_fetch_with_arg:
 *   - if `vis_p` is `SHOW_P`, must act as `patomic_opsig_explicit_void_t`
 *   - otherwise, if `vis_p` is `HIDE_P`, must act as `patomic_opsig_void_t`
 *
 * - do_fetch_noarg:
 *   - if `vis_p` is `SHOW_P`, must act as `patomic_opsig_explicit_void_noarg_t`
 *   - otherwise, if `vis_p` is `HIDE_P`, must act as `patomic_opsig_void_noarg_t`
 *
 * - do_assert:
 *   - must be callable as `do_assert(expr);`
 *   - the result of the expression will not be assigned to anything
 *   - `expr` will evaluate to an expression of type (int)
 *   - standard assertion
 *
 * - do_assert_aligned:
 *   - must be callable as `do_assert_aligned(cv_vptr, TYPE);`
 *   - the result of the expressions will not be assigned to anything
 *   - `cv_vptr` will be of type (void*) and may be const/void qualified
 *   - `TYPE` will represent a valid type
 *   - asserts that `cv_vptr` meets the alignment required by `TYPE`
 *
 * - do_memcpy:
 *   - must be callable as `do_memcpy(vptr, c_vptr, len);`
 *   - the result of the expression will not be assigned to anything
 *   - `vptr` will be of type (void*)
 *   - `c_vptr` will be of type (void*) and may be const-qualified
 *   - `len` will be of type (size_t)
 *
 * - type:
 *   - the non-qualified version of the type the atomic operation will be
 *     performed on (e.g. `int`)
 *   - must not be a VLA or array of unknown size
 *
 * - atype:
 *   - the type the atomic operation will be performed on (e.g. `_Atomic(int)`)
 *   - sizeof(atype) must equal sizeof(type)
 *   - must not be const-qualified
 *   - may, but need not be, volatile-qualified (would be redundant)
 *   - must not be a VLA or array of unknown size
 *
 * - fn_name:
 *   - the full name for the function to be defined
 *   - aforementioned function will always have `static` linkage
 *
 * - vis_p:
 *   - must be one of HIDE_P or SHOW_P
 *   - used to hide or show memory order parameters
 *   - use SHOW_P for the explicit version of a function, and HIDE_P otherwise
 */


#define PATOMIC_WRAPPED_FETCH_DEFINE_OP_WITH_ARG(                   \
    do_fetch_with_arg,                                              \
    do_assert, do_assert_aligned, do_memcpy,                        \
    type, atype, fn_name, vis_p                                     \
)                                                                   \
    static PATOMIC_FORCE_INLINE void                                \
    fn_name(                                                        \
        volatile void *obj                                          \
        ,const void *arg                                            \
 vis_p(_,int order)                                                 \
    )                                                               \
    {                                                               \
        /* declarations */                                          \
        type val;                                                   \
        /* assertions */                                            \
        do_assert(obj != NULL);                                     \
        do_assert(arg != NULL);                                     \
        do_assert_aligned(obj, atype);                              \
        PATOMIC_IGNORE_UNUSED((                                     \
            PATOMIC_IGNORE_UNUSED(0)                                \
            vis_p(_,do_assert(patomic_is_valid_order((int) order))) \
        ));                                                         \
        /* operation */                                             \
        do_fetch_with_arg(                                          \
            obj                                                     \
            ,arg                                                    \
     vis_p(_,order)                                                 \
            ,&val                                                   \
        );                                                          \
        PATOMIC_IGNORE_UNUSED(val);                                 \
    }



#define PATOMIC_WRAPPED_FETCH_DEFINE_OP_NOARG(                      \
    do_fetch_noarg,                                                 \
    do_assert, do_assert_aligned, do_memcpy,                        \
    type, atype, fn_name, vis_p                                     \
)                                                                   \
    static PATOMIC_FORCE_INLINE void                                \
    fn_name(                                                        \
        volatile void *obj                                          \
 vis_p(_,int order)                                                 \
    )                                                               \
    {                                                               \
        /* declarations */                                          \
        type val;                                                   \
        /* assertions */                                            \
        do_assert(obj != NULL);                                     \
        do_assert_aligned(obj, atype);                              \
        PATOMIC_IGNORE_UNUSED((                                     \
            PATOMIC_IGNORE_UNUSED(0)                                \
            vis_p(_,do_assert(patomic_is_valid_order((int) order))) \
        ));                                                         \
        /* operation */                                             \
        do_fetch_noarg(                                             \
            obj                                                     \
     vis_p(_,order)                                                 \
            ,&val                                                   \
        );                                                          \
        PATOMIC_IGNORE_UNUSED(val);                                 \
    }

#endif  /* !PATOMIC_PATOMIC_WRAPPED_FETCH_H */
