#ifndef PATOMIC_WRAPPED_DIRECT_H
#define PATOMIC_WRAPPED_DIRECT_H

#include "base.h"

#include <patomic/macros/ignore_unused.h>
#include <patomic/macros/static_assert.h>

#include <patomic/api/memory_order.h>

#include <stddef.h>


/**
 * @addtogroup wrapped.direct
 *
 * @brief
 *   Defines a function which implements an atomic store operation using
 *   store as the underlying atomic operation.
 *
 * @details
 *   The defined function's signature will match either patomic_opsig_store_t
 *   or patomic_opsig_explicit_store_t (depending on the value of 'vis_p').
 *
 * @param atomic_type
 *   The type of the object on which the atomic operation is to be performed.
 *   Must not be a VLA or an array of unknown size.
 *
 * @param type
 *   The non-atomic counterpart of 'atomic_type'. This must have the same size
 *   as 'atomic_type' and must not have a stricter alignment.
 *
 * @param fn_name
 *   The name of the function to be defined.
 *
 * @param vis_p
 *   Either the macro 'SHOW_P' if the function should be defined as taking a
 *   memory order parameter (a.k.a. explicit), or the macro 'HIDE_P' if it
 *   should not (a.k.a. implicit).
 *
 * @param order
 *   The literal token 'order' if 'vis_p' is 'SHOW_P', otherwise the desired
 *   memory order to be used implicitly by the atomic operation.
 *
 * @param do_atomic_store_explicit
 *   A macro, M, callable as 'M(obj, des, order)' where:
 *   - the result of the expression is unused
 *   - 'obj' will be an expression of the type 'volatile atomic_type *'
 *   - 'des' will be the name of a local identifier, with the type 'type'
 *   - 'order' will be an expression of type 'int' whose value is a valid
 *     store memory order
 *
 *   The expected behaviour of calling the macro M as above is:
 *   - 'des' is atomically stored into the object pointed to by 'obj' with a
 *     store memory ordering at least as strong as 'order'
 *
 *   The following local variables will also be available to be used by
 *   the macro M:
 *   - 'temp' has type 'int'
 *   - 'scratch' has type 'type'
 *   - their value is unspecified and they may be uninitialized
 */
#define PATOMIC_WRAPPED_DIRECT_DEFINE_OP_STORE(                               \
    atomic_type, type, fn_name, vis_p, order,                                 \
    do_atomic_store_explicit                                                  \
)                                                                             \
    static void                                                               \
    fn_name(                                                                  \
        volatile void *obj                                                    \
        ,const void *desired                                                  \
 vis_p(_,int order)                                                           \
    )                                                                         \
    {                                                                         \
        /* static assertions */                                               \
        PATOMIC_STATIC_ASSERT(                                                \
            sizeof_type_eq_atype, sizeof(type) == sizeof(atomic_type));       \
                                                                              \
        /* declarations */                                                    \
        type des;                                                             \
        type scratch                                                          \
        int temp;                                                             \
                                                                              \
        /* assertions */                                                      \
        PATOMIC_WRAPPED_DO_ASSERT(obj != NULL);                               \
        PATOMIC_WRAPPED_DO_ASSERT(desired != NULL);                           \
        PATOMIC_WRAPPED_DO_ASSERT_ALIGNED(obj, atomic_type);                  \
        PATOMIC_WRAPPED_DO_ASSERT(PATOMIC_IS_VALID_STORE_ORDER((int) order)); \
                                                                              \
        /* setup */                                                           \
        PATOMIC_WRAPPED_DO_MEMCPY(&des, desired, sizeof(type));               \
                                                                              \
        /* operation */                                                       \
        do_atomic_store_explicit(                                             \
            (volatile atomic_type *) obj,                                     \
            des,                                                              \
            (int) order                                                       \
        );                                                                    \
                                                                              \
        /* cleanup */                                                         \
        PATOMIC_IGNORE_UNUSED(scratch);                                       \
        PATOMIC_IGNORE_UNUSED(temp);                                          \
    }


