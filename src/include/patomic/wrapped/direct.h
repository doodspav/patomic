#ifndef PATOMIC_PATOMIC_WRAPPED_DIRECT_H
#define PATOMIC_PATOMIC_WRAPPED_DIRECT_H

#include <limits.h>
#include <stddef.h>

#include <patomic/types/memory_order.h>

#include <patomic/macros/force_inline.h>
#include <patomic/macros/ignore_unused.h>

#include <patomic/wrapped/do.h>


/*
 * MACRO PARAMETERS
 *
 * - bit_width:
 *   - must be an int literal
 *   - must either have the value of `sizeof(type) * CHAR_BIT` or be `0`
 *
 * - byte_width:
 *   - must be an int literal
 *   - must either have the value of `sizeof(type)` or be `0`
 *
 * - type:
 *   - the non-qualified version of `atype` (e.g. `int`)
 *   - must not be a VLA or array of unknown size
 *
 * - atype:
 *   - the type the atomic operation will be performed on (e.g. `_Atomic(int)`)
 *   - sizeof(atype) must equal sizeof(type)
 *   - may be volatile-qualified but must not be const-qualified
 *   - must not be a VLA or array of unknown size
 *
 * - fn_name:
 *   - the full name for the function to be defined
 *   - the aforementioned function will always be defined with `static` linkage
 *
 * - order:
 *   - if `vis_p` is `SHOW_P`: must be the literal token `order`
 *   - if `vis_p` is `HIDE_P`: must be an (ideally compile time constant)
 *     expression convertible to (int) with the value of a valid memory order
 *     for the given operation
 *
 * - vis_p:
 *   - must be one of `HIDE_P` or `SHOW_P`
 *   - used to hide or show memory order parameters
 *   - use `SHOW_P` for the explicit version of a function, and `HIDE_P` otherwise
 *
 * - inv:
 *   - must be one of `HIDE` or `SHOW`
 *   - the inverse of `vis_p` (e.g. if `vis_p` is `HIDE_P` then `inv` would
 *     be `SHOW`)
 *
 * - min_val:
 *   - an (ideally compile time constant) expression of type (type) whose value
 *     is the minimum value representable by an object of type (type)
 *   - e.g. for `int` it would be `INT_MIN` (or for `unsigned` it would be `0u`)
 *   - this is only ever passed to the arithmetic `do_atomic_*` macros, so
 *     realistically `min_val` can be whatever you want to accept in those
 *     macros
 *
 *
 * MACRO LOCALS
 *
 * - these identifiers will be local to the function the macros is used in
 * - they may be uninitialised, otherwise their value is unspecified
 *
 * - temp:
 *   - has type (int)
 *
 * - scratch:
 *   - has type (type)
 */


/*
 * - do_atomic_store_explicit:
 *   - must be callable as `fn(t, bi, by, obj_p, des, order);`
 *   - the result of the expression will not be assigned to anything
 *   - `t`, `bi`, and `by` will be the macro parameters `type`, `bit_width`, and
 *     `byte_width`
 *   - `obj_p` will be an expression of the form `(volatile atype *) ptr`
 *   - `des` will be the name of a local identifier
 *   - `des` will have type (type), possibly const-qualified
 *   - `order` will be an expression of type (int) whose value is a valid
 *     store memory order
 */
#define PATOMIC_WRAPPED_DIRECT_DEFINE_OP_STORE(                               \
    bit_width, byte_width,                                                    \
    do_atomic_store_explicit,                                                 \
    type, atype, fn_name, order, vis_p                                        \
)                                                                             \
    static PATOMIC_FORCE_INLINE void                                          \
    fn_name(                                                                  \
        volatile void *obj                                                    \
        ,const void *desired                                                  \
 vis_p(_,int order)                                                           \
    )                                                                         \
    {                                                                         \
        /* declarations */                                                    \
        type des_val;                                                         \
        type scratch;                                                         \
        int temp;                                                             \
        /* assertions */                                                      \
        PATOMIC_WRAPPED_DO_ASSERT(obj != NULL);                               \
        PATOMIC_WRAPPED_DO_ASSERT(desired != NULL);                           \
        PATOMIC_WRAPPED_DO_ASSERT_ALIGNED(obj, atype);                        \
        PATOMIC_WRAPPED_DO_ASSERT(patomic_is_valid_store_order((int) order)); \
        /* setup */                                                           \
        PATOMIC_WRAPPED_DO_MEMCPY(&des_val, desired, sizeof(type));           \
        /* operation */                                                       \
        do_atomic_store_explicit(                                             \
            type, bit_width, byte_width,                                      \
            (volatile atype *) obj,                                           \
            des_val,                                                          \
            (int) order                                                       \
        );                                                                    \
        /* cleanup */                                                         \
        PATOMIC_IGNORE_UNUSED(scratch);                                       \
        PATOMIC_IGNORE_UNUSED(temp);                                          \
    }


