/* Copyright (c) doodspav. */
/* SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception */

#ifndef PATOMIC_WRAPPED_CMPXCHG_H
#define PATOMIC_WRAPPED_CMPXCHG_H

#include "base.h"


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
        int ok;                                                               \
        const int succ = (int) order;                                         \
        const int fail = PATOMIC_CMPXCHG_FAIL_ORDER(succ);                    \
                                                                              \
        /* assertions */                                                      \
        PATOMIC_WRAPPED_DO_ASSERT(obj != NULL);                               \
        PATOMIC_WRAPPED_DO_ASSERT(desired != NULL);                           \
        PATOMIC_WRAPPED_DO_ASSERT_ALIGNED(obj, atomic_type);                  \
        PATOMIC_WRAPPED_DO_ASSERT(PATOMIC_IS_VALID_STORE_ORDER((int) order)); \
                                                                              \
        /* inputs */                                                          \
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
        }                                                                     \
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
        int ok;                                                         \
        const int succ = (int) order;                                   \
        const int fail = PATOMIC_CMPXCHG_FAIL_ORDER(succ);              \
                                                                        \
        /* assertions */                                                \
        PATOMIC_WRAPPED_DO_ASSERT(obj != NULL);                         \
        PATOMIC_WRAPPED_DO_ASSERT(desired != NULL);                     \
        PATOMIC_WRAPPED_DO_ASSERT(ret != NULL);                         \
        PATOMIC_WRAPPED_DO_ASSERT_ALIGNED(obj, atomic_type);            \
        PATOMIC_WRAPPED_DO_ASSERT(PATOMIC_IS_VALID_ORDER((int) order)); \
                                                                        \
        /* inputs */                                                    \
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


/**
 * @addtogroup wrapped.cmpxchg
 *
 * @brief
 *   Defines a function which implements an atomic cmpxchg operation which
 *   cannot spuriously fail using cmpxchg_weak as the underlying atomic
 *   operation.
 *
 * @details
 *   The defined function's signature will match either patomic_opsig_cmpxchg_t
 *   or patomic_opsig_explicit_cmpxchg_t (depending on the value of 'vis_p').
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
 * @param inv
 *   Either the macro 'HIDE' if 'vis_p' is 'SHOW_P', or the macro 'SHOW' if
 *   'vis_p' is 'HIDE_P'.
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
 *
 * @param do_cmp_eq
 *   A macro, C, callable as 'C(type, a, b, cmp);' in block scope.
 *
 *   The arguments provided are:
 *   - 'type' : forwarded directly from the 'type' argument in this macro
 *   - 'a'    : the name of an identifier designating an object of type 'type'
 *   - 'b'    : the name of an identifier designating an object of type 'type'
 *   - 'cmp'  : the name of an identifier designating an object of type 'int'
 *
 *   The expected behaviour of calling the macro C as above is:
 *   - 'cmp' is set to a non-zero value if 'a' and 'b' compare equal, otherwise
 *     it is set to zero
 */
