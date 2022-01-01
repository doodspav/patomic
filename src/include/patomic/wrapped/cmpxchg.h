#ifndef PATOMIC_PATOMIC_WRAPPED_CMPXCHG_H
#define PATOMIC_PATOMIC_WRAPPED_CMPXCHG_H

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
 * - do_atomic_cmpxchg_weak_explicit:
 *   - must be callable as `fn(t, bi, by, obj_p, exp, des, succ, fail, ok);`
 *   - the result of the expression will not be assigned to anything
 *   - `t`, `bi`, and `by` will be the macro parameters `type`, `bit_width`, and
 *     `byte_width`
 *   - `obj_p` will be an expression of the form `(volatile atype *) ptr`
 *   - `exp`, `des`, `succ`, `fail`, and `ok` will be the names of local identifiers
 *   - `exp` and `des` will both have type (type)
 *   - `des` may be const-qualified
 *   - `succ` and `fail` will both have type (int), possibly const-qualified
 *   - `succ` will be a valid memory order, and `fail` will be a valid load
 *      memory order which will compare `<= succ`
 *   - `ok` will have type (int)
 *   - `ok` may be uninitialised and should be set to `0` if the operation
 *     failed or non-zero if it succeeded
 *
 * - do_cmp_neq:
 *   - must be callable as `do_cmp_neq(t, bi, by, a, b, r)`
 *   - the result of the expression will not be assigned to anything
 *   - `t`, `bi`, and `by` will be the macro parameters `type`, `bit_width`, and
 *     `byte_width`
 *   - `a`, `b`, and `r` will be the names of local identifiers
 *   - `a` and `b` will both have type (type), possibly const-qualified
 *   - `r` will have type (int)
 *   - `r` may be uninitialised and should be set to the result of the comparison
 *
 * - do_cmp_eqz:
 *   - must be callable as `do_cmp_eqz(t, bi, by, a, r)`
 *   - the result of the expression will not be assigned to anything
 *   - `t`, `bi`, and `by` will be the macro parameters `type`, `bit_width`, and
 *     `byte_width`
 *   - `a` and `r` will be the names of local identifiers
 *   - `a` will have the type (type), possibly const-qualified
 *   - `r` will have type (int)
 *   - `r` may be uninitialised and should be set to the result of the comparison
 *
 * - do_ip_nth_bit_mask:
 *   - must be callable as `do_ip_nth_bit_mask(t, bi, by, mask, n);`
 *   - the result of the expression will not be assigned to anything
 *   - `t`, `bi`, and `by` will be the macro parameters `type`, `bit_width`, and
 *     `byte_width`
 *   - `mask` and `n` will be the names of local identifiers
 *   - `mask` will have the type (type)
 *   - `n` will have the type (int), possibly const-qualified
 *   - `n` is guaranteed to be non-negative and less than the value of the
 *     expression `sizeof(type) * CHAR_BIT`
 *   - the effect should be equivalent to what `mask = 1; mask <<= n;` is for
 *     integer types
 *
 * - do_ip_bit_or:
 *   - must be callable as `do_ip_bit_or(t, bi, by, a, b);`
 *   - the result of the expression will not be assigned to anything
 *   - `t`, `bi`, and `by` will be the macro parameters `type`, `bit_width`, and
 *     `byte_width`
 *   - `a` and `b` will be the names of local identifiers
 *   - `a` and `b` will have type (type), `b` may be const-qualified
 *   - the effect should be equivalent to what `a |= b` is for integer types
 *
 * - do_ip_bit_xor:
 *   - must be callable as `do_ip_bit_xor(t, bi, by, a, b);`
 *   - the result of the expression will not be assigned to anything
 *   - `t`, `bi`, and `by` will be the macro parameters `type`, `bit_width`, and
 *     `byte_width`
 *   - `a` and `b` will be the names of local identifiers
 *   - `a` and `b` will have type (type), `b` may be const-qualified
 *   - the effect should be equivalent to what `a ^= b` is for integer types
 *
 * - do_ip_bit_and:
 *   - must be callable as `do_ip_bit_and(t, bi, by, a, b);`
 *   - the result of the expression will not be assigned to anything
 *   - `t`, `bi`, and `by` will be the macro parameters `type`, `bit_width`, and
 *     `byte_width`
 *   - `a` and `b` will be the names of local identifiers
 *   - `a` and `b` will have type (type), `b` may be const-qualified
 *   - the effect should be equivalent to what `a &= b` is for integer types
 *
 * - do_ip_bit_not:
 *   - must be callable as `do_ip_bit_not(t, bi, by, a);`
 *   - the result of the expression will not be assigned to anything
 *   - `t`, `bi`, and `by` will be the macro parameters `type`, `bit_width`, and
 *     `byte_width`
 *   - `a` will be the name of a local identifier and will have type (type)
 *   - the effect should be equivalent to what `a = (type) ~a` is for
 *     integer types
 *
 * - do_ip_add:
 *   - must be callable as `do_ip_add(t, bi, by, mv, a, b);`
 *   - the result of the expression will not be assigned to anything
 *   - `t`, `bi`, and `by` will be the macro parameters `type`, `bit_width`, and
 *     `byte_width`
 *   - `mv` will be the macro parameter `min_value`
 *   - `a` and `b` will be the names of local identifiers
 *   - `a` and `b` will have the type (type), `b` may be const-qualified
 *   - the effect should be equivalent to what `a += b` is for integer types
 *
 * - do_ip_sub:
 *   - must be callable as `do_ip_sub(t, bi, by, mv, a, b);`
 *   - the result of the expression will not be assigned to anything
 *   - `t`, `bi`, and `by` will be the macro parameters `type`, `bit_width`, and
 *     `byte_width`
 *   - `mv` will be the macro parameter `min_value`
 *   - `a` and `b` will be the names of local identifiers
 *   - `a` and `b` will have the type (type), `b` may be const-qualified
 *   - the effect should be equivalent to what `a -= b` is for integer types
 *
 * - do_ip_inc:
 *   - must be callable as `do_ip_inc(t, bi, by, mv, a);`
 *   - the result of the expression will not be assigned to anything
 *   - `t`, `bi`, and `by` will be the macro parameters `type`, `bit_width`, and
 *     `byte_width`
 *   - `mv` will be the macro parameter `min_value`
 *   - `a` will be the name of a local identifier and will have type (type)
 *   - the effect should be equivalent to what `a += 1` is for integer types
 *
 * - do_ip_dec:
 *   - must be callable as `do_ip_dec(t, bi, by, mv, a);`
 *   - the result of the expression will not be assigned to anything
 *   - `t`, `bi`, and `by` will be the macro parameters `type`, `bit_width`, and
 *     `byte_width`
 *   - `mv` will be the macro parameter `min_value`
 *   - `a` will be the name of a local identifier and will have type (type)
 *   - the effect should be equivalent to what `a -= 1` is for integer types
 *
 * - do_ip_neg:
 *   - must be callable as `do_ip_neg(t, bi, by, mv, a);`
 *   - the result of the expression will not be assigned to anything
 *   - `t`, `bi`, and `by` will be the macro parameters `type`, `bit_width`, and
 *     `byte_width`
 *   - `mv` will be the macro parameter `min_value`
 *   - `a` will be the name of a local identifier and will have type (type)
 *   - the effect should be equivalent to what `a = -a` is for integer types
 *   - the expression `-a` must yield `a` without overflow if `a` is the
 *     minimum value of a signed integer type with twos compl representation
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
 * - order:
 *   - if vis_p is SHOW_P: must be the identifier `order`
 *   - if vis_p is HIDE_P: must be a valid memory order compile time constant
 *     convertible to (int)
 *
 * - vis_p:
 *   - must be one of HIDE_P or SHOW_P
 *   - used to hide or show memory order parameters
 *   - use SHOW_P for the explicit version of a function, and HIDE_P otherwise
 *
 * - inv:
 *   - must be one of HIDE or SHOW
 *   - the inverse of vis_p (e.g. if vis_p is HIDE_P then inv would be SHOW)
 *
 * - min_val:
 *   - an (ideally compile time constant) expression of type (type) representing
 *     the minimum value representable by an object of type (type)
 *   - e.g. for `int` it would be `INT_MIN` (or for `unsigned` it would be `0u`)
 *   - this is only ever passed to the arithmetic do_ip_* macros, so
 *     realistically `min_val` can be whatever you want to accept in those
 *     macros
 *
 *
 * MACRO LOCALS
 *
 * - these identifiers will be local to the function the macro is used in
 * - their value may be undefined (they may be uninitialised)
 *
 * - temp:
 *   - has type (int)
 *
 * - scratch:
 *   - has type (type)
 */


