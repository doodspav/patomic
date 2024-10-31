#ifndef PATOMIC_WRAPPED_DIRECT_H
#define PATOMIC_WRAPPED_DIRECT_H

#include "base.h"

#include <patomic/macros/static_assert.h>

#include <patomic/api/memory_order.h>

#include <limits.h>
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
 *   A macro, M, callable as 'M(type, obj, des, order);' in block scope.
 *
 *   The arguments provided are:
 *   - 'type'  : forwarded directly from the 'type' argument in this macro
 *   - 'obj'   : an expression of type 'volatile atomic_type *'
 *   - 'des'   : the name of an identifier designating an object of type 'type'
 *   - 'order' : an expression of type 'int' whose value is a valid store
 *               memory order
 *
 *   The expected behaviour of calling the macro M as above is:
 *   - the value of 'des' is read and atomically stored into the object pointed
 *     to by 'obj'
 *   - the atomic operation uses a store memory ordering at least as strong as
 *     'order'
 */
#define PATOMIC_WRAPPED_DIRECT_DEFINE_OP_STORE(                               \
    atomic_type, type, fn_name, vis_p, order,                                 \
    do_atomic_store_explicit                                                  \
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
        type des;                                                             \
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
        do_atomic_store_explicit(                                             \
            type,                                                             \
            (volatile atomic_type *) obj,                                     \
            des,                                                              \
            (int) order                                                       \
        );                                                                    \
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
 *   A macro, M, callable as 'M(type, obj, order, res);' in block scope.
 *
 *   The arguments provided are:
 *   - 'type'  : forwarded directly from the 'type' argument in this macro
 *   - 'obj'   : an expression of type 'const volatile atomic_type *'
 *   - 'order' : an expression of type 'int' whose value is a valid load memory
 *               order
 *   - 'res'   : the name of an identifier designating an object of type 'type'
 *
 *   The expected behaviour of calling the macro M as above is:
 *   - the value of the object pointed to by 'obj' is atomically read
 *   - 'res' is set to the value which was read
 *   - the atomic operation uses a load memory ordering at least as strong as
 *     'order'
 */
#define PATOMIC_WRAPPED_DIRECT_DEFINE_OP_LOAD(                               \
    atomic_type, type, fn_name, vis_p, order,                                \
    do_atomic_load_explicit                                                  \
)                                                                            \
    static void                                                              \
    fn_name(                                                                 \
        const volatile void *const obj                                       \
 vis_p(_,const int order)                                                    \
        ,void *const ret                                                     \
    )                                                                        \
    {                                                                        \
        /* static assertions */                                              \
        PATOMIC_STATIC_ASSERT(                                               \
            sizeof_type_eq_atype, sizeof(type) == sizeof(atomic_type));      \
                                                                             \
        /* declarations */                                                   \
        type res;                                                            \
                                                                             \
        /* assertions */                                                     \
        PATOMIC_WRAPPED_DO_ASSERT(obj != NULL);                              \
        PATOMIC_WRAPPED_DO_ASSERT(ret != NULL);                              \
        PATOMIC_WRAPPED_DO_ASSERT_ALIGNED(obj, atomic_type);                 \
        PATOMIC_WRAPPED_DO_ASSERT(PATOMIC_IS_VALID_LOAD_ORDER((int) order)); \
                                                                             \
        /* operation */                                                      \
        do_atomic_load_explicit(                                             \
            type,                                                            \
            (const volatile atomic_type *) obj,                              \
            (int) order,                                                     \
            res                                                              \
        );                                                                   \
                                                                             \
        /* outputs */                                                        \
        PATOMIC_WRAPPED_DO_MEMCPY(ret, &res, sizeof(type));                  \
    }


/**
 * @addtogroup wrapped.direct
 *
 * @brief
 *   Defines a function which implements an atomic exchange operation using
 *   exchange as the underlying atomic operation.
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
 * @param do_atomic_exchange_explicit
 *   A macro, M, callable as 'M(type, obj, des, order, res);' in block scope.
 *
 *   The arguments provided are:
 *   - 'type'  : forwarded directly from the 'type' argument in this macro
 *   - 'obj'   : an expression of type 'volatile atomic_type *'
 *   - 'des'   : the name of an identifier designating an object of type 'type'
 *   - 'order' : an expression of type 'int' whose value is a valid memory order
 *   - 'res'   : the name of an identifier designating an object of type 'type'
 *
 *   The expected behaviour of calling the macro M as above is:
 *   - the value of 'des' is read and, in a single atomic operation is stored
 *     into the object pointed to by 'obj' while the value of the object
 *     pointed to by 'obj' is read
 *   - the value read from 'obj' is stored into 'res'
 *   - the atomic operation uses a memory ordering at least as strong as
 *     'order'
 */