#define PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_CMPXCHG_STRONG(                   \
    atomic_type, type, fn_name, vis_p, inv, order,                          \
    do_atomic_cmpxchg_weak_explicit, do_cmp_eq                              \
)                                                                           \
    static int                                                              \
    fn_name(                                                                \
        volatile void *const obj                                            \
        ,void *const expected                                               \
        ,const void *const desired                                          \
 vis_p(_,const int succ)                                                    \
 vis_p(_,const int fail)                                                    \
    )                                                                       \
    {                                                                       \
        /* static assertions */                                             \
        PATOMIC_STATIC_ASSERT(                                              \
            sizeof_type_eq_atype, sizeof(type) == sizeof(atomic_type));     \
                                                                            \
        /* declarations */                                                  \
        type exp;                                                           \
        type des;                                                           \
        type old;                                                           \
        int eq;                                                             \
        int ok;                                                             \
    inv(const int succ = (int) order;)                                      \
    inv(const int fail = PATOMIC_CMPXCHG_FAIL_ORDER((int) order);)          \
                                                                            \
        /* assertions */                                                    \
        PATOMIC_WRAPPED_DO_ASSERT(obj != NULL);                             \
        PATOMIC_WRAPPED_DO_ASSERT(expected != NULL);                        \
        PATOMIC_WRAPPED_DO_ASSERT(desired != NULL);                         \
        PATOMIC_WRAPPED_DO_ASSERT_ALIGNED(obj, atomic_type);                \
        PATOMIC_WRAPPED_DO_ASSERT(PATOMIC_IS_VALID_ORDER(succ));            \
        PATOMIC_WRAPPED_DO_ASSERT(PATOMIC_IS_VALID_FAIL_ORDER(succ, fail)); \
                                                                            \
        /* inputs */                                                        \
        PATOMIC_WRAPPED_DO_MEMCPY(&exp, expected, sizeof(type));            \
        PATOMIC_WRAPPED_DO_MEMCPY(&des, desired, sizeof(type));             \
                                                                            \
        /* operation */                                                     \
        do {                                                                \
            /* save expected value for comparison */                        \
            PATOMIC_WRAPPED_DO_MEMCPY(&old, &exp, sizeof(type));            \
                                                                            \
            /* cmpxchg_weak */                                              \
            do_atomic_cmpxchg_weak_explicit(                                \
                type,                                                       \
                (volatile atomic_type *) obj,                               \
                exp, des,                                                   \
                succ, fail,                                                 \
                ok                                                          \
            );                                                              \
                                                                            \
            /* check for modification */                                    \
            do_cmp_eq(                                                      \
                type,                                                       \
                exp, old,                                                   \
                eq                                                          \
            );                                                              \
                                                                            \
            /* was modified: non-spurious failure */                        \
            if (!eq)                                                        \
            {                                                               \
                break;                                                      \
            }                                                               \
        }                                                                   \
        while (!ok);                                                        \
                                                                            \
        /* outputs */                                                       \
        PATOMIC_WRAPPED_DO_MEMCPY(expected, &exp, sizeof(type));            \
        return ok != 0;                                                     \
    }


/**
 * @addtogroup wrapped.cmpxchg
 *
 * @brief
 *   Defines a function which implements an atomic bit test-modify operation
 *   using cmpxchg_weak as the underlying atomic operation.
 *
 * @details
 *   The defined function's signature will match either
 *   patomic_opsig_test_modify_t or patomic_opsig_explicit_test_modify_t
 *   (depending on the value of 'vis_p').
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
 *
 * @param do_get_bit
 *   A macro, B, callable as 'B(type, exp, offset, bit);' in block scope.
 *
 *   The arguments provided are:
 *   - 'type'   : forwarded directly from the 'type' argument in this macro
 *   - 'exp'    : the name of an identifier designating an object of type 'type'
 *   - 'offset' : an expression of type 'int' whose value is non-negative and
 *                less than 'sizeof(type) * CHAR_BIT'
 *   - 'bit'    : the name of an identifier designating an object of type 'int'
 *
 *   The expected behaviour of calling the macro B as above is:
 *   - 'bit' is set to the value of the bit at offset 'offset' of 'exp'
 *   - 'exp' should not be modified
 *
 * @param do_make_desired
 *   A macro, D, callable as 'D(type, exp, offset, des);' in block scope.
 *
 *   The arguments provided are:
 *   - 'type'   : forwarded directly from the 'type' argument in this macro
 *   - 'exp'    : the name of an identifier designating an object of type 'type'
 *   - 'offset' : an expression of type 'int' whose value is non-negative and
 *                less than 'sizeof(type) * CHAR_BIT'
 *   - 'des'    : the name of an identifier designating an object of type 'type'
 *
 *   The expected behaviour of calling the macro D as above is:
 *   - 'des' should be set to the value of 'exp' after the desired modify
 *     operation is applied
 *   - 'exp' should not be modified
 */