/*
 * - do_atomic_load_explicit:
 *   - must be callable as `fn(t, bi, by, obj_p, order, res);`
 *   - the result of the expression will not be assigned to anything
 *   - `t`, `bi`, and `by` will be the macro parameters `type`, `bit_width`, and
 *     `byte_width`
 *   - `obj_p` will be an expression of the form `(const volatile atype *) ptr`
 *   - `res` will be the name of a local identifier
 *   - `res` will have type (type)
 *   - `order` will be an expression of type (int) whose value is a valid load
 *     memory order
 */
#define PATOMIC_WRAPPED_DIRECT_DEFINE_OP_LOAD(                               \
    bit_width, byte_width,                                                   \
    do_atomic_load_explicit,                                                 \
    type, atype, fn_name, order, vis_p                                       \
)                                                                            \
    static PATOMIC_FORCE_INLINE void                                         \
    fn_name(                                                                 \
        const volatile void *obj                                             \
 vis_p(_,int order)                                                          \
        ,void *ret                                                           \
    )                                                                        \
    {                                                                        \
        /* declarations */                                                   \
        type ret_val;                                                        \
        type scratch;                                                        \
        int temp;                                                            \
        /* assertions */                                                     \
        PATOMIC_WRAPPED_DO_ASSERT(obj != NULL);                              \
        PATOMIC_WRAPPED_DO_ASSERT(ret != NULL);                              \
        PATOMIC_WRAPPED_DO_ASSERT_ALIGNED(obj, atype);                       \
        PATOMIC_WRAPPED_DO_ASSERT(patomic_is_valid_load_order((int) order)); \
        /* operation */                                                      \
        do_atomic_load_explicit(                                             \
            type, bit_width, byte_width,                                     \
            (const volatile atype *) obj,                                    \
            (int) order,                                                     \
            ret_val                                                          \
        );                                                                   \
        /* cleanup */                                                        \
        PATOMIC_WRAPPED_DO_MEMCPY(ret, &ret_val, sizeof(type));              \
        PATOMIC_IGNORE_UNUSED(scratch);                                      \
        PATOMIC_IGNORE_UNUSED(temp);                                         \
    }


/*
 * - do_atomic_exchange_explicit:
 *   - must be callable as `fn(t, bi, by, obj_p, des, order, res);`
 *   - the result of the expression will not be assigned to anything
 *   - `t`, `bi`, and `by` will be the macro parameters `type`, `bit_width`, and
 *     `byte_width`
 *   - `obj_p` will be an expression of the form `(volatile atype *) ptr`
 *   - `des` and `res` will be the names of local identifiers
 *   - `des` will have the type (type), possibly const-qualified
 *   - `res` will have the type (type)
 *   - `order` will be an expression of type (int) whose value is a valid
 *     memory order
 */
#define PATOMIC_WRAPPED_DIRECT_DEFINE_OP_EXCHANGE(                      \
    bit_width, byte_width,                                              \
    do_atomic_exchange_explicit,                                        \
    type, atype, fn_name, order, vis_p                                  \
)                                                                       \
    static PATOMIC_FORCE_INLINE void                                    \
    fn_name(                                                            \
        volatile void *obj                                              \
        ,const void *desired                                            \
 vis_p(_,int order)                                                     \
        ,void *ret                                                      \
    )                                                                   \
    {                                                                   \
        /* declarations */                                              \
        type des_val;                                                   \
        type ret_val;                                                   \
        type scratch;                                                   \
        int temp;                                                       \
        /* assertions */                                                \
        PATOMIC_WRAPPED_DO_ASSERT(obj != NULL);                         \
        PATOMIC_WRAPPED_DO_ASSERT(desired != NULL);                     \
        PATOMIC_WRAPPED_DO_ASSERT(ret != NULL);                         \
        PATOMIC_WRAPPED_DO_ASSERT_ALIGNED(obj, atype);                  \
        PATOMIC_WRAPPED_DO_ASSERT(patomic_is_valid_order((int) order)); \
        /* setup */                                                     \
        PATOMIC_WRAPPED_DO_MEMCPY(&des_val, desired, sizeof(type));     \
        /* operation */                                                 \
        do_atomic_exchange_explicit(                                    \
            type, bit_width, byte_width,                                \
            (volatile atype *) obj,                                     \
            des_val,                                                    \
            (int) order,                                                \
            ret_val                                                     \
        );                                                              \
        /* cleanup */                                                   \
        PATOMIC_WRAPPED_DO_MEMCPY(ret, &ret_val, sizeof(type));         \
        PATOMIC_IGNORE_UNUSED(scratch);                                 \
        PATOMIC_IGNORE_UNUSED(temp);                                    \
    }


