/* Copyright (c) doodspav. */
/* SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception */

#ifndef PATOMIC_IMPL_MSVC_OPS_BITWISE_SET_H
#define PATOMIC_IMPL_MSVC_OPS_BITWISE_SET_H

#ifdef _MSC_VER

#include "../base.h"

#include <patomic/macros/static_assert.h>

#include <patomic/stdlib/stdint.h>

#include <patomic/wrapped/direct.h>

#include <patomic/api/ops.h>

#include <limits.h>


#if PATOMIC_IMPL_MSVC_HAS_IL_BIT_TEST_SET_32


/**
 * @note
 *   Doing pointer hacks like this is _technically_ undefined behaviour,
 *   but msvc won't care and I would be surprised if this broke anything.
 *   It shouldn't be affected by CET or equivalent pointer checks.
 */
#define do_bit_test_set_explicit(type, obj, offset, order, res)              \
    do {                                                                     \
        PATOMIC_STATIC_ASSERT(char_bit_is_8, CHAR_BIT == 8u);                \
                                                                             \
        const patomic_intptr_unsigned_t addr =                               \
            (patomic_intptr_unsigned_t) obj;                                 \
                                                                             \
        /* declarations */                                                   \
        patomic_intptr_unsigned_t new_addr;                                  \
        long *new_obj;                                                       \
        int new_offset = offset;                                             \
                                                                             \
        /* align our address to 4 bytes */                                   \
        new_addr = (patomic_intptr_unsigned_t) (                             \
            (addr & ~((patomic_intptr_unsigned_t) 3ul))                      \
        );                                                                   \
        new_obj = (long *) new_addr;                                         \
        new_offset += (int) ((addr - new_addr) * CHAR_BIT);                  \
                                                                             \
        /* update offset to be less than 32 */                               \
        new_obj += new_offset / 32;                                          \
        new_offset %= 32;                                                    \
                                                                             \
        switch (order)                                                       \
        {                                                                    \
            case patomic_RELAXED:                                            \
      VIS_IL_NF((res = _interlockedbittestandset_nf(new_obj, new_offset));)  \
      VIS_IL_NF(break;)                                                      \
            case patomic_CONSUME:                                            \
            case patomic_ACQUIRE:                                            \
     VIS_IL_ACQ((res = _interlockedbittestandset_acq(new_obj, new_offset));) \
     VIS_IL_ACQ(break;)                                                      \
            case patomic_RELEASE:                                            \
     VIS_IL_REL((res = _interlockedbittestandset_rel(new_obj, new_offset));) \
     VIS_IL_REL(break;)                                                      \
            case patomic_ACQ_REL:                                            \
            case patomic_SEQ_CST:                                            \
            default:                                                         \
                res = _interlockedbittestandset(new_obj, new_offset);        \
        }                                                                    \
    }                                                                        \
    while (0)


unsigned char _interlockedbittestandset(long *, long);
#pragma intrinsic(_interlockedbittestandset)

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    unsigned char _interlockedbittestandset_nf(long *, long);
    #pragma intrinsic(_interlockedbittestandset_nf)
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    unsigned char _interlockedbittestandset_acq(long *, long);
    unsigned char _interlockedbittestandset_rel(long *, long);
    #pragma intrinsic(_interlockedbittestandset_acq)
    #pragma intrinsic(_interlockedbittestandset_rel)
#endif


/**
 * Defines patomic_opimpl_bit_test_set_8_<order> with order:
 * - relaxed
 * - acquire
 * - release
 * - seq_cst
 * - explicit
 */

#define PATOMIC_IMPL_MSVC_HAS_OP_BIT_TEST_SET_8 1

PATOMIC_WRAPPED_DIRECT_DEFINE_OP_BIT_TEST_MODIFY(
    char, char, patomic_opimpl_bit_test_set_8_explicit,
    SHOW_P, order, do_bit_test_set_explicit
)