#define PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_STORE(                              \
    bit_width, byte_width,                                                    \
    do_atomic_cmpxchg_weak_explicit,                                          \
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
        type exp_val = {0};                                                   \
        type des_val;                                                         \
        type scratch;                                                         \
        int succ;                                                             \
        int fail;                                                             \
        int ok = 0;                                                           \
        int temp;                                                             \
        /* assertions */                                                      \
        PATOMIC_WRAPPED_DO_ASSERT(obj != NULL);                               \
        PATOMIC_WRAPPED_DO_ASSERT(desired != NULL);                           \
        PATOMIC_WRAPPED_DO_ASSERT_ALIGNED(obj, atype);                        \
        PATOMIC_WRAPPED_DO_ASSERT(patomic_is_valid_store_order((int) order)); \
        /* setup */                                                           \
        succ = (int) order;                                                   \
        fail = patomic_cmpxchg_fail_order(succ);                              \
        PATOMIC_WRAPPED_DO_MEMCPY(&des_val, desired, sizeof(type));           \
        /* operation */                                                       \
        do {                                                                  \
            do_atomic_cmpxchg_weak_explicit(                                  \
                type, bit_width, byte_width,                                  \
                (volatile atype *) obj,                                       \
                exp_val, des_val,                                             \
                succ, fail, ok                                                \
            );                                                                \
        }                                                                     \
        while (!ok);                                                          \
        /* cleanup */                                                         \
        PATOMIC_IGNORE_UNUSED(scratch);                                       \
        PATOMIC_IGNORE_UNUSED(temp);                                          \
    }


