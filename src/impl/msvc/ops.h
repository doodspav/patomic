/* Copyright (c) doodspav. */
/* SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception */

#ifndef PATOMIC_IMPL_MSVC_OPS_H
#define PATOMIC_IMPL_MSVC_OPS_H

#ifdef _MSC_VER


/* DO NOT CHANGE ORDER WITHOUT CHECKING DEPENDENCIES */

/* xchg */
#include "ops/xchg/cmpxchg.h"
#include "ops/xchg/exchange.h"

/* ldst */
#include "ops/ldst/load.h"
#include "ops/ldst/store.h"

/* bitwise */
#include "ops/bitwise/test.h"
#include "ops/bitwise/compl.h"
#include "ops/bitwise/reset.h"
#include "ops/bitwise/set.h"

/* binary */
#include "ops/binary/or.h"
#include "ops/binary/xor.h"
#include "ops/binary/and.h"
#include "ops/binary/not.h"

/* arithmetic */
#include "ops/arithmetic/add.h"
#include "ops/arithmetic/sub.h"
#include "ops/arithmetic/inc.h"
#include "ops/arithmetic/dec.h"
#include "ops/arithmetic/neg.h"


#if PATOMIC_IMPL_MSVC_HAS_OP_COMPARE_EXCHANGE_8 || \
    PATOMIC_IMPL_MSVC_HAS_OP_EXCHANGE_8         || \
    PATOMIC_IMPL_MSVC_HAS_OP_LOAD_8             || \
    PATOMIC_IMPL_MSVC_HAS_OP_STORE_8            || \
    PATOMIC_IMPL_MSVC_HAS_OP_BIT_TEST_8         || \
    PATOMIC_IMPL_MSVC_HAS_OP_BIT_TEST_COMPL_8   || \
    PATOMIC_IMPL_MSVC_HAS_OP_BIT_TEST_RESET_8   || \
    PATOMIC_IMPL_MSVC_HAS_OP_BIT_TEST_SET_8     || \
    PATOMIC_IMPL_MSVC_HAS_OP_OR_8               || \
    PATOMIC_IMPL_MSVC_HAS_OP_XOR_8              || \
    PATOMIC_IMPL_MSVC_HAS_OP_AND_8              || \
    PATOMIC_IMPL_MSVC_HAS_OP_NOT_8              || \
    PATOMIC_IMPL_MSVC_HAS_OP_ADD_8              || \
    PATOMIC_IMPL_MSVC_HAS_OP_SUB_8              || \
    PATOMIC_IMPL_MSVC_HAS_OP_INC_8              || \
    PATOMIC_IMPL_MSVC_HAS_OP_DEC_8              || \
    PATOMIC_IMPL_MSVC_HAS_OP_NEG_8             
#define PATOMIC_IMPL_MSVC_HAS_OP_ANY_8 1
#endif


#if PATOMIC_IMPL_MSVC_HAS_OP_COMPARE_EXCHANGE_16 || \
    PATOMIC_IMPL_MSVC_HAS_OP_EXCHANGE_16         || \
    PATOMIC_IMPL_MSVC_HAS_OP_LOAD_16             || \
    PATOMIC_IMPL_MSVC_HAS_OP_STORE_16            || \
    PATOMIC_IMPL_MSVC_HAS_OP_BIT_TEST_16         || \
    PATOMIC_IMPL_MSVC_HAS_OP_BIT_TEST_COMPL_16   || \
    PATOMIC_IMPL_MSVC_HAS_OP_BIT_TEST_RESET_16   || \
    PATOMIC_IMPL_MSVC_HAS_OP_BIT_TEST_SET_16     || \
    PATOMIC_IMPL_MSVC_HAS_OP_OR_16               || \
    PATOMIC_IMPL_MSVC_HAS_OP_XOR_16              || \
    PATOMIC_IMPL_MSVC_HAS_OP_AND_16              || \
    PATOMIC_IMPL_MSVC_HAS_OP_NOT_16              || \
    PATOMIC_IMPL_MSVC_HAS_OP_ADD_16              || \
    PATOMIC_IMPL_MSVC_HAS_OP_SUB_16              || \
    PATOMIC_IMPL_MSVC_HAS_OP_INC_16              || \
    PATOMIC_IMPL_MSVC_HAS_OP_DEC_16              || \
    PATOMIC_IMPL_MSVC_HAS_OP_NEG_16
#define PATOMIC_IMPL_MSVC_HAS_OP_ANY_16 1
#endif