#define PATOMIC_WRAPPED_DIRECT_DEFINE_OP_EXCHANGE(                      \
    atomic_type, type, fn_name, vis_p, order,                           \
    do_atomic_exchange_explicit                                         \
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
        type des;                                                       \
        type res;                                                       \
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
        do_atomic_exchange_explicit(                                    \
            type,                                                       \
            (volatile atomic_type *) obj,                               \
            des,                                                        \
            (int) order,                                                \
            res                                                         \
        );                                                              \
                                                                        \
        /* outputs */                                                   \
        PATOMIC_WRAPPED_DO_MEMCPY(ret, &res, sizeof(type));             \
    }


/**
 * @addtogroup wrapped.direct
 *
 * @brief
 *   Defines a function which implements an atomic cmpxchg operation using
 *   cmpxchg as the underlying atomic operation.
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
 * @param do_atomic_cmpxchg_explicit
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
#define PATOMIC_WRAPPED_DIRECT_DEFINE_OP_CMPXCHG(                           \
    atomic_type, type, fn_name, vis_p, inv, order,                          \
    do_atomic_cmpxchg_explicit                                              \
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
        int ok;                                                             \
    inv(const int succ = (int) order;)                                      \
    inv(const int fail = PATOMIC_CMPXCHG_FAIL_ORDER(succ);)                 \
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
        PATOMIC_WRAPPED_DO_MEMCPY(&des, desired, sizeof(type));             \
        PATOMIC_WRAPPED_DO_MEMCPY(&exp, expected, sizeof(type));            \
                                                                            \
        /* operation */                                                     \
        do_atomic_cmpxchg_explicit(                                         \
            type,                                                           \
            (volatile atomic_type *) obj,                                   \
            exp, des,                                                       \
            succ, fail,                                                     \
            ok                                                              \
        );                                                                  \
                                                                            \
        /* outputs */                                                       \
        PATOMIC_WRAPPED_DO_MEMCPY(expected, &exp, sizeof(type));            \
        return ok != 0;                                                     \
    }


/**
 * @addtogroup wrapped.direct
 *
 * @brief
 *   Defines a function which implements an atomic bit test operation using
 *   bit test as the underlying atomic operation.
 *
 * @details
 *   The defined function's signature will match either patomic_opsig_test_t
 *   or patomic_opsig_explicit_test_t (depending on the value of 'vis_p').
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
 * @param do_atomic_bit_test_explicit
 *   A macro, M, callable as 'M(type, obj, offset, order, res);' in block scope.
 *
 *   The arguments provided are:
 *   - 'type'   : forwarded directly from the 'type' argument in this macro
 *   - 'obj'    : an expression of type 'const volatile atomic_type *'
 *   - 'offset' : an expression of type 'int' whose value is non-negative and
 *                less than 'sizeof(type) * CHAR_BIT'
 *   - 'order'  : an expression of type 'int' whose value is a valid load
 *                memory order
 *   - 'res'    : the name of an identifier designating an object of type 'int'
 *
 *   The expected behaviour of calling the macro M as above is:
 *   - the value of the object pointed to by 'obj' is atomically read
 *   - 'res' is set to the bit at offset 'offset' of the value which was read
 *   - the atomic operation uses a load memory ordering at least as strong as
 *     'order'
 */
#define PATOMIC_WRAPPED_DIRECT_DEFINE_OP_BIT_TEST(                              \
    atomic_type, type, fn_name, vis_p, order,                                   \
    do_atomic_bit_test_explicit                                                 \
)                                                                               \
    static int                                                                  \
    fn_name(                                                                    \
        const volatile void *const obj                                          \
        ,const int offset                                                       \
 vis_p(_,const int order)                                                       \
    )                                                                           \
    {                                                                           \
        /* static assertions */                                                 \
        PATOMIC_STATIC_ASSERT(                                                  \
            sizeof_type_eq_atype, sizeof(type) == sizeof(atomic_type));         \
                                                                                \
        /* declarations */                                                      \
        int res;                                                                \
                                                                                \
        /* assertions */                                                        \
        PATOMIC_WRAPPED_DO_ASSERT(obj != NULL);                                 \
        PATOMIC_WRAPPED_DO_ASSERT_ALIGNED(obj, atomic_type);                    \
        PATOMIC_WRAPPED_DO_ASSERT(offset >= 0);                                 \
        PATOMIC_WRAPPED_DO_ASSERT((size_t) offset < (sizeof(type) * CHAR_BIT)); \
        PATOMIC_WRAPPED_DO_ASSERT(PATOMIC_IS_VALID_LOAD_ORDER((int) order));    \
                                                                                \
        /* operation */                                                         \
        do_atomic_bit_test_explicit(                                            \
            type,                                                               \
            (const volatile atomic_type *) obj,                                 \
            offset,                                                             \
            (int) order,                                                        \
            res                                                                 \
        );                                                                      \
                                                                                \
        /* outputs */                                                           \
        return res != 0;                                                        \
    }