#define PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_EXCHANGE(                     \
    bit_width, byte_width,                                              \
    do_atomic_cmpxchg_weak_explicit,                                    \
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
        type exp_val = {0};                                             \
        type des_val;                                                   \
        type scratch;                                                   \
        int succ;                                                       \
        int fail;                                                       \
        int ok = 0;                                                     \
        int temp;                                                       \
        /* assertions */                                                \
        PATOMIC_WRAPPED_DO_ASSERT(obj != NULL);                         \
        PATOMIC_WRAPPED_DO_ASSERT(desired != NULL);                     \
        PATOMIC_WRAPPED_DO_ASSERT(ret != NULL);                         \
        PATOMIC_WRAPPED_DO_ASSERT_ALIGNED(obj, atype);                  \
        PATOMIC_WRAPPED_DO_ASSERT(patomic_is_valid_order((int) order)); \
        /* setup */                                                     \
        succ = (int) order;                                             \
        fail = patomic_cmpxchg_fail_order(succ);                        \
        PATOMIC_WRAPPED_DO_MEMCPY(&des_val, desired, sizeof(type));     \
        /* operation */                                                 \
        do {                                                            \
            do_atomic_cmpxchg_weak_explicit(                            \
                type, bit_width, byte_width,                            \
                (volatile atype *) obj,                                 \
                exp_val, des_val,                                       \
                succ, fail, ok                                          \
            );                                                          \
        }                                                               \
        while (!ok);                                                    \
        /* cleanup */                                                   \
        PATOMIC_WRAPPED_DO_MEMCPY(ret, &exp_val, sizeof(type));         \
        PATOMIC_IGNORE_UNUSED(scratch);                                 \
        PATOMIC_IGNORE_UNUSED(temp);                                    \
    }


#define PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_CMPXCHG_STRONG(                   \
    bit_width, byte_width,                                                  \
    do_atomic_cmpxchg_weak_explicit, do_cmp_neq,                            \
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
        type old_val;  /* old value of exp_val */                           \
        type des_val;                                                       \
        type scratch;                                                       \
        int temp;                                                           \
        int ret = 0;                                                        \
        int cmp = 0;                                                        \
    inv(int succ = (int) order;)                                            \
    inv(int fail = patomic_cmpxchg_fail_order((int) order);)                \
        /* assertions */                                                    \
        PATOMIC_WRAPPED_DO_ASSERT(obj != NULL);                             \
        PATOMIC_WRAPPED_DO_ASSERT(expected != NULL);                        \
        PATOMIC_WRAPPED_DO_ASSERT(desired != NULL);                         \
        PATOMIC_WRAPPED_DO_ASSERT_ALIGNED(obj, atype);                      \
        PATOMIC_WRAPPED_DO_ASSERT(patomic_is_valid_order(succ));            \
        PATOMIC_WRAPPED_DO_ASSERT(patomic_is_valid_fail_order(succ, fail)); \
        /* setup */                                                         \
        PATOMIC_WRAPPED_DO_MEMCPY(&exp_val, expected, sizeof(type));        \
        PATOMIC_WRAPPED_DO_MEMCPY(&old_val, &exp_val, sizeof(type));        \
        PATOMIC_WRAPPED_DO_MEMCPY(&des_val, desired, sizeof(type));         \
        /* operation */                                                     \
        do {                                                                \
            do_atomic_cmpxchg_weak_explicit(                                \
                type, bit_width, byte_width,                                \
                (volatile atype *) obj,                                     \
                exp_val, des_val,                                           \
                succ, fail, ret                                             \
            );                                                              \
            do_cmp_neq(                                                     \
                type, bit_width, byte_width,                                \
                exp_val, old_val, cmp                                       \
            );                                                              \
            /* expected was modified */                                     \
            if (cmp) { break; }                                             \
            /* spurious fail */                                             \
            /* use memcpy instead of assignment operation */                \
            /* in case op compares bitwise instead of by value */           \
            else { PATOMIC_WRAPPED_DO_MEMCPY(&old_val, &exp_val, sizeof(type)); }  \
        }                                                                   \
        while (!ret);                                                       \
        /* cleanup */                                                       \
        PATOMIC_WRAPPED_DO_MEMCPY(expected, &exp_val, sizeof(type));        \
        PATOMIC_IGNORE_UNUSED(scratch);                                     \
        PATOMIC_IGNORE_UNUSED(temp);                                        \
        return ret != 0;                                                    \
    }