#define PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_BIT_TEST_MODIFY(                      \
    atomic_type, type, fn_name, vis_p, order,                                   \
    do_atomic_cmpxchg_weak_explicit,                                            \
    do_get_bit, do_make_desired                                                 \
)                                                                               \
    static int                                                                  \
    fn_name(                                                                    \
        volatile void *const obj                                                \
        ,const int offset                                                       \
 vis_p(_,const int order)                                                       \
    )                                                                           \
    {                                                                           \
        /* static assertions */                                                 \
        PATOMIC_STATIC_ASSERT(                                                  \
            sizeof_type_eq_atype, sizeof(type) == sizeof(atomic_type));         \
                                                                                \
        /* declarations */                                                      \
        type exp = {0};                                                         \
        type des;                                                               \
        int exp_bit;                                                            \
        int ok;                                                                 \
        const int succ = (int) order;                                           \
        const int fail = PATOMIC_CMPXCHG_FAIL_ORDER(succ);                      \
                                                                                \
        /* assertions */                                                        \
        PATOMIC_WRAPPED_DO_ASSERT(obj != NULL);                                 \
        PATOMIC_WRAPPED_DO_ASSERT_ALIGNED(obj, atomic_type);                    \
        PATOMIC_WRAPPED_DO_ASSERT(offset >= 0);                                 \
        PATOMIC_WRAPPED_DO_ASSERT((size_t) offset < (sizeof(type) * CHAR_BIT)); \
        PATOMIC_WRAPPED_DO_ASSERT(PATOMIC_IS_VALID_ORDER((int) order));         \
                                                                                \
        /* operation */                                                         \
        do {                                                                    \
            /* get the expected bit */                                          \
            do_get_bit(                                                         \
                type,                                                           \
                exp, offset,                                                    \
                exp_bit                                                         \
            );                                                                  \
                                                                                \
            /* make the desired value from the expected value */                \
            do_make_desired(                                                    \
                type,                                                           \
                exp, offset,                                                    \
                des                                                             \
            );                                                                  \
                                                                                \
            /* cmpxchg_weak */                                                  \
            do_atomic_cmpxchg_weak_explicit(                                    \
                type,                                                           \
                (volatile atomic_type *) obj,                                   \
                exp, des,                                                       \
                succ, fail,                                                     \
                ok                                                              \
            );                                                                  \
        }                                                                       \
        while (!ok);                                                            \
                                                                                \
        /* outputs */                                                           \
        return exp_bit;                                                         \
    }


/**
 * @addtogroup wrapped.cmpxchg
 *
 * @brief
 *   Defines a function which implements an atomic fetch operation using
 *   cmpxchg_weak as the underlying atomic operation.
 *
 * @details
 *   The defined function's signature will match either patomic_opsig_fetch_t
 *   or patomic_opsig_explicit_fetch_t (depending on the value of 'vis_p').
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
 *
 * @param do_make_desired
 *   A macro, D, callable as 'D(type, exp, arg, des);' in block scope.
 *
 *   The arguments provided are:
 *   - 'type' : forwarded directly from the 'type' argument in this macro
 *   - 'exp'  : the name of an identifier designating an object of type 'type'
 *   - 'arg'  : the name of an identifier designating an object of type 'type'
 *   - 'des'  : the name of an identifier designating an object of type 'type'
 *
 *   The expected behaviour of calling the macro D as above is:
 *   - 'des' should be set to the value of 'exp' after the desired modify
 *     operation is applied
 *   - 'exp' and 'arg' should not be modified
 */