/**
 * @addtogroup wrapped.direct
 *
 * @brief
 *   Defines a function which implements an atomic bit test-modify operation
 *   using bit test-modify as the underlying atomic operation.
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
 * @param do_atomic_bit_test_modify_explicit
 *   A macro, M, callable as 'M(type, obj, offset, order, res);' in block scope.
 *
 *   The arguments provided are:
 *   - 'type'   : forwarded directly from the 'type' argument in this macro
 *   - 'obj'    : an expression of type 'volatile atomic_type *'
 *   - 'offset' : an expression of type 'int' whose value is non-negative and
 *                less than 'sizeof(type) * CHAR_BIT'
 *   - 'order'  : an expression of type 'int' whose value is a valid memory
 *                order
 *   - 'res'    : the name of an identifier designating an object of type 'int'
 *
 *   The expected behaviour of calling the macro M as above is:
 *   - the bit at offset 'offset' in the object pointed to by 'obj' is modified
 *     in a single atomic operation
 *   - the atomic operation uses a store memory ordering at least as strong as
 *     'order' if the operation does not require reading the old value,
 *     otherwise it uses a memory ordering at least as strong as 'order'
 */
#define PATOMIC_WRAPPED_DIRECT_DEFINE_OP_BIT_TEST_MODIFY(                       \
    atomic_type, type, fn_name, vis_p, order,                                   \
    do_atomic_bit_test_modify_explicit                                          \
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
        int res;                                                                \
                                                                                \
        /* assertions */                                                        \
        PATOMIC_WRAPPED_DO_ASSERT(obj != NULL);                                 \
        PATOMIC_WRAPPED_DO_ASSERT_ALIGNED(obj, atomic_type);                    \
        PATOMIC_WRAPPED_DO_ASSERT(offset >= 0);                                 \
        PATOMIC_WRAPPED_DO_ASSERT((size_t) offset < (sizeof(type) * CHAR_BIT)); \
        PATOMIC_WRAPPED_DO_ASSERT(PATOMIC_IS_VALID_ORDER((int) order));         \
                                                                                \
        /* operation */                                                         \
        do_atomic_bit_test_modify_explicit(                                     \
            type,                                                               \
            (volatile atomic_type *) obj,                                       \
            offset,                                                             \
            (int) order,                                                        \
            res                                                                 \
        );                                                                      \
                                                                                \
        /* outputs */                                                           \
        return res != 0;                                                        \
    }


/**
 * @addtogroup wrapped.direct
 *
 * @brief
 *   Defines a function which implements an atomic fetch operation using fetch
 *   as the underlying atomic operation.
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
 * @param do_atomic_fetch_explicit
 *   A macro, M, callable as 'M(type, obj, arg, order, res);' in block scope.
 *
 *   The arguments provided are:
 *   - 'type'  : forwarded directly from the 'type' argument in this macro
 *   - 'obj'   : an expression of type 'volatile atomic_type *'
 *   - 'arg'   : the name of an identifier designating an object of type 'type'
 *   - 'order' : an expression of type 'int' whose value is a valid memory order
 *   - 'res'   : the name of an identifier designating an object of type 'type'
 *
 *   The expected behaviour of calling the macro M as above is:
 *   - the value of the object pointed to by 'obj' is read and modified in a
 *     single atomic operation
 *   - 'res' is set to the original value read from the object pointed to by
 *     'obj'
 *   - the atomic operation uses a memory ordering at least as strong as
 *     'order'
 */
#define PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH(                         \
    atomic_type, type, fn_name, vis_p, order,                           \
    do_atomic_fetch_explicit                                            \
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
        type arg;                                                       \
        type res;                                                       \
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
        do_atomic_fetch_explicit(                                       \
            type,                                                       \
            (volatile atomic_type *) obj,                               \
            arg,                                                        \
            (int) order,                                                \
            res                                                         \
        );                                                              \
                                                                        \
        /* outputs */                                                   \
        PATOMIC_WRAPPED_DO_MEMCPY(ret, &res, sizeof(type));             \
    }


/**
 * @addtogroup wrapped.direct
 * 
 * @brief
 *   Defines a function which implements an atomic noarg fetch operation using
 *   noarg fetch as the underlying atomic operation.
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
 * @param do_atomic_fetch_noarg_explicit
 *   A macro, M, callable as 'M(type, obj, order, res);' in block scope.
 *
 *   The arguments provided are:
 *   - 'type'  : forwarded directly from the 'type' argument in this macro
 *   - 'obj'   : an expression of type 'volatile atomic_type *'
 *   - 'order' : an expression of type 'int' whose value is a valid memory order
 *   - 'res'   : the name of an identifier designating an object of type 'type'
 *
 *   The expected behaviour of calling the macro M as above is:
 *   - the value of the object pointed to by 'obj' is read and modified in a
 *     single atomic operation
 *   - 'res' is set to the original value read from the object pointed to by
 *     'obj'
 *   - the atomic operation uses a memory ordering at least as strong as
 *     'order'
 */