#define PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_BIT_TEST_COMPL(                      \
    bit_width, byte_width,                                                     \
    do_atomic_cmpxchg_weak_explicit, do_cmp_eqz,                               \
    do_ip_nth_bit_mask, do_ip_bit_and, do_ip_bit_xor,                          \
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
        type exp_val = {0};                                                    \
        type des_val;                                                          \
        type mask;                                                             \
        type scratch;                                                          \
        int succ;                                                              \
        int fail;                                                              \
        int ok = 0;                                                            \
        int temp;                                                              \
        /* assertions */                                                       \
        PATOMIC_WRAPPED_DO_ASSERT(obj != NULL);                                \
        PATOMIC_WRAPPED_DO_ASSERT_ALIGNED(obj, atype);                         \
        PATOMIC_WRAPPED_DO_ASSERT(offset >= 0);                                \
        PATOMIC_WRAPPED_DO_ASSERT((CHAR_BIT * sizeof(type)) > (size_t)offset); \
        PATOMIC_WRAPPED_DO_ASSERT(patomic_is_valid_order((int) order));        \
        /* setup */                                                            \
        succ = (int) order;                                                    \
        fail = patomic_cmpxchg_fail_order(succ);                               \
        do_ip_nth_bit_mask(                                                    \
            type, bit_width, byte_width,                                       \
            mask, offset                                                       \
        );                                                                     \
        /* operation */                                                        \
        do {                                                                   \
            PATOMIC_WRAPPED_DO_MEMCPY(&des_val, &exp_val, sizeof(type));       \
            do_ip_bit_xor(                                                     \
                type, bit_width, byte_width,                                   \
                des_val, mask                                                  \
            );                                                                 \
            do_atomic_cmpxchg_weak_explicit(                                   \
                type, bit_width, byte_width,                                   \
                (volatile atype *) obj,                                        \
                exp_val, des_val,                                              \
                succ, fail, ok                                                 \
            );                                                                 \
        }                                                                      \
        while (!ok);                                                           \
        /* cleanup */                                                          \
        PATOMIC_IGNORE_UNUSED(scratch);                                        \
        PATOMIC_IGNORE_UNUSED(temp);                                           \
        do_ip_bit_and(                                                         \
            type, bit_width, byte_width,                                       \
            exp_val, mask                                                      \
        );                                                                     \
        do_cmp_eqz(                                                            \
            type, bit_width, byte_width,                                       \
            exp_val, ok                                                        \
        );                                                                     \
        return !ok;                                                            \
    }


#define PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_BIT_TEST_SET(                        \
    bit_width, byte_width,                                                     \
    do_atomic_cmpxchg_weak_explicit, do_cmp_eqz,                               \
    do_ip_nth_bit_mask, do_ip_bit_and, do_ip_bit_or,                           \
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
        type exp_val = {0};                                                    \
        type des_val;                                                          \
        type mask;                                                             \
        type scratch;                                                          \
        int succ;                                                              \
        int fail;                                                              \
        int ok = 0;                                                            \
        int temp;                                                              \
        /* assertions */                                                       \
        PATOMIC_WRAPPED_DO_ASSERT(obj != NULL);                                \
        PATOMIC_WRAPPED_DO_ASSERT_ALIGNED(obj, atype);                         \
        PATOMIC_WRAPPED_DO_ASSERT(offset >= 0);                                \
        PATOMIC_WRAPPED_DO_ASSERT((CHAR_BIT * sizeof(type)) > (size_t)offset); \
        PATOMIC_WRAPPED_DO_ASSERT(patomic_is_valid_order((int) order));        \
        /* setup */                                                            \
        succ = (int) order;                                                    \
        fail = patomic_cmpxchg_fail_order(succ);                               \
        do_ip_nth_bit_mask(                                                    \
            type, bit_width, byte_width,                                       \
            mask, offset                                                       \
        );                                                                     \
        /* operation */                                                        \
        do {                                                                   \
            PATOMIC_WRAPPED_DO_MEMCPY(&des_val, &exp_val, sizeof(type));       \
            do_ip_bit_or(                                                      \
                type, bit_width, byte_width,                                   \
                des_val, mask                                                  \
            );                                                                 \
            do_atomic_cmpxchg_weak_explicit(                                   \
                type, bit_width, byte_width,                                   \
                (volatile atype *) obj,                                        \
                exp_val, des_val,                                              \
                succ, fail, ok                                                 \
            );                                                                 \
        }                                                                      \
        while (!ok);                                                           \
        /* cleanup */                                                          \
        PATOMIC_IGNORE_UNUSED(scratch);                                        \
        PATOMIC_IGNORE_UNUSED(temp);                                           \
        do_ip_bit_and(                                                         \
            type, bit_width, byte_width,                                       \
            exp_val, mask                                                      \
        );                                                                     \
        do_cmp_eqz(                                                            \
            type, bit_width, byte_width,                                       \
            exp_val, ok                                                        \
        );                                                                     \
        return !ok;                                                            \
    }


