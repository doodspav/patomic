/* Copyright (c) doodspav. */
/* SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception */

#ifndef PATOMIC_IMPL_MSVC_OPS_BIT_MODIFY_RESET_H
#define PATOMIC_IMPL_MSVC_OPS_BIT_MODIFY_RESET_H

#ifdef _MSC_VER

#include "../base.h"

#include <patomic/stdlib/stdint.h>

#include <patomic/wrapped/direct.h>

#include <patomic/api/ops.h>

#include <limits.h>


/**
 * @note
 *   Doing pointer hacks like this is _technically_ undefined behaviour,
 *   but msvc won't care and I would be surprised if this broke anything.
 *   It shouldn't be affected by CET or equivalent pointer checks.
 */
#define do_bit_test_reset_explicit(type, obj, offset, order, res)          \
    do {                                                                   \
        const patomic_intptr_unsigned_t addr =                             \
            (patomic_intptr_unsigned_t) obj;                               \
                                                                           \
        /* align our address to 4 bytes */                                 \
        const patomic_intptr_unsigned_t new_addr =                         \
            (patomic_intptr_unsigned_t) (addr & ~3ul);                     \
        volatile long * new_obj = (volatile long *) new_addr;              \
        offset += (int) ((addr - new_addr) * CHAR_BIT);                    \
                                                                           \
        /* update offset to be less than 32 */                             \
        new_obj += offset / 32;                                            \
        offset %= 32;                                                      \
                                                                           \
        switch (order)                                                     \
        {                                                                  \
            case RELAXED:                                                  \
      VIS_IL_NF((res = _interlockedbittestandreset_nf(new_obj, offset));)  \
      VIS_IL_NF(break;)                                                    \
            case patomic_CONSUME:                                          \
            case patomic_ACQUIRE:                                          \
     VIS_IL_ACQ((res = _interlockedbittestandreset_acq(new_obj, offset));) \
     VIS_IL_ACQ(break;)                                                    \
            case patomic_RELEASE:                                          \
     VIS_IL_REL((res = _interlockedbittestandreset_rel(new_obj, offset));) \
     VIS_IL_REL(break;)                                                    \
            case patomic_ACQ_REL:                                          \
            case patomic_SEQ_CST:                                          \
            default:                                                       \
                res = _interlockedbittestandreset(new_obj, offset);        \
        }                                                                  \
    }                                                                      \
    while (0)


#endif  /* defined(_MSC_VER) */

#endif  /* PATOMIC_IMPL_MSVC_OPS_BIT_MODIFY_RESET_H */