#if PATOMIC_IMPL_MSVC_HAS_OP_COMPARE_EXCHANGE_32 || \
    PATOMIC_IMPL_MSVC_HAS_OP_EXCHANGE_32         || \
    PATOMIC_IMPL_MSVC_HAS_OP_LOAD_32             || \
    PATOMIC_IMPL_MSVC_HAS_OP_STORE_32            || \
    PATOMIC_IMPL_MSVC_HAS_OP_BIT_TEST_32         || \
    PATOMIC_IMPL_MSVC_HAS_OP_BIT_TEST_COMPL_32   || \
    PATOMIC_IMPL_MSVC_HAS_OP_BIT_TEST_RESET_32   || \
    PATOMIC_IMPL_MSVC_HAS_OP_BIT_TEST_SET_32     || \
    PATOMIC_IMPL_MSVC_HAS_OP_OR_32               || \
    PATOMIC_IMPL_MSVC_HAS_OP_XOR_32              || \
    PATOMIC_IMPL_MSVC_HAS_OP_AND_32              || \
    PATOMIC_IMPL_MSVC_HAS_OP_NOT_32              || \
    PATOMIC_IMPL_MSVC_HAS_OP_ADD_32              || \
    PATOMIC_IMPL_MSVC_HAS_OP_SUB_32              || \
    PATOMIC_IMPL_MSVC_HAS_OP_INC_32              || \
    PATOMIC_IMPL_MSVC_HAS_OP_DEC_32              || \
    PATOMIC_IMPL_MSVC_HAS_OP_NEG_32
#define PATOMIC_IMPL_MSVC_HAS_OP_ANY_32 1
#endif


#if PATOMIC_IMPL_MSVC_HAS_OP_COMPARE_EXCHANGE_64 || \
    PATOMIC_IMPL_MSVC_HAS_OP_EXCHANGE_64         || \
    PATOMIC_IMPL_MSVC_HAS_OP_LOAD_64             || \
    PATOMIC_IMPL_MSVC_HAS_OP_STORE_64            || \
    PATOMIC_IMPL_MSVC_HAS_OP_BIT_TEST_64         || \
    PATOMIC_IMPL_MSVC_HAS_OP_BIT_TEST_COMPL_64   || \
    PATOMIC_IMPL_MSVC_HAS_OP_BIT_TEST_RESET_64   || \
    PATOMIC_IMPL_MSVC_HAS_OP_BIT_TEST_SET_64     || \
    PATOMIC_IMPL_MSVC_HAS_OP_OR_64               || \
    PATOMIC_IMPL_MSVC_HAS_OP_XOR_64              || \
    PATOMIC_IMPL_MSVC_HAS_OP_AND_64              || \
    PATOMIC_IMPL_MSVC_HAS_OP_NOT_64              || \
    PATOMIC_IMPL_MSVC_HAS_OP_ADD_64              || \
    PATOMIC_IMPL_MSVC_HAS_OP_SUB_64              || \
    PATOMIC_IMPL_MSVC_HAS_OP_INC_64              || \
    PATOMIC_IMPL_MSVC_HAS_OP_DEC_64              || \
    PATOMIC_IMPL_MSVC_HAS_OP_NEG_64
#define PATOMIC_IMPL_MSVC_HAS_OP_ANY_64 1
#endif


#if PATOMIC_IMPL_MSVC_HAS_OP_COMPARE_EXCHANGE_128 || \
    PATOMIC_IMPL_MSVC_HAS_OP_EXCHANGE_128         || \
    PATOMIC_IMPL_MSVC_HAS_OP_LOAD_128             || \
    PATOMIC_IMPL_MSVC_HAS_OP_STORE_128            || \
    PATOMIC_IMPL_MSVC_HAS_OP_BIT_TEST_128         || \
    PATOMIC_IMPL_MSVC_HAS_OP_BIT_TEST_COMPL_128   || \
    PATOMIC_IMPL_MSVC_HAS_OP_BIT_TEST_RESET_128   || \
    PATOMIC_IMPL_MSVC_HAS_OP_BIT_TEST_SET_128     || \
    PATOMIC_IMPL_MSVC_HAS_OP_OR_128               || \
    PATOMIC_IMPL_MSVC_HAS_OP_XOR_128              || \
    PATOMIC_IMPL_MSVC_HAS_OP_AND_128              || \
    PATOMIC_IMPL_MSVC_HAS_OP_NOT_128              || \
    PATOMIC_IMPL_MSVC_HAS_OP_ADD_128              || \
    PATOMIC_IMPL_MSVC_HAS_OP_SUB_128              || \
    PATOMIC_IMPL_MSVC_HAS_OP_INC_128              || \
    PATOMIC_IMPL_MSVC_HAS_OP_DEC_128              || \
    PATOMIC_IMPL_MSVC_HAS_OP_NEG_128
#define PATOMIC_IMPL_MSVC_HAS_OP_ANY_128 1
#endif


#endif  /* defined(_MSC_VER) */

#endif  /* PATOMIC_IMPL_MSVC_OPS_H */