#define PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_BIT_TEST_RESET(                      \
    bit_width, byte_width,                                                     \
    do_atomic_cmpxchg_weak_explicit, do_cmp_eqz,                               \
    do_ip_nth_bit_mask, do_ip_bit_and, do_ip_bit_not,                          \
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
        type exp_val = {0};                                                    \
        type des_val;                                                          \
        type mask;                                                             \
        type mask_inv;                                                         \
        type scratch;                                                          \
        int succ;                                                              \
        int fail;                                                              \
        int ok = 0;                                                            \
        int temp;                                                              \
        /* assertions */                                                       \
        PATOMIC_WRAPPED_DO_ASSERT(obj != NULL);                                \
        PATOMIC_WRAPPED_DO_ASSERT_ALIGNED(obj, atype);                         \
        PATOMIC_WRAPPED_DO_ASSERT(offset >= 0);                                \
        PATOMIC_WRAPPED_DO_ASSERT((CHAR_BIT * sizeof(type)) > (size_t)offset); \
        PATOMIC_WRAPPED_DO_ASSERT(patomic_is_valid_order((int) order));        \
        /* setup */                                                            \
        succ = (int) order;                                                    \
        fail = patomic_cmpxchg_fail_order(succ);                               \
        do_ip_nth_bit_mask(                                                    \
            type, bit_width, byte_width,                                       \
            mask, offset                                                       \
        );                                                                     \
        mask_inv = mask;                                                       \
        do_ip_bit_not(                                                         \
            type, bit_width, byte_width,                                       \
            mask_inv                                                           \
        );                                                                     \
        /* operation */                                                        \
        do {                                                                   \
            PATOMIC_WRAPPED_DO_MEMCPY(&des_val, &exp_val, sizeof(type));       \
            do_ip_bit_and(                                                     \
                type, bit_width, byte_width,                                   \
                des_val, mask_inv                                              \
            );                                                                 \
            do_atomic_cmpxchg_weak_explicit(                                   \
                type, bit_width, byte_width,                                   \
                (volatile atype *) obj,                                        \
                exp_val, des_val,                                              \
                succ, fail, ok                                                 \
            );                                                                 \
        }                                                                      \
        while (!ok);                                                           \
        /* cleanup */                                                          \
        PATOMIC_IGNORE_UNUSED(scratch);                                        \
        PATOMIC_IGNORE_UNUSED(temp);                                           \
        do_ip_bit_and(                                                         \
            type, bit_width, byte_width,                                       \
            exp_val, mask                                                      \
        );                                                                     \
        do_cmp_eqz(                                                            \
            type, bit_width, byte_width,                                       \
            exp_val, ok                                                        \
        );                                                                     \
        return !ok;                                                            \
    }


#define PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_OR(                     \
    bit_width, byte_width,                                              \
    do_atomic_cmpxchg_weak_explicit, do_ip_bit_or,                      \
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
        type exp_val = {0};                                             \
        type des_val;                                                   \
        type arg_val;                                                   \
        type scratch;                                                   \
        int succ;                                                       \
        int fail;                                                       \
        int ok = 0;                                                     \
        int temp;                                                       \
        /* assertions */                                                \
        PATOMIC_WRAPPED_DO_ASSERT(obj != NULL);                         \
        PATOMIC_WRAPPED_DO_ASSERT(arg != NULL);                         \
        PATOMIC_WRAPPED_DO_ASSERT(ret != NULL);                         \
        PATOMIC_WRAPPED_DO_ASSERT_ALIGNED(obj, atype);                  \
        PATOMIC_WRAPPED_DO_ASSERT(patomic_is_valid_order((int) order)); \
        /* setup */                                                     \
        succ = (int) order;                                             \
        fail = patomic_cmpxchg_fail_order(succ);                        \
        PATOMIC_WRAPPED_DO_MEMCPY(&arg_val, arg, sizeof(type));         \
        /* operation */                                                 \
        do {                                                            \
            des_val = exp_val;                                          \
            do_ip_bit_or(                                               \
                type, bit_width, byte_width,                            \
                des_val, arg_val                                        \
            );                                                          \
            do_atomic_cmpxchg_weak_explicit(                            \
                type, bit_width, byte_width,                            \
                (volatile atype *) obj,                                 \
                exp_val, des_val,                                       \
                succ, fail, ok                                          \
            );                                                          \
        }                                                               \
        while (!ok);                                                    \
        /* cleanup */                                                   \
        PATOMIC_IGNORE_UNUSED(scratch);                                 \
        PATOMIC_IGNORE_UNUSED(temp);                                    \
        PATOMIC_WRAPPED_DO_MEMCPY(ret, &exp_val, sizeof(type));         \
    }