/*
 * - do_atomic_cmpxchg_explicit:
 *   - must be callable as `fn(t, bi, by, obj_p, exp, des, succ, fail, ok);`
 *   - the result of the expression will not be assigned to anything
 *   - `t`, `bi`, and `by` will be the macro parameters `type`, `bit_width`, and
 *     `byte_width`
 *   - `obj_p` will be an expression of the form `(volatile atype *) ptr`
 *   - `exp`, `des`, and `ok` will be the names of local identifiers
 *   - `exp` will have the type (type)
 *   - `des` will have the type (type), possibly const-qualified
 *   - `succ` will be an expression of type (int) whose value is a valid
 *     memory order
 *   - `fail` will be an expression of type (int) whose value is a valid load
 *     memory order which  is `<= succ`
 *   - `ok` will have type (int)
 *   - `ok` may be uninitialised and should be set to `0` if the operation
 *     failed or non-zero if it succeeded
 */
#define PATOMIC_WRAPPED_DIRECT_IMPL_DEFINE_OP_CMPXCHG(                      \
    bit_width, byte_width,                                                  \
    do_atomic_cmpxchg_explicit,                                             \
    type, atype, fn_name, order, vis_p, inv                                 \
)                                                                           \
    static PATOMIC_FORCE_INLINE int                                         \
    fn_name(                                                                \
        volatile void *obj                                                  \
        ,void *expected                                                     \
        ,const void *desired                                                \
 vis_p(_,int succ)                                                          \
 vis_p(_,int fail)                                                          \
    )                                                                       \
    {                                                                       \
        /* declarations */                                                  \
        type exp_val;                                                       \
        type des_val;                                                       \
        type scratch;                                                       \
        int ret = 0;                                                        \
        int temp;                                                           \
    inv(int succ = (int) order;)                                            \
    inv(int fail = patomic_cmpxchg_fail_order(succ);)                       \
        /* assertions */                                                    \
        PATOMIC_WRAPPED_DO_ASSERT(obj != NULL);                             \
        PATOMIC_WRAPPED_DO_ASSERT(expected != NULL);                        \
        PATOMIC_WRAPPED_DO_ASSERT(desired != NULL);                         \
        PATOMIC_WRAPPED_DO_ASSERT_ALIGNED(obj, atype);                      \
        PATOMIC_WRAPPED_DO_ASSERT(patomic_is_valid_order(succ));            \
        PATOMIC_WRAPPED_DO_ASSERT(patomic_is_valid_fail_order(succ, fail)); \
        /* setup */                                                         \
        PATOMIC_WRAPPED_DO_MEMCPY(&des_val, desired, sizeof(type));         \
        PATOMIC_WRAPPED_DO_MEMCPY(&exp_val, expected, sizeof(type));        \
        /* operation */                                                     \
        do_atomic_cmpxchg_explicit(                                         \
            type, bit_width, byte_width,                                    \
            (volatile atype *) obj,                                         \
            exp_val, des_val,                                               \
            succ, fail, ret                                                 \
        );                                                                  \
        /* cleanup */                                                       \
        PATOMIC_WRAPPED_DO_MEMCPY(expected, &exp_val, sizeof(type));        \
        PATOMIC_IGNORE_UNUSED(scratch);                                     \
        PATOMIC_IGNORE_UNUSED(temp);                                        \
        return ret != 0;                                                    \
    }

