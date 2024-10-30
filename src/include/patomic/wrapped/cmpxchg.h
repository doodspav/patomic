#ifndef PATOMIC_WRAPPED_CMPXCHG_H
#define PATOMIC_WRAPPED_CMPXCHG_H

#include "base.h"

#include <patomic/macros/static_assert.h>

#include <patomic/api/memory_order.h>

#include <limits.h>
#include <stddef.h>


/**
 * @addtogroup wrapped.cmpxchg
 *
 * @brief
 *   Defines a function which implements an atomic store operation using
 *   cmpxchg_weak as the underlying atomic operation.
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
 * @param do_atomic_cmpxchg_weak_explicit
 *   A macro, M, callable as 'M(type, obj, exp, des, succ, fail, ok);' in block
 *   scope.
 *
 *   The arguments provided are:
 *   - 'type' : forwarded directly from the 'type' argument in this macro
 *   - 'obj'  : an expression of type 'volatile atomic_type *'
 *   - 'exp'  : the name of an identifier designating an object of type 'type'
 *   - 'des'  : the name of an identifier designating an object of type 'type'
 *   - 'succ' : an expression of type 'int' whose value is a valid memory order
 *   - 'fail' : an expression of type 'int' whose value is a valid load memory
 *              order not stronger than 'succ'
 *   - 'ok'   : the name of an identifier designating an object of type 'int'
 *
 *   The expected behaviour of calling the macro M as above is:
 *   - the values of 'exp' and 'des' are read
 *   - in a single atomic operation, the value of the object pointed to by
 *     'obj' is read and, if it compares equal to the value of 'exp', the
 *     value of 'des' is written to the object pointed to by 'obj'
 *   - the value read from 'obj' is stored in 'exp'
 *   - 'ok' is set to non-zero if the value of 'des' was written to the object
 *     pointed to by 'obj' (success), otherwise it is set to zero (failure)
 *   - the atomic operation uses a memory ordering at least as strong as 'succ'
 *     for a successful exchange, and a load memory ordering at least as strong
 *     as 'fail' for a failed exchange
 */