#define PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_XOR(                    \
    bit_width, byte_width,                                              \
    do_atomic_cmpxchg_weak_explicit, do_ip_bit_xor,                     \
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
        type exp_val = {0};                                             \
        type des_val;                                                   \
        type arg_val;                                                   \
        type scratch;                                                   \
        int succ;                                                       \
        int fail;                                                       \
        int ok = 0;                                                     \
        int temp;                                                       \
        /* assertions */                                                \
        PATOMIC_WRAPPED_DO_ASSERT(obj != NULL);                         \
        PATOMIC_WRAPPED_DO_ASSERT(arg != NULL);                         \
        PATOMIC_WRAPPED_DO_ASSERT(ret != NULL);                         \
        PATOMIC_WRAPPED_DO_ASSERT_ALIGNED(obj, atype);                  \
        PATOMIC_WRAPPED_DO_ASSERT(patomic_is_valid_order((int) order)); \
        /* setup */                                                     \
        succ = (int) order;                                             \
        fail = patomic_cmpxchg_fail_order(succ);                        \
        PATOMIC_WRAPPED_DO_MEMCPY(&arg_val, arg, sizeof(type));         \
        /* operation */                                                 \
        do {                                                            \
            des_val = exp_val;                                          \
            do_ip_bit_xor(                                              \
                type, bit_width, byte_width,                            \
                des_val, arg_val                                        \
            );                                                          \
            do_atomic_cmpxchg_weak_explicit(                            \
                type, bit_width, byte_width,                            \
                (volatile atype *) obj,                                 \
                exp_val, des_val,                                       \
                succ, fail, ok                                          \
            );                                                          \
        }                                                               \
        while (!ok);                                                    \
        /* cleanup */                                                   \
        PATOMIC_IGNORE_UNUSED(scratch);                                 \
        PATOMIC_IGNORE_UNUSED(temp);                                    \
        PATOMIC_WRAPPED_DO_MEMCPY(ret, &exp_val, sizeof(type));         \
    }


#define PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_AND(                    \
    bit_width, byte_width,                                              \
    do_atomic_cmpxchg_weak_explicit, do_ip_bit_and,                     \
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
        type exp_val = {0};                                             \
        type des_val;                                                   \
        type arg_val;                                                   \
        type scratch;                                                   \
        int succ;                                                       \
        int fail;                                                       \
        int ok = 0;                                                     \
        int temp;                                                       \
        /* assertions */                                                \
        PATOMIC_WRAPPED_DO_ASSERT(obj != NULL);                         \
        PATOMIC_WRAPPED_DO_ASSERT(arg != NULL);                         \
        PATOMIC_WRAPPED_DO_ASSERT(ret != NULL);                         \
        PATOMIC_WRAPPED_DO_ASSERT_ALIGNED(obj, atype);                  \
        PATOMIC_WRAPPED_DO_ASSERT(patomic_is_valid_order((int) order)); \
        /* setup */                                                     \
        succ = (int) order;                                             \
        fail = patomic_cmpxchg_fail_order(succ);                        \
        PATOMIC_WRAPPED_DO_MEMCPY(&arg_val, arg, sizeof(type));         \
        /* operation */                                                 \
        do {                                                            \
            des_val = exp_val;                                          \
            do_ip_bit_and(                                              \
                type, bit_width, byte_width,                            \
                des_val, arg_val                                        \
            );                                                          \
            do_atomic_cmpxchg_weak_explicit(                            \
                type, bit_width, byte_width,                            \
                (volatile atype *) obj,                                 \
                exp_val, des_val,                                       \
                succ, fail, ok                                          \
            );                                                          \
        }                                                               \
        while (!ok);                                                    \
        /* cleanup */                                                   \
        PATOMIC_IGNORE_UNUSED(scratch);                                 \
        PATOMIC_IGNORE_UNUSED(temp);                                    \
        PATOMIC_WRAPPED_DO_MEMCPY(ret, &exp_val, sizeof(type));         \
    }


#define PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NOT(                    \
    bit_width, byte_width,                                              \
    do_atomic_cmpxchg_weak_explicit, do_ip_bit_not,                     \
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
        type exp_val = {0};                                             \
        type des_val;                                                   \
        type scratch;                                                   \
        int succ;                                                       \
        int fail;                                                       \
        int ok = 0;                                                     \
        int temp;                                                       \
        /* assertions */                                                \
        PATOMIC_WRAPPED_DO_ASSERT(obj != NULL);                         \
        PATOMIC_WRAPPED_DO_ASSERT(ret != NULL);                         \
        PATOMIC_WRAPPED_DO_ASSERT_ALIGNED(obj, atype);                  \
        PATOMIC_WRAPPED_DO_ASSERT(patomic_is_valid_order((int) order)); \
        /* setup */                                                     \
        succ = (int) order;                                             \
        fail = patomic_cmpxchg_fail_order(succ);                        \
        /* operation */                                                 \
        do {                                                            \
            des_val = exp_val;                                          \
            do_ip_bit_not(                                              \
                type, bit_width, byte_width,                            \
                des_val                                                 \
            );                                                          \
            do_atomic_cmpxchg_weak_explicit(                            \
                type, bit_width, byte_width,                            \
                (volatile atype *) obj,                                 \
                exp_val, des_val,                                       \
                succ, fail, ok                                          \
            );                                                          \
        }                                                               \
        while (!ok);                                                    \
        /* cleanup */                                                   \
        PATOMIC_IGNORE_UNUSED(scratch);                                 \
        PATOMIC_IGNORE_UNUSED(temp);                                    \
        PATOMIC_WRAPPED_DO_MEMCPY(ret, &exp_val, sizeof(type));         \
    }