PATOMIC_WRAPPED_DIRECT_DEFINE_OP_BIT_TEST_MODIFY(
    char, char, patomic_opimpl_bit_test_set_8_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_bit_test_set_explicit
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_BIT_TEST_MODIFY(
        char, char, patomic_opimpl_bit_test_set_8_acquire,
        HIDE_P, patomic_ACQUIRE, do_bit_test_set_explicit
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_BIT_TEST_MODIFY(
        char, char, patomic_opimpl_bit_test_set_8_release,
        HIDE_P, patomic_RELEASE, do_bit_test_set_explicit
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_BIT_TEST_MODIFY(
        char, char, patomic_opimpl_bit_test_set_8_relaxed,
        HIDE_P, patomic_RELAXED, do_bit_test_set_explicit
    )
#endif


/**
 * Defines patomic_opimpl_bit_test_set_16_<order> with order:
 * - relaxed
 * - acquire
 * - release
 * - seq_cst
 * - explicit
 */

#define PATOMIC_IMPL_MSVC_HAS_OP_BIT_TEST_SET_16 1

PATOMIC_WRAPPED_DIRECT_DEFINE_OP_BIT_TEST_MODIFY(
    short, short, patomic_opimpl_bit_test_set_16_explicit,
    SHOW_P, order, do_bit_test_set_explicit
)

PATOMIC_WRAPPED_DIRECT_DEFINE_OP_BIT_TEST_MODIFY(
    short, short, patomic_opimpl_bit_test_set_16_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_bit_test_set_explicit
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_BIT_TEST_MODIFY(
        short, short, patomic_opimpl_bit_test_set_16_acquire,
        HIDE_P, patomic_ACQUIRE, do_bit_test_set_explicit
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_BIT_TEST_MODIFY(
        short, short, patomic_opimpl_bit_test_set_16_release,
        HIDE_P, patomic_RELEASE, do_bit_test_set_explicit
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_BIT_TEST_MODIFY(
        short, short, patomic_opimpl_bit_test_set_16_relaxed,
        HIDE_P, patomic_RELAXED, do_bit_test_set_explicit
    )
#endif


/**
 * Defines patomic_opimpl_bit_test_set_32_<order> with order:
 * - relaxed
 * - acquire
 * - release
 * - seq_cst
 * - explicit
 */

#define PATOMIC_IMPL_MSVC_HAS_OP_BIT_TEST_SET_32 1

PATOMIC_WRAPPED_DIRECT_DEFINE_OP_BIT_TEST_MODIFY(
    long, long, patomic_opimpl_bit_test_set_32_explicit,
    SHOW_P, order, do_bit_test_set_explicit
)

PATOMIC_WRAPPED_DIRECT_DEFINE_OP_BIT_TEST_MODIFY(
    long, long, patomic_opimpl_bit_test_set_32_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_bit_test_set_explicit
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_BIT_TEST_MODIFY(
        long, long, patomic_opimpl_bit_test_set_32_acquire,
        HIDE_P, patomic_ACQUIRE, do_bit_test_set_explicit
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_BIT_TEST_MODIFY(
        long, long, patomic_opimpl_bit_test_set_32_release,
        HIDE_P, patomic_RELEASE, do_bit_test_set_explicit
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_BIT_TEST_MODIFY(
        long, long, patomic_opimpl_bit_test_set_32_relaxed,
        HIDE_P, patomic_RELAXED, do_bit_test_set_explicit
    )
#endif


/**
 * Defines patomic_opimpl_bit_test_set_64_<order> with order:
 * - relaxed
 * - acquire
 * - release
 * - seq_cst
 * - explicit
 */

#define PATOMIC_IMPL_MSVC_HAS_OP_BIT_TEST_SET_64 1

PATOMIC_WRAPPED_DIRECT_DEFINE_OP_BIT_TEST_MODIFY(
    __int64, __int64, patomic_opimpl_bit_test_set_64_explicit,
    SHOW_P, order, do_bit_test_set_explicit
)

PATOMIC_WRAPPED_DIRECT_DEFINE_OP_BIT_TEST_MODIFY(
    __int64, __int64, patomic_opimpl_bit_test_set_64_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_bit_test_set_explicit
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_BIT_TEST_MODIFY(
        __int64, __int64, patomic_opimpl_bit_test_set_64_acquire,
        HIDE_P, patomic_ACQUIRE, do_bit_test_set_explicit
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_BIT_TEST_MODIFY(
        __int64, __int64, patomic_opimpl_bit_test_set_64_release,
        HIDE_P, patomic_RELEASE, do_bit_test_set_explicit
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_BIT_TEST_MODIFY(
        __int64, __int64, patomic_opimpl_bit_test_set_64_relaxed,
        HIDE_P, patomic_RELAXED, do_bit_test_set_explicit
    )
#endif


/**
 * Defines patomic_opimpl_bit_test_set_128_<order> with order:
 * - relaxed
 * - acquire
 * - release
 * - seq_cst
 * - explicit
 */

#define PATOMIC_IMPL_MSVC_HAS_OP_BIT_TEST_SET_128 1

PATOMIC_WRAPPED_DIRECT_DEFINE_OP_BIT_TEST_MODIFY(
    patomic_msvc128_t, patomic_msvc128_t, patomic_opimpl_bit_test_set_128_explicit,
    SHOW_P, order, do_bit_test_set_explicit
)

PATOMIC_WRAPPED_DIRECT_DEFINE_OP_BIT_TEST_MODIFY(
    patomic_msvc128_t, patomic_msvc128_t, patomic_opimpl_bit_test_set_128_seq_cst,
    HIDE_P, patomic_SEQ_CST, do_bit_test_set_explicit
)

#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_BIT_TEST_MODIFY(
        patomic_msvc128_t, patomic_msvc128_t, patomic_opimpl_bit_test_set_128_acquire,
        HIDE_P, patomic_ACQUIRE, do_bit_test_set_explicit
    )
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_BIT_TEST_MODIFY(
        patomic_msvc128_t, patomic_msvc128_t, patomic_opimpl_bit_test_set_128_release,
        HIDE_P, patomic_RELEASE, do_bit_test_set_explicit
    )
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    PATOMIC_WRAPPED_DIRECT_DEFINE_OP_BIT_TEST_MODIFY(
        patomic_msvc128_t, patomic_msvc128_t, patomic_opimpl_bit_test_set_128_relaxed,
        HIDE_P, patomic_RELAXED, do_bit_test_set_explicit
    )
#endif


#endif  /* PATOMIC_IMPL_MSVC_HAS_IL_BIT_TEST_SET_32 */


#endif  /* defined(_MSC_VER) */

#endif  /* PATOMIC_IMPL_MSVC_OPS_BITWISE_SET_H */