/**
 * @addtogroup wrapped.direct
 *
 * @brief
 *   Defines a function which implements an atomic load operation using
 *   load as the underlying atomic operation.
 *
 * @details
 *   The defined function's signature will match either patomic_opsig_load_t
 *   or patomic_opsig_explicit_load_t (depending on the value of 'vis_p').
 *
 * @param atomic_type
 *   The type of the object on which the atomic operation is to be performed.
 *   Must not be a VLA or an array of unknown size.
 *
 * @param type
 *   The non-atomic counterpart of 'atomic_type'. This must have the same size
 *   as 'atomic_type' and must not have a stricter alignment.
 *
 * @param fn_name
 *   The name of the function to be defined.
 *
 * @param vis_p
 *   Either the macro 'SHOW_P' if the function should be defined as taking a
 *   memory order parameter (a.k.a. explicit), or the macro 'HIDE_P' if it
 *   should not (a.k.a. implicit).
 *
 * @param order
 *   The literal token 'order' if 'vis_p' is 'SHOW_P', otherwise the desired
 *   memory order to be used implicitly by the atomic operation.
 *
 * @param do_atomic_load_explicit
 *   A macro, M, callable as 'M(obj, order, res)' where:
 *   - the result of the expression is unused
 *   - 'obj' will be an expression of the type 'const volatile atomic_type *'
 *   - 'order' will be an expression of type 'int' whose value is a valid
 *     load memory order
 *   - 'res' will be the name of a local identifier, with the type 'type'
 *
 *   The expected behaviour of calling the macro M as above is:
 *   - the value of the object pointed to by 'obj' is atomically read with a
 *     load memory ordering at least as strong as 'order'
 *   - 'res' is set to the value which was read
 *
 *   The following local variables will also be available to be used by
 *   the macro M:
 *   - 'temp' has type 'int'
 *   - 'scratch' has type 'type'
 *   - their value is unspecified and they may be uninitialized
 */
#define PATOMIC_WRAPPED_DIRECT_DEFINE_OP_LOAD(                               \
    atomic_type, type, fn_name, vis_p, order,                                \
    do_atomic_load_explicit                                                  \
)                                                                            \
    static void                                                              \
    fn_name(                                                                 \
        volatile void *obj                                                   \
 vis_p(_,int order)                                                          \
        ,void *ret                                                           \
    )                                                                        \
    {                                                                        \
        /* static assertions */                                              \
        PATOMIC_STATIC_ASSERT(                                               \
            sizeof_type_eq_atype, sizeof(type) == sizeof(atomic_type));      \
                                                                             \
        /* declarations */                                                   \
        type res;                                                            \
        type scratch;                                                        \
        int temp;                                                            \
                                                                             \
        /* assertions */                                                     \
        PATOMIC_WRAPPED_DO_ASSERT(obj != NULL);                              \
        PATOMIC_WRAPPED_DO_ASSERT(ret != NULL);                              \
        PATOMIC_WRAPPED_DO_ASSERT_ALIGNED(obj, atomic_type);                 \
        PATOMIC_WRAPPED_DO_ASSERT(PATOMIC_IS_VALID_LOAD_ORDER((int) order)); \
                                                                             \
        /* operation */                                                      \
        do_atomic_load_explicit(                                             \
            (const volatile atype *) obj,                                    \
            (int) order,                                                     \
            res                                                              \
        );                                                                   \
                                                                             \
        /* cleanup */                                                        \
        PATOMIC_WRAPPED_DO_MEMCPY(ret, &res, sizeof(type));                  \
        PATOMIC_IGNORE_UNUSED(scratch);                                      \
        PATOMIC_IGNORE_UNUSED(temp);                                         \
    }


#endif  /* PATOMIC_WRAPPED_DIRECT_H */