#define PATOMIC_WRAPPED_DIRECT_DEFINE_OP_CMPXCHG_WEAK \
    PATOMIC_WRAPPED_DIRECT_IMPL_DEFINE_OP_CMPXCHG

#define PATOMIC_WRAPPED_DIRECT_DEFINE_OP_CMPXCHG_STRONG \
    PATOMIC_WRAPPED_DIRECT_IMPL_DEFINE_OP_CMPXCHG


/*
 * - do_atomic_bit_test_explicit:
 *   - must be callable as `fn(t, bi, by, obj_p, offset, order, res);`
 *   - the result of the expression will not be assigned to anything
 *   - `t`, `bi`, and `by` will be the macro parameters `type`, `bit_width`, and
 *     `byte_width`
 *   - `obj_p` will be an expression of the form `(const volatile atype *) ptr`
 *   - `offset` and `res` will be the names of local identifiers
 *   - `offset` will have the type (int), possibly const-qualified, and will
 *     have a value `>= 0` and `< (sizeof(type) * CHAR_BIT)`
 *   - `order` will be an expression of type (int) whose value is a valid load
 *     memory order
 *   - `res` will have type (int)
 *   - `res` may be uninitialised and should be set to the old value of the bit
 *     being tested (zero or non-zero)
 */
#define PATOMIC_WRAPPED_DIRECT_DEFINE_OP_BIT_TEST(                             \
    bit_width, byte_width,                                                     \
    do_atomic_bit_test_explicit,                                               \
    type, atype, fn_name, order, vis_p                                         \
)                                                                              \
    static PATOMIC_FORCE_INLINE int                                            \
    fn_name(                                                                   \
        const volatile void *obj                                               \
        ,int offset                                                            \
 vis_p(_,int order)                                                            \
    )                                                                          \
    {                                                                          \
        /* declarations */                                                     \
        type scratch;                                                          \
        int temp;                                                              \
        int ret;                                                               \
        /* assertions */                                                       \
        PATOMIC_WRAPPED_DO_ASSERT(obj != NULL);                                \
        PATOMIC_WRAPPED_DO_ASSERT_ALIGNED(obj, atype);                         \
        PATOMIC_WRAPPED_DO_ASSERT(offset >= 0);                                \
        PATOMIC_WRAPPED_DO_ASSERT((size_t)offset < (sizeof(type) * CHAR_BIT)); \
        PATOMIC_WRAPPED_DO_ASSERT(patomic_is_valid_load_order((int) order));   \
        /* operation */                                                        \
        do_atomic_bit_test_explicit(                                           \
            type, bit_width, byte_width,                                       \
            (const volatile atype *) obj,                                      \
            offset,                                                            \
            (int) order,                                                       \
            ret                                                                \
        );                                                                     \
        /* cleanup */                                                          \
        PATOMIC_IGNORE_UNUSED(scratch);                                        \
        PATOMIC_IGNORE_UNUSED(temp);                                           \
        return ret != 0;                                                       \
    }


/*
 * - do_atomic_bit_test_modify_explicit:
 *   - must be callable as `fn(t, bi, by, obj_p, offset, order, res);`
 *   - the result of the expression will not be assigned to anything
 *   - `t`, `bi`, and `by` will be the macro parameters `type`, `bit_width`, and
 *     `byte_width`
 *   - `obj_p` will be an expression of the form `(volatile atype *) ptr`
 *   - `offset` and `res` will be the names of local identifiers
 *   - `offset` will have the type (int), possibly const-qualified, and will
 *     have a value `>= 0` and `< (sizeof(type) * CHAR_BIT)`
 *   - `order` will be an expression of type (int) whose value is a valid
 *     memory order
 *   - `res` will have type (int)
 *   - `res` may be uninitialised and should be set to the old value of the bit
 *     being tested (zero or non-zero)
 */