#define PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_ADD(                    \
    bit_width, byte_width,                                              \
    do_atomic_cmpxchg_weak_explicit, do_ip_add,                         \
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
        type exp_val = {0};                                             \
        type des_val;                                                   \
        type arg_val;                                                   \
        type scratch;                                                   \
        int succ;                                                       \
        int fail;                                                       \
        int ok = 0;                                                     \
        int temp;                                                       \
        /* assertions */                                                \
        PATOMIC_WRAPPED_DO_ASSERT(obj != NULL);                         \
        PATOMIC_WRAPPED_DO_ASSERT(arg != NULL);                         \
        PATOMIC_WRAPPED_DO_ASSERT(ret != NULL);                         \
        PATOMIC_WRAPPED_DO_ASSERT_ALIGNED(obj, atype);                  \
        PATOMIC_WRAPPED_DO_ASSERT(patomic_is_valid_order((int) order)); \
        /* setup */                                                     \
        succ = (int) order;                                             \
        fail = patomic_cmpxchg_fail_order(succ);                        \
        PATOMIC_WRAPPED_DO_MEMCPY(&arg_val, arg, sizeof(type));         \
        /* operation */                                                 \
        do {                                                            \
            des_val = exp_val;                                          \
            do_ip_add(                                                  \
                type, bit_width, byte_width, min_val,                   \
                des_val, arg_val                                        \
            );                                                          \
            do_atomic_cmpxchg_weak_explicit(                            \
                type, bit_width, byte_width,                            \
                (volatile atype *) obj,                                 \
                exp_val, des_val,                                       \
                succ, fail, ok                                          \
            );                                                          \
        }                                                               \
        while (!ok);                                                    \
        /* cleanup */                                                   \
        PATOMIC_IGNORE_UNUSED(scratch);                                 \
        PATOMIC_IGNORE_UNUSED(temp);                                    \
        PATOMIC_WRAPPED_DO_MEMCPY(ret, &exp_val, sizeof(type));         \
    }


#define PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_SUB(                    \
    bit_width, byte_width,                                              \
    do_atomic_cmpxchg_weak_explicit, do_ip_sub,                         \
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
        type exp_val = {0};                                             \
        type des_val;                                                   \
        type arg_val;                                                   \
        type scratch;                                                   \
        int succ;                                                       \
        int fail;                                                       \
        int ok = 0;                                                     \
        int temp;                                                       \
        /* assertions */                                                \
        PATOMIC_WRAPPED_DO_ASSERT(obj != NULL);                         \
        PATOMIC_WRAPPED_DO_ASSERT(arg != NULL);                         \
        PATOMIC_WRAPPED_DO_ASSERT(ret != NULL);                         \
        PATOMIC_WRAPPED_DO_ASSERT_ALIGNED(obj, atype);                  \
        PATOMIC_WRAPPED_DO_ASSERT(patomic_is_valid_order((int) order)); \
        /* setup */                                                     \
        succ = (int) order;                                             \
        fail = patomic_cmpxchg_fail_order(succ);                        \
        PATOMIC_WRAPPED_DO_MEMCPY(&arg_val, arg, sizeof(type));         \
        /* operation */                                                 \
        do {                                                            \
            des_val = exp_val;                                          \
            do_ip_sub(                                                  \
                type, bit_width, byte_width, min_val,                   \
                des_val, arg_val                                        \
            );                                                          \
            do_atomic_cmpxchg_weak_explicit(                            \
                type, bit_width, byte_width,                            \
                (volatile atype *) obj,                                 \
                exp_val, des_val,                                       \
                succ, fail, ok                                          \
            );                                                          \
        }                                                               \
        while (!ok);                                                    \
        /* cleanup */                                                   \
        PATOMIC_IGNORE_UNUSED(scratch);                                 \
        PATOMIC_IGNORE_UNUSED(temp);                                    \
        PATOMIC_WRAPPED_DO_MEMCPY(ret, &exp_val, sizeof(type));         \
    }


