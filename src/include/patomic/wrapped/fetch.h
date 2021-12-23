#ifndef PATOMIC_PATOMIC_WRAPPED_FETCH_H
#define PATOMIC_PATOMIC_WRAPPED_FETCH_H

#include <patomic/types/memory_order.h>

#include <patomic/macros/force_inline.h>
#include <patomic/macros/ignore_unused.h>

#include <patomic/wrapped/do.h>


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


#define PATOMIC_WRAPPED_FETCH_DEFINE_OP_WITH_ARG(      \
    do_fetch_with_arg,                                 \
    type, atype, fn_name, vis_p                        \
)                                                      \
    static PATOMIC_FORCE_INLINE void                   \
    fn_name(                                           \
        volatile void *obj                             \
        ,const void *arg                               \
 vis_p(_,int order)                                    \
    )                                                  \
    {                                                  \
        /* declarations */                             \
        type val;                                      \
        /* assertions */                               \
        PATOMIC_WRAPPED_DO_ASSERT(obj != NULL);        \
        PATOMIC_WRAPPED_DO_ASSERT(arg != NULL);        \
        PATOMIC_WRAPPED_DO_ASSERT_ALIGNED(obj, atype); \
        PATOMIC_IGNORE_UNUSED((                        \
            PATOMIC_IGNORE_UNUSED(0)                   \
            vis_p(_,PATOMIC_WRAPPED_DO_ASSERT(         \
                patomic_is_valid_order((int) order)))  \
        ));                                            \
        /* operation */                                \
        do_fetch_with_arg(                             \
            obj                                        \
            ,arg                                       \
     vis_p(_,order)                                    \
            ,&val                                      \
        );                                             \
        PATOMIC_IGNORE_UNUSED(val);                    \
    }



#define PATOMIC_WRAPPED_FETCH_DEFINE_OP_NOARG(         \
    do_fetch_noarg,                                    \
    type, atype, fn_name, vis_p                        \
)                                                      \
    static PATOMIC_FORCE_INLINE void                   \
    fn_name(                                           \
        volatile void *obj                             \
 vis_p(_,int order)                                    \
    )                                                  \
    {                                                  \
        /* declarations */                             \
        type val;                                      \
        /* assertions */                               \
        PATOMIC_WRAPPED_DO_ASSERT(obj != NULL);        \
        PATOMIC_WRAPPED_DO_ASSERT_ALIGNED(obj, atype); \
        PATOMIC_IGNORE_UNUSED((                        \
            PATOMIC_IGNORE_UNUSED(0)                   \
            vis_p(_,PATOMIC_WRAPPED_DO_ASSERT(         \
                patomic_is_valid_order((int) order)))  \
        ));                                            \
        /* operation */                                \
        do_fetch_noarg(                                \
            obj                                        \
     vis_p(_,order)                                    \
            ,&val                                      \
        );                                             \
        PATOMIC_IGNORE_UNUSED(val);                    \
    }

#endif  /* !PATOMIC_PATOMIC_WRAPPED_FETCH_H */