#define PATOMIC_WRAPPED_DIRECT_IMPL_DEFINE_OP_BIT_TEST_MODIFY(                 \
    bit_width, byte_width,                                                     \
    do_atomic_bit_test_modify_explicit,                                        \
    type, atype, fn_name, order, vis_p                                         \
)                                                                              \
    static PATOMIC_FORCE_INLINE int                                            \
    fn_name(                                                                   \
        volatile void *obj                                                     \
        ,int offset                                                            \
 vis_p(_,int order)                                                            \
    )                                                                          \
    {                                                                          \
        /* declarations */                                                     \
        type scratch;                                                          \
        int temp;                                                              \
        int ret;                                                               \
        /* assertions */                                                       \
        PATOMIC_WRAPPED_DO_ASSERT(obj != NULL);                                \
        PATOMIC_WRAPPED_DO_ASSERT_ALIGNED(obj, atype);                         \
        PATOMIC_WRAPPED_DO_ASSERT(offset >= 0);                                \
        PATOMIC_WRAPPED_DO_ASSERT((size_t)offset < (sizeof(type) * CHAR_BIT)); \
        PATOMIC_WRAPPED_DO_ASSERT(patomic_is_valid_order((int) order));        \
        /* operation */                                                        \
        do_atomic_bit_test_modify_explicit(                                    \
            type, bit_width, byte_width,                                       \
            (volatile atype *) obj,                                            \
            offset,                                                            \
            (int) order,                                                       \
            ret                                                                \
        );                                                                     \
        /* cleanup */                                                          \
        PATOMIC_IGNORE_UNUSED(scratch);                                        \
        PATOMIC_IGNORE_UNUSED(temp);                                           \
        return ret != 0;                                                       \
    }

#define PATOMIC_WRAPPED_DIRECT_DEFINE_OP_BIT_TEST_COMPL \
    PATOMIC_WRAPPED_DIRECT_IMPL_DEFINE_OP_BIT_TEST_MODIFY

#define PATOMIC_WRAPPED_DIRECT_DEFINE_OP_BIT_TEST_SET \
    PATOMIC_WRAPPED_DIRECT_IMPL_DEFINE_OP_BIT_TEST_MODIFY

#define PATOMIC_WRAPPED_DIRECT_DEFINE_OP_BIT_TEST_RESET \
    PATOMIC_WRAPPED_DIRECT_IMPL_DEFINE_OP_BIT_TEST_MODIFY


/*
 * - do_atomic_binary_op_fetch_arg_explicit:
 *   - must be callable as `fn(t, bi, by, obj_p, arg, order, ret);`
 *   - the result of the expression will not be assigned to anything
 *   - `t`, `bi`, and `by` will be the macro parameters `type`, `bit_width`, and
 *     `byte_width`
 *   - `obj_p` will be an expression of the form `(volatile atype *) ptr`
 *   - `arg` and `ret` will be the names of local identifiers
 *   - `arg` will have the type (int), possibly const-qualified
 *   - `ret` will have type (type) and may be uninitialised
 *   - `order` will be an expression of type (int) whose value is a valid
 *     memory order
 */
#define PATOMIC_WRAPPED_DIRECT_IMPL_DEFINE_BINARY_OP_FETCH_ARG(         \
    bit_width, byte_width,                                              \
    do_atomic_binary_op_fetch_arg_explicit,                             \
    type, atype, fn_name, order, vis_p                                  \
)                                                                       \
    static PATOMIC_FORCE_INLINE void                                    \
    fn_name(                                                            \
        volatile void *obj                                              \
        ,const void *arg                                                \
 vis_p(_,int order)                                                     \
        ,void *ret                                                      \
    )                                                                   \
    {                                                                   \
        /* declarations */                                              \
        type arg_val;                                                   \
        type ret_val;                                                   \
        type scratch;                                                   \
        int temp;                                                       \
        /* assertions */                                                \
        PATOMIC_WRAPPED_DO_ASSERT(obj != NULL);                         \
        PATOMIC_WRAPPED_DO_ASSERT(arg != NULL);                         \
        PATOMIC_WRAPPED_DO_ASSERT(ret != NULL);                         \
        PATOMIC_WRAPPED_DO_ASSERT_ALIGNED(obj, atype);                  \
        PATOMIC_WRAPPED_DO_ASSERT(patomic_is_valid_order((int) order)); \
        /* setup */                                                     \
        PATOMIC_WRAPPED_DO_MEMCPY(&arg_val, arg, sizeof(type));         \
        /* operation */                                                 \
        do_atomic_binary_op_fetch_arg_explicit(                         \
            type, bit_width, byte_width,                                \
            (volatile atype *) obj,                                     \
            arg_val,                                                    \
            (int) order,                                                \
            ret_val                                                     \
        );                                                              \
        /* cleanup */                                                   \
        PATOMIC_WRAPPED_DO_MEMCPY(ret, &ret_val, sizeof(type));         \
        PATOMIC_IGNORE_UNUSED(scratch);                                 \
        PATOMIC_IGNORE_UNUSED(temp);                                    \
    }