#define PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_STORE(                              \
    atomic_type, type, fn_name, vis_p, order,                                 \
    do_atomic_cmpxchg_weak_explicit                                           \
)                                                                             \
    static void                                                               \
    fn_name(                                                                  \
        volatile void *const obj                                              \
        ,const void *const desired                                            \
 vis_p(_,const int order)                                                     \
    )                                                                         \
    {                                                                         \
        /* static assertions */                                               \
        PATOMIC_STATIC_ASSERT(                                                \
            sizeof_type_eq_atype, sizeof(type) == sizeof(atomic_type));       \
                                                                              \
        /* declarations */                                                    \
        type exp = {0};                                                       \
        type des;                                                             \
        int succ;                                                             \
        int fail;                                                             \
        int ok = 0;                                                           \
                                                                              \
        /* assertions */                                                      \
        PATOMIC_WRAPPED_DO_ASSERT(obj != NULL);                               \
        PATOMIC_WRAPPED_DO_ASSERT(desired != NULL);                           \
        PATOMIC_WRAPPED_DO_ASSERT_ALIGNED(obj, atomic_type);                  \
        PATOMIC_WRAPPED_DO_ASSERT(PATOMIC_IS_VALID_STORE_ORDER((int) order)); \
                                                                              \
        /* inputs */                                                          \
        succ = (int) order;                                                   \
        fail = PATOMIC_CMPXCHG_FAIL_ORDER(succ);                              \
        PATOMIC_WRAPPED_DO_MEMCPY(&des, desired, sizeof(type));               \
                                                                              \
        /* operation */                                                       \
        do {                                                                  \
            do_atomic_cmpxchg_weak_explicit(                                  \
                type,                                                         \
                (volatile atomic_type *) obj,                                 \
                exp, des,                                                     \
                succ, fail,                                                   \
                ok                                                            \
            );                                                                \
        while (!ok);                                                          \
    }


/**
 * @addtogroup wrapped.cmpxchg
 *
 * @brief
 *   Defines a function which implements an atomic exchange operation using
 *   cmpxchg_weak as the underlying atomic operation.
 *
 * @details
 *   The defined function's signature will match either patomic_opsig_exchange_t
 *   or patomic_opsig_explicit_exchange_t (depending on the value of 'vis_p').
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
 * @param do_atomic_cmpxchg_weak_explicit
 *   A macro, M, callable as 'M(type, obj, exp, des, succ, fail, ok);' in block
 *   scope.
 *
 *   The arguments provided are:
 *   - 'type' : forwarded directly from the 'type' argument in this macro
 *   - 'obj'  : an expression of type 'volatile atomic_type *'
 *   - 'exp'  : the name of an identifier designating an object of type 'type'
 *   - 'des'  : the name of an identifier designating an object of type 'type'
 *   - 'succ' : an expression of type 'int' whose value is a valid memory order
 *   - 'fail' : an expression of type 'int' whose value is a valid load memory
 *              order not stronger than 'succ'
 *   - 'ok'   : the name of an identifier designating an object of type 'int'
 *
 *   The expected behaviour of calling the macro M as above is:
 *   - the values of 'exp' and 'des' are read
 *   - in a single atomic operation, the value of the object pointed to by
 *     'obj' is read and, if it compares equal to the value of 'exp', the
 *     value of 'des' is written to the object pointed to by 'obj'
 *   - the value read from 'obj' is stored in 'exp'
 *   - 'ok' is set to non-zero if the value of 'des' was written to the object
 *     pointed to by 'obj' (success), otherwise it is set to zero (failure)
 *   - the atomic operation uses a memory ordering at least as strong as 'succ'
 *     for a successful exchange, and a load memory ordering at least as strong
 *     as 'fail' for a failed exchange
 */
#define PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_EXCHANGE(                     \
    atomic_type, type, fn_name, vis_p, order,                           \
    do_atomic_cmpxchg_weak_explicit                                     \
)                                                                       \
    static void                                                         \
    fn_name(                                                            \
        volatile void *const obj                                        \
        ,const void *const desired                                      \
 vis_p(_,const int order)                                               \
        ,void *const ret                                                \
    )                                                                   \
    {                                                                   \
        /* static assertions */                                         \
        PATOMIC_STATIC_ASSERT(                                          \
            sizeof_type_eq_atype, sizeof(type) == sizeof(atomic_type)); \
                                                                        \
        /* declarations */                                              \
        type exp = {0};                                                 \
        type des;                                                       \
        int succ;                                                       \
        int fail;                                                       \
        int ok = 0;                                                     \
                                                                        \
        /* assertions */                                                \
        PATOMIC_WRAPPED_DO_ASSERT(obj != NULL);                         \
        PATOMIC_WRAPPED_DO_ASSERT(desired != NULL);                     \
        PATOMIC_WRAPPED_DO_ASSERT(ret != NULL);                         \
        PATOMIC_WRAPPED_DO_ASSERT_ALIGNED(obj, atomic_type);            \
        PATOMIC_WRAPPED_DO_ASSERT(PATOMIC_IS_VALID_ORDER((int) order)); \
                                                                        \
        /* inputs */                                                    \
        succ = (int) order;                                             \
        fail = PATOMIC_CMPXCHG_FAIL_ORDER(succ);                        \
        PATOMIC_WRAPPED_DO_MEMCPY(&des, desired, sizeof(type));         \
                                                                        \
        /* operation */                                                 \
        do {                                                            \
            do_atomic_cmpxchg_weak_explicit(                            \
                type,                                                   \
                (volatile atomic_type *) obj,                           \
                exp, des,                                               \
                succ, fail,                                             \
                ok                                                      \
            );                                                          \
        }                                                               \
        while (!ok);                                                    \
                                                                        \
        /* outputs */                                                   \
        PATOMIC_WRAPPED_DO_MEMCPY(ret, &exp, sizeof(type));             \
    }


#endif  /* PATOMIC_WRAPPED_CMPXCHG_H */