#define PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH_NOARG(                   \
    atomic_type, type, fn_name, vis_p, order,                           \
    do_atomic_fetch_noarg_explicit                                      \
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
        type res;                                                       \
                                                                        \
        /* assertions */                                                \
        PATOMIC_WRAPPED_DO_ASSERT(obj != NULL);                         \
        PATOMIC_WRAPPED_DO_ASSERT(ret != NULL);                         \
        PATOMIC_WRAPPED_DO_ASSERT_ALIGNED(obj, atomic_type);            \
        PATOMIC_WRAPPED_DO_ASSERT(PATOMIC_IS_VALID_ORDER((int) order)); \
                                                                        \
        /* operation */                                                 \
        do_atomic_fetch_noarg_explicit(                                 \
            type,                                                       \
            (volatile atomic_type *) obj,                               \
            (int) order,                                                \
            res                                                         \
        );                                                              \
                                                                        \
        /* outputs */                                                   \
        PATOMIC_WRAPPED_DO_MEMCPY(ret, &res, sizeof(type));             \
    }


/**
 * @addtogroup wrapped.direct
 *
 * @brief
 *   Defines a function which implements an atomic void operation using void
 *   as the underlying atomic operation.
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
 * @param do_atomic_void_explicit
 *   A macro, M, callable as 'M(type, obj, arg, order);' in block scope.
 *
 *   The arguments provided are:
 *   - 'type'  : forwarded directly from the 'type' argument in this macro
 *   - 'obj'   : an expression of type 'volatile atomic_type *'
 *   - 'arg'   : the name of an identifier designating an object of type 'type'
 *   - 'order' : an expression of type 'int' whose value is a valid memory order
 *
 *   The expected behaviour of calling the macro M as above is:
 *   - the value of the object pointed to by 'obj' is read and modified in a
 *     single atomic operation
 *   - the atomic operation uses a memory ordering at least as strong as
 *     'order'
 */
#define PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID(                          \
    atomic_type, type, fn_name, vis_p, order,                           \
    do_atomic_void_explicit                                             \
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
        type arg;                                                       \
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
        do_atomic_void_explicit(                                        \
            type,                                                       \
            (volatile atomic_type *) obj,                               \
            arg,                                                        \
            (int) order                                                 \
        );                                                              \
    }


/**
 * @addtogroup wrapped.direct
 *
 * @brief
 *   Defines a function which implements an atomic void noarg operation using
 *   void noarg as the underlying atomic operation.
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
 * @param do_atomic_void_noarg_explicit
 *   A macro, M, callable as 'M(type, obj, order);' in block scope.
 *
 *   The arguments provided are:
 *   - 'type'  : forwarded directly from the 'type' argument in this macro
 *   - 'obj'   : an expression of type 'volatile atomic_type *'
 *   - 'order' : an expression of type 'int' whose value is a valid memory order
 *
 *   The expected behaviour of calling the macro M as above is:
 *   - the value of the object pointed to by 'obj' is read and modified in a
 *     single atomic operation
 *   - the atomic operation uses a memory ordering at least as strong as
 *     'order'
 */
#define PATOMIC_WRAPPED_DIRECT_DEFINE_OP_VOID_NOARG(                       \
    atomic_type, type, fn_name, vis_p, order,                              \
    do_atomic_void_noarg_explicit                                          \
)                                                                          \
    static void                                                            \
    fn_name(                                                               \
        volatile void *const obj                                           \
 vis_p(_,const int order)                                                  \
    )                                                                      \
    {                                                                      \
        /* static assertions */                                            \
        PATOMIC_STATIC_ASSERT(                                             \
            sizeof_type_eq_atype, sizeof(type) == sizeof(atomic_type));    \
                                                                           \
        /* assertions */                                                   \
        PATOMIC_WRAPPED_DO_ASSERT(obj != NULL);                            \
        PATOMIC_WRAPPED_DO_ASSERT_ALIGNED(obj, atomic_type);               \
        PATOMIC_WRAPPED_DO_ASSERT(PATOMIC_IS_VALID_ORDER((int) order));    \
                                                                           \
        /* operation */                                                    \
        do_atomic_void_noarg_explicit(                                     \
            type,                                                          \
            (volatile atomic_type *) obj,                                  \
            (int) order                                                    \
        );                                                                 \
    }


#endif  /* PATOMIC_WRAPPED_DIRECT_H */