#define PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH_OR \
    PATOMIC_WRAPPED_DIRECT_IMPL_DEFINE_BINARY_OP_FETCH_ARG

#define PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH_XOR \
    PATOMIC_WRAPPED_DIRECT_IMPL_DEFINE_BINARY_OP_FETCH_ARG

#define PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH_AND \
    PATOMIC_WRAPPED_DIRECT_IMPL_DEFINE_BINARY_OP_FETCH_ARG


/*
 * - do_atomic_fetch_not_explicit:
 *   - must be callable as `fn(t, bi, by, obj_p, order, ret);`
 *   - the result of the expression will not be assigned to anything
 *   - `t`, `bi`, and `by` will be the macro parameters `type`, `bit_width`, and
 *     `byte_width`
 *   - `obj_p` will be an expression of the form `(volatile atype *) ptr`
 *   - `ret` will be the name of a local identifier
 *   - `ret` will have type (type) and may be uninitialised
 *   - `order` will be an expression of type (int) whose value is a valid
 *     memory order
 */
#define PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH_NOT(                     \
    bit_width, byte_width,                                              \
    do_atomic_fetch_not_explicit,                                       \
    type, atype, fn_name, order, vis_p                                  \
)                                                                       \
    static PATOMIC_FORCE_INLINE void                                    \
    fn_name(                                                            \
        volatile void *obj                                              \
 vis_p(_,int order)                                                     \
        ,void *ret                                                      \
    )                                                                   \
    {                                                                   \
        /* declarations */                                              \
        type ret_val;                                                   \
        type scratch;                                                   \
        int temp;                                                       \
        /* assertions */                                                \
        PATOMIC_WRAPPED_DO_ASSERT(obj != NULL);                         \
        PATOMIC_WRAPPED_DO_ASSERT(ret != NULL);                         \
        PATOMIC_WRAPPED_DO_ASSERT_ALIGNED(obj, atype);                  \
        PATOMIC_WRAPPED_DO_ASSERT(patomic_is_valid_order((int) order)); \
        /* operation */                                                 \
        do_atomic_fetch_not_explicit(                                   \
            type, bit_width, byte_width,                                \
            (volatile atype *) obj,                                     \
            (int) order,                                                \
            ret_val                                                     \
        );                                                              \
        /* cleanup */                                                   \
        PATOMIC_WRAPPED_DO_MEMCPY(ret, &ret_val, sizeof(type));         \
        PATOMIC_IGNORE_UNUSED(scratch);                                 \
        PATOMIC_IGNORE_UNUSED(temp);                                    \
    }


/*
 * - do_atomic_arithmetic_op_fetch_arg_explicit:
 *   - must be callable as `fn(t, bi, by, mv, obj_p, arg, order, ret);`
 *   - the result of the expression will not be assigned to anything
 *   - `t`, `bi`, `by`, and `mv` will be the macro parameters `type`,
 *     `bit_width`, `byte_width`, and `min_val`
 *   - `obj_p` will be an expression of the form `(volatile atype *) ptr`
 *   - `arg` and `ret` will be the names of local identifiers
 *   - `arg` will have the type (int), possibly const-qualified
 *   - `ret` will have type (type) and may be uninitialised
 *   - `order` will be an expression of type (int) whose value is a valid
 *     memory order
 */