#define PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH(                        \
    atomic_type, type, fn_name, vis_p, order,                           \
    do_atomic_cmpxchg_weak_explicit,                                    \
    do_make_desired                                                     \
)                                                                       \
    static void                                                         \
    fn_name(                                                            \
        volatile void *const obj                                        \
        ,const void *const argument                                     \
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
        type arg;                                                       \
        int ok;                                                         \
        const int succ = (int) order;                                   \
        const int fail = PATOMIC_CMPXCHG_FAIL_ORDER(succ);              \
                                                                        \
        /* assertions */                                                \
        PATOMIC_WRAPPED_DO_ASSERT(obj != NULL);                         \
        PATOMIC_WRAPPED_DO_ASSERT(argument != NULL);                    \
        PATOMIC_WRAPPED_DO_ASSERT(ret != NULL);                         \
        PATOMIC_WRAPPED_DO_ASSERT_ALIGNED(obj, atomic_type);            \
        PATOMIC_WRAPPED_DO_ASSERT(PATOMIC_IS_VALID_ORDER((int) order)); \
                                                                        \
        /* inputs */                                                    \
        PATOMIC_WRAPPED_DO_MEMCPY(&arg, argument, sizeof(type));        \
                                                                        \
        /* operation */                                                 \
        do {                                                            \
            /* make the desired value from the expected value */        \
            do_make_desired(                                            \
                type,                                                   \
                exp, arg,                                               \
                des                                                     \
            );                                                          \
                                                                        \
            /* cmpxchg_weak */                                          \
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


/**
 * @addtogroup wrapped.cmpxchg
 *
 * @brief
 *   Defines a function which implements an atomic noarg fetch operation using
 *   cmpxchg_weak as the underlying atomic operation.
 *
 * @details
 *   The defined function's signature will match either
 *   patomic_opsig_fetch_noarg_t or patomic_opsig_explicit_fetch_noarg_t
 *   (depending on the value of 'vis_p').
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
 *
 * @param do_make_desired
 *   A macro, D, callable as 'D(type, exp, des);' in block scope.
 *
 *   The arguments provided are:
 *   - 'type' : forwarded directly from the 'type' argument in this macro
 *   - 'exp'  : the name of an identifier designating an object of type 'type'
 *   - 'des'  : the name of an identifier designating an object of type 'type'
 *
 *   The expected behaviour of calling the macro D as above is:
 *   - 'des' should be set to the value of 'exp' after the desired modify
 *     operation is applied
 *   - 'exp' should not be modified
 */
#define PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOARG(                  \
    atomic_type, type, fn_name, vis_p, order,                           \
    do_atomic_cmpxchg_weak_explicit,                                    \
    do_make_desired                                                     \
)                                                                       \
    static void                                                         \
    fn_name(                                                            \
        volatile void *const obj                                        \
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
        int ok;                                                         \
        const int succ = (int) order;                                   \
        const int fail = PATOMIC_CMPXCHG_FAIL_ORDER(succ);              \
                                                                        \
        /* assertions */                                                \
        PATOMIC_WRAPPED_DO_ASSERT(obj != NULL);                         \
        PATOMIC_WRAPPED_DO_ASSERT(ret != NULL);                         \
        PATOMIC_WRAPPED_DO_ASSERT_ALIGNED(obj, atomic_type);            \
        PATOMIC_WRAPPED_DO_ASSERT(PATOMIC_IS_VALID_ORDER((int) order)); \
                                                                        \
        /* operation */                                                 \
        do {                                                            \
            /* make the desired value from the expected value */        \
            do_make_desired(                                            \
                type,                                                   \
                exp,                                                    \
                des                                                     \
            );                                                          \
                                                                        \
            /* cmpxchg_weak */                                          \
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


/**
 * @addtogroup wrapped.cmpxchg
 *
 * @brief
 *   Defines a function which implements an atomic void operation using
 *   cmpxchg_weak as the underlying atomic operation.
 *
 * @details
 *   The defined function's signature will match either patomic_opsig_void_t or
 *   patomic_opsig_explicit_void_t (depending on the value of 'vis_p').
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
 *
 * @param do_make_desired
 *   A macro, D, callable as 'D(type, exp, arg, des);' in block scope.
 *
 *   The arguments provided are:
 *   - 'type' : forwarded directly from the 'type' argument in this macro
 *   - 'exp'  : the name of an identifier designating an object of type 'type'
 *   - 'arg'  : the name of an identifier designating an object of type 'type'
 *   - 'des'  : the name of an identifier designating an object of type 'type'
 *
 *   The expected behaviour of calling the macro D as above is:
 *   - 'des' should be set to the value of 'exp' after the desired modify
 *     operation is applied
 *   - 'exp' and 'arg' should not be modified
 */
#define PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID(                         \
    atomic_type, type, fn_name, vis_p, order,                           \
    do_atomic_cmpxchg_weak_explicit,                                    \
    do_make_desired                                                     \
)                                                                       \
    static void                                                         \
    fn_name(                                                            \
        volatile void *const obj                                        \
        ,const void *const argument                                     \
 vis_p(_,const int order)                                               \
    )                                                                   \
    {                                                                   \
        /* static assertions */                                         \
        PATOMIC_STATIC_ASSERT(                                          \
            sizeof_type_eq_atype, sizeof(type) == sizeof(atomic_type)); \
                                                                        \
        /* declarations */                                              \
        type exp = {0};                                                 \
        type des;                                                       \
        type arg;                                                       \
        int ok;                                                         \
        const int succ = (int) order;                                   \
        const int fail = PATOMIC_CMPXCHG_FAIL_ORDER(succ);              \
                                                                        \
        /* assertions */                                                \
        PATOMIC_WRAPPED_DO_ASSERT(obj != NULL);                         \
        PATOMIC_WRAPPED_DO_ASSERT(argument != NULL);                    \
        PATOMIC_WRAPPED_DO_ASSERT_ALIGNED(obj, atomic_type);            \
        PATOMIC_WRAPPED_DO_ASSERT(PATOMIC_IS_VALID_ORDER((int) order)); \
                                                                        \
        /* inputs */                                                    \
        PATOMIC_WRAPPED_DO_MEMCPY(&arg, argument, sizeof(type));        \
                                                                        \
        /* operation */                                                 \
        do {                                                            \
            /* make the desired value from the expected value */        \
            do_make_desired(                                            \
                type,                                                   \
                exp, arg,                                               \
                des                                                     \
            );                                                          \
                                                                        \
            /* cmpxchg_weak */                                          \
            do_atomic_cmpxchg_weak_explicit(                            \
                type,                                                   \
                (volatile atomic_type *) obj,                           \
                exp, des,                                               \
                succ, fail,                                             \
                ok                                                      \
            );                                                          \
        }                                                               \
        while (!ok);                                                    \
    }


/**
 * @addtogroup wrapped.cmpxchg
 *
 * @brief
 *   Defines a function which implements an atomic void noarg operation using
 *   cmpxchg_weak as the underlying atomic operation.
 *
 * @details
 *   The defined function's signature will match either
 *   patomic_opsig_void_noarg_t or patomic_opsig_explicit_void_noarg_t
 *   (depending on the value of 'vis_p').
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
 *
 * @param do_make_desired
 *   A macro, D, callable as 'D(type, exp, des);' in block scope.
 *
 *   The arguments provided are:
 *   - 'type' : forwarded directly from the 'type' argument in this macro
 *   - 'exp'  : the name of an identifier designating an object of type 'type'
 *   - 'des'  : the name of an identifier designating an object of type 'type'
 *
 *   The expected behaviour of calling the macro D as above is:
 *   - 'des' should be set to the value of 'exp' after the desired modify
 *     operation is applied
 *   - 'exp' should not be modified
 */
#define PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_VOID_NOARG(                   \
    atomic_type, type, fn_name, vis_p, order,                           \
    do_atomic_cmpxchg_weak_explicit,                                    \
    do_make_desired                                                     \
)                                                                       \
    static void                                                         \
    fn_name(                                                            \
        volatile void *const obj                                        \
 vis_p(_,const int order)                                               \
    )                                                                   \
    {                                                                   \
        /* static assertions */                                         \
        PATOMIC_STATIC_ASSERT(                                          \
            sizeof_type_eq_atype, sizeof(type) == sizeof(atomic_type)); \
                                                                        \
        /* declarations */                                              \
        type exp = {0};                                                 \
        type des;                                                       \
        int ok;                                                         \
        const int succ = (int) order;                                   \
        const int fail = PATOMIC_CMPXCHG_FAIL_ORDER(succ);              \
                                                                        \
        /* assertions */                                                \
        PATOMIC_WRAPPED_DO_ASSERT(obj != NULL);                         \
        PATOMIC_WRAPPED_DO_ASSERT_ALIGNED(obj, atomic_type);            \
        PATOMIC_WRAPPED_DO_ASSERT(PATOMIC_IS_VALID_ORDER((int) order)); \
                                                                        \
        /* operation */                                                 \
        do {                                                            \
            /* make the desired value from the expected value */        \
            do_make_desired(                                            \
                type,                                                   \
                exp,                                                    \
                des                                                     \
            );                                                          \
                                                                        \
            /* cmpxchg_weak */                                          \
            do_atomic_cmpxchg_weak_explicit(                            \
                type,                                                   \
                (volatile atomic_type *) obj,                           \
                exp, des,                                               \
                succ, fail,                                             \
                ok                                                      \
            );                                                          \
        }                                                               \
        while (!ok);                                                    \
    }


#endif  /* PATOMIC_WRAPPED_CMPXCHG_H */