#define PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_INC(                    \
    bit_width, byte_width,                                              \
    do_atomic_cmpxchg_weak_explicit, do_ip_inc,                         \
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
        type exp_val = {0};                                             \
        type des_val;                                                   \
        type scratch;                                                   \
        int succ;                                                       \
        int fail;                                                       \
        int ok = 0;                                                     \
        int temp;                                                       \
        /* assertions */                                                \
        PATOMIC_WRAPPED_DO_ASSERT(obj != NULL);                         \
        PATOMIC_WRAPPED_DO_ASSERT(ret != NULL);                         \
        PATOMIC_WRAPPED_DO_ASSERT_ALIGNED(obj, atype);                  \
        PATOMIC_WRAPPED_DO_ASSERT(patomic_is_valid_order((int) order)); \
        /* setup */                                                     \
        succ = (int) order;                                             \
        fail = patomic_cmpxchg_fail_order(succ);                        \
        /* operation */                                                 \
        do {                                                            \
            des_val = exp_val;                                          \
            do_ip_inc(                                                  \
                type, bit_width, byte_width, min_val,                   \
                des_val                                                 \
            );                                                          \
            do_atomic_cmpxchg_weak_explicit(                            \
                type, bit_width, byte_width,                            \
                (volatile atype *) obj,                                 \
                exp_val, des_val,                                       \
                succ, fail, ok                                          \
            );                                                          \
        }                                                               \
        while (!ok);                                                    \
        /* cleanup */                                                   \
        PATOMIC_IGNORE_UNUSED(scratch);                                 \
        PATOMIC_IGNORE_UNUSED(temp);                                    \
        PATOMIC_WRAPPED_DO_MEMCPY(ret, &exp_val, sizeof(type));         \
    }


#define PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_DEC(                    \
    bit_width, byte_width,                                              \
    do_atomic_cmpxchg_weak_explicit, do_ip_dec,                         \
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
        type exp_val = {0};                                             \
        type des_val;                                                   \
        type scratch;                                                   \
        int succ;                                                       \
        int fail;                                                       \
        int ok = 0;                                                     \
        int temp;                                                       \
        /* assertions */                                                \
        PATOMIC_WRAPPED_DO_ASSERT(obj != NULL);                         \
        PATOMIC_WRAPPED_DO_ASSERT(ret != NULL);                         \
        PATOMIC_WRAPPED_DO_ASSERT_ALIGNED(obj, atype);                  \
        PATOMIC_WRAPPED_DO_ASSERT(patomic_is_valid_order((int) order)); \
        /* setup */                                                     \
        succ = (int) order;                                             \
        fail = patomic_cmpxchg_fail_order(succ);                        \
        /* operation */                                                 \
        do {                                                            \
            des_val = exp_val;                                          \
            do_ip_dec(                                                  \
                type, bit_width, byte_width, min_val,                   \
                des_val                                                 \
            );                                                          \
            do_atomic_cmpxchg_weak_explicit(                            \
                type, bit_width, byte_width,                            \
                (volatile atype *) obj,                                 \
                exp_val, des_val,                                       \
                succ, fail, ok                                          \
            );                                                          \
        }                                                               \
        while (!ok);                                                    \
        /* cleanup */                                                   \
        PATOMIC_IGNORE_UNUSED(scratch);                                 \
        PATOMIC_IGNORE_UNUSED(temp);                                    \
        PATOMIC_WRAPPED_DO_MEMCPY(ret, &exp_val, sizeof(type));         \
    }


#define PATOMIC_WRAPPED_CMPXCHG_DEFINE_OP_FETCH_NEG(                    \
    bit_width, byte_width,                                              \
    do_atomic_cmpxchg_weak_explicit, do_ip_neg,                         \
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
        type exp_val = {0};                                             \
        type des_val;                                                   \
        type scratch;                                                   \
        int succ;                                                       \
        int fail;                                                       \
        int ok = 0;                                                     \
        int temp;                                                       \
        /* assertions */                                                \
        PATOMIC_WRAPPED_DO_ASSERT(obj != NULL);                         \
        PATOMIC_WRAPPED_DO_ASSERT(ret != NULL);                         \
        PATOMIC_WRAPPED_DO_ASSERT_ALIGNED(obj, atype);                  \
        PATOMIC_WRAPPED_DO_ASSERT(patomic_is_valid_order((int) order)); \
        /* setup */                                                     \
        succ = (int) order;                                             \
        fail = patomic_cmpxchg_fail_order(succ);                        \
        /* operation */                                                 \
        do {                                                            \
            des_val = exp_val;                                          \
            do_ip_neg(                                                  \
                type, bit_width, byte_width, min_val,                   \
                des_val                                                 \
            );                                                          \
            do_atomic_cmpxchg_weak_explicit(                            \
                type, bit_width, byte_width,                            \
                (volatile atype *) obj,                                 \
                exp_val, des_val,                                       \
                succ, fail, ok                                          \
            );                                                          \
        }                                                               \
        while (!ok);                                                    \
        /* cleanup */                                                   \
        PATOMIC_IGNORE_UNUSED(scratch);                                 \
        PATOMIC_IGNORE_UNUSED(temp);                                    \
        PATOMIC_WRAPPED_DO_MEMCPY(ret, &exp_val, sizeof(type));         \
    }

#endif  /* !PATOMIC_PATOMIC_WRAPPED_CMPXCHG_H */