#define PATOMIC_WRAPPED_DIRECT_IMPL_DEFINE_ARITHMETIC_OP_FETCH_ARG(     \
    bit_width, byte_width,                                              \
    do_atomic_arithmetic_op_fetch_arg_explicit,                         \
    type, atype, fn_name, order, vis_p, min_val                         \
)                                                                       \
    static PATOMIC_FORCE_INLINE void                                    \
    fn_name(                                                            \
        volatile void *obj                                              \
        ,const void *arg                                                \
 vis_p(_,int order)                                                     \
        ,void *ret                                                      \
    )                                                                   \
    {                                                                   \
        /* declarations */                                              \
        type arg_val;                                                   \
        type ret_val;                                                   \
        type scratch;                                                   \
        int temp;                                                       \
        /* assertions */                                                \
        PATOMIC_WRAPPED_DO_ASSERT(obj != NULL);                         \
        PATOMIC_WRAPPED_DO_ASSERT(arg != NULL);                         \
        PATOMIC_WRAPPED_DO_ASSERT(ret != NULL);                         \
        PATOMIC_WRAPPED_DO_ASSERT_ALIGNED(obj, atype);                  \
        PATOMIC_WRAPPED_DO_ASSERT(patomic_is_valid_order((int) order)); \
        /* setup */                                                     \
        PATOMIC_WRAPPED_DO_MEMCPY(&arg_val, arg, sizeof(type));         \
        /* operation */                                                 \
        do_atomic_arithmetic_op_fetch_arg_explicit(                     \
            type, bit_width, byte_width, min_val,                       \
            (volatile atype *) obj,                                     \
            arg_val,                                                    \
            (int) order,                                                \
            ret_val                                                     \
        );                                                              \
        /* cleanup */                                                   \
        PATOMIC_WRAPPED_DO_MEMCPY(ret, &ret_val, sizeof(type));         \
        PATOMIC_IGNORE_UNUSED(scratch);                                 \
        PATOMIC_IGNORE_UNUSED(temp);                                    \
    }

#define PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH_ADD \
    PATOMIC_WRAPPED_DIRECT_IMPL_DEFINE_ARITHMETIC_OP_FETCH_ARG

#define PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH_SUB \
    PATOMIC_WRAPPED_DIRECT_IMPL_DEFINE_ARITHMETIC_OP_FETCH_ARG


/*
 * - do_atomic_arithmetic_op_fetch_void_explicit:
 *   - must be callable as `fn(t, bi, by, mv, obj_p, order, ret);`
 *   - the result of the expression will not be assigned to anything
 *   - `t`, `bi`, `by`, and `mv` will be the macro parameters `type`,
 *     `bit_width`, `byte_width`, and `min_val`
 *   - `obj_p` will be an expression of the form `(volatile atype *) ptr`
 *   - `ret` will be the name of a local identifier
 *   - `ret` will have type (type) and may be uninitialised
 *   - `order` will be an expression of type (int) whose value is a valid
 *     memory order
 */
#define PATOMIC_WRAPPED_DIRECT_IMPL_DEFINE_ARITHMETIC_OP_FETCH_VOID(    \
    bit_width, byte_width,                                              \
    do_atomic_arithmetic_op_fetch_void_explicit,                        \
    type, atype, fn_name, order, vis_p, min_val                         \
)                                                                       \
    static PATOMIC_FORCE_INLINE void                                    \
    fn_name(                                                            \
        volatile void *obj                                              \
 vis_p(_,int order)                                                     \
        ,void *ret                                                      \
    )                                                                   \
    {                                                                   \
        /* declarations */                                              \
        type ret_val;                                                   \
        type scratch;                                                   \
        int temp;                                                       \
        /* assertions */                                                \
        PATOMIC_WRAPPED_DO_ASSERT(obj != NULL);                         \
        PATOMIC_WRAPPED_DO_ASSERT(ret != NULL);                         \
        PATOMIC_WRAPPED_DO_ASSERT_ALIGNED(obj, atype);                  \
        PATOMIC_WRAPPED_DO_ASSERT(patomic_is_valid_order((int) order)); \
        /* operation */                                                 \
        do_atomic_arithmetic_op_fetch_void_explicit(                    \
            type, bit_width, byte_width, min_val,                       \
            (volatile atype *) obj,                                     \
            (int) order,                                                \
            ret_val                                                     \
        );                                                              \
        /* cleanup */                                                   \
        PATOMIC_WRAPPED_DO_MEMCPY(ret, &ret_val, sizeof(type));         \
        PATOMIC_IGNORE_UNUSED(scratch);                                 \
        PATOMIC_IGNORE_UNUSED(temp);                                    \
    }

#define PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH_INC \
    PATOMIC_WRAPPED_DIRECT_IMPL_DEFINE_ARITHMETIC_OP_FETCH_VOID

#define PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH_DEC \
    PATOMIC_WRAPPED_DIRECT_IMPL_DEFINE_ARITHMETIC_OP_FETCH_VOID

#define PATOMIC_WRAPPED_DIRECT_DEFINE_OP_FETCH_NEG \
    PATOMIC_WRAPPED_DIRECT_IMPL_DEFINE_ARITHMETIC_OP_FETCH_VOID


#endif  /* !PATOMIC_PATOMIC_WRAPPED_DIRECT_H */
