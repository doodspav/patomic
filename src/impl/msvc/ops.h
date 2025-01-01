/* Copyright (c) doodspav. */
/* SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception */

#ifndef PATOMIC_IMPL_MSVC_OPS_H
#define PATOMIC_IMPL_MSVC_OPS_H

#ifdef _MSC_VER

#include <stddef.h>


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


#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    #define VIS_FP_ACQ(expr) expr
    #define VIS_FP_REL(expr) expr
#else
    #define VIS_FP_ACQ(expr)
    #define VIS_FP_REL(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    #define VIS_FP_NF(expr) expr
#else
    #define VIS_FP_NF(expr)
#endif


#if PATOMIC_IMPL_MSVC_HAS_OP_LOAD_8
    #define VIS_FP_LOAD_8(expr) expr
#else
    #define VIS_FP_LOAD_8(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_STORE_8
    #define VIS_FP_STORE_8(expr) expr
#else
    #define VIS_FP_STORE_8(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_COMPARE_EXCHANGE_8
    #define VIS_FP_COMPARE_EXCHANGE_8(expr) expr
#else
    #define VIS_FP_COMPARE_EXCHANGE_8(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_EXCHANGE_8
    #define VIS_FP_EXCHANGE_8(expr) expr
#else
    #define VIS_FP_EXCHANGE_8(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_BIT_TEST_8
    #define VIS_FP_BIT_TEST_8(expr) expr
#else
    #define VIS_FP_BIT_TEST_8(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_BIT_TEST_COMPL_8
    #define VIS_FP_BIT_TEST_COMPL_8(expr) expr
#else
    #define VIS_FP_BIT_TEST_COMPL_8(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_BIT_TEST_RESET_8
    #define VIS_FP_BIT_TEST_RESET_8(expr) expr
#else
    #define VIS_FP_BIT_TEST_RESET_8(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_BIT_TEST_SET_8
    #define VIS_FP_BIT_TEST_SET_8(expr) expr
#else
    #define VIS_FP_BIT_TEST_SET_8(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_OR_8
    #define VIS_FP_OR_8(expr) expr
#else
    #define VIS_FP_OR_8(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_XOR_8
    #define VIS_FP_XOR_8(expr) expr
#else
    #define VIS_FP_XOR_8(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_AND_8
    #define VIS_FP_AND_8(expr) expr
#else
    #define VIS_FP_AND_8(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_NOT_8
    #define VIS_FP_NOT_8(expr) expr
#else
    #define VIS_FP_NOT_8(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_ADD_8
    #define VIS_FP_ADD_8(expr) expr
#else
    #define VIS_FP_ADD_8(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_SUB_8
    #define VIS_FP_SUB_8(expr) expr
#else
    #define VIS_FP_SUB_8(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_INC_8
    #define VIS_FP_INC_8(expr) expr
#else
    #define VIS_FP_INC_8(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_DEC_8
    #define VIS_FP_DEC_8(expr) expr
#else
    #define VIS_FP_DEC_8(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_NEG_8
    #define VIS_FP_NEG_8(expr) expr
#else
    #define VIS_FP_NEG_8(expr)
#endif


#if PATOMIC_IMPL_MSVC_HAS_OP_LOAD_16
    #define VIS_FP_LOAD_16(expr) expr
#else
    #define VIS_FP_LOAD_16(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_STORE_16
    #define VIS_FP_STORE_16(expr) expr
#else
    #define VIS_FP_STORE_16(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_COMPARE_EXCHANGE_16
    #define VIS_FP_COMPARE_EXCHANGE_16(expr) expr
#else
    #define VIS_FP_COMPARE_EXCHANGE_16(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_EXCHANGE_16
    #define VIS_FP_EXCHANGE_16(expr) expr
#else
    #define VIS_FP_EXCHANGE_16(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_BIT_TEST_16
    #define VIS_FP_BIT_TEST_16(expr) expr
#else
    #define VIS_FP_BIT_TEST_16(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_BIT_TEST_COMPL_16
    #define VIS_FP_BIT_TEST_COMPL_16(expr) expr
#else
    #define VIS_FP_BIT_TEST_COMPL_16(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_BIT_TEST_RESET_16
    #define VIS_FP_BIT_TEST_RESET_16(expr) expr
#else
    #define VIS_FP_BIT_TEST_RESET_16(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_BIT_TEST_SET_16
    #define VIS_FP_BIT_TEST_SET_16(expr) expr
#else
    #define VIS_FP_BIT_TEST_SET_16(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_OR_16
    #define VIS_FP_OR_16(expr) expr
#else
    #define VIS_FP_OR_16(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_XOR_16
    #define VIS_FP_XOR_16(expr) expr
#else
    #define VIS_FP_XOR_16(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_AND_16
    #define VIS_FP_AND_16(expr) expr
#else
    #define VIS_FP_AND_16(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_NOT_16
    #define VIS_FP_NOT_16(expr) expr
#else
    #define VIS_FP_NOT_16(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_ADD_16
    #define VIS_FP_ADD_16(expr) expr
#else
    #define VIS_FP_ADD_16(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_SUB_16
    #define VIS_FP_SUB_16(expr) expr
#else
    #define VIS_FP_SUB_16(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_INC_16
    #define VIS_FP_INC_16(expr) expr
#else
    #define VIS_FP_INC_16(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_DEC_16
    #define VIS_FP_DEC_16(expr) expr
#else
    #define VIS_FP_DEC_16(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_NEG_16
    #define VIS_FP_NEG_16(expr) expr
#else
    #define VIS_FP_NEG_16(expr)
#endif


#if PATOMIC_IMPL_MSVC_HAS_OP_LOAD_32
    #define VIS_FP_LOAD_32(expr) expr
#else
    #define VIS_FP_LOAD_32(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_STORE_32
    #define VIS_FP_STORE_32(expr) expr
#else
    #define VIS_FP_STORE_32(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_COMPARE_EXCHANGE_32
    #define VIS_FP_COMPARE_EXCHANGE_32(expr) expr
#else
    #define VIS_FP_COMPARE_EXCHANGE_32(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_EXCHANGE_32
    #define VIS_FP_EXCHANGE_32(expr) expr
#else
    #define VIS_FP_EXCHANGE_32(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_BIT_TEST_32
    #define VIS_FP_BIT_TEST_32(expr) expr
#else
    #define VIS_FP_BIT_TEST_32(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_BIT_TEST_COMPL_32
    #define VIS_FP_BIT_TEST_COMPL_32(expr) expr
#else
    #define VIS_FP_BIT_TEST_COMPL_32(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_BIT_TEST_RESET_32
    #define VIS_FP_BIT_TEST_RESET_32(expr) expr
#else
    #define VIS_FP_BIT_TEST_RESET_32(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_BIT_TEST_SET_32
    #define VIS_FP_BIT_TEST_SET_32(expr) expr
#else
    #define VIS_FP_BIT_TEST_SET_32(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_OR_32
    #define VIS_FP_OR_32(expr) expr
#else
    #define VIS_FP_OR_32(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_XOR_32
    #define VIS_FP_XOR_32(expr) expr
#else
    #define VIS_FP_XOR_32(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_AND_32
    #define VIS_FP_AND_32(expr) expr
#else
    #define VIS_FP_AND_32(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_NOT_32
    #define VIS_FP_NOT_32(expr) expr
#else
    #define VIS_FP_NOT_32(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_ADD_32
    #define VIS_FP_ADD_32(expr) expr
#else
    #define VIS_FP_ADD_32(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_SUB_32
    #define VIS_FP_SUB_32(expr) expr
#else
    #define VIS_FP_SUB_32(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_INC_32
    #define VIS_FP_INC_32(expr) expr
#else
    #define VIS_FP_INC_32(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_DEC_32
    #define VIS_FP_DEC_32(expr) expr
#else
    #define VIS_FP_DEC_32(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_NEG_32
    #define VIS_FP_NEG_32(expr) expr
#else
    #define VIS_FP_NEG_32(expr)
#endif


#if PATOMIC_IMPL_MSVC_HAS_OP_LOAD_64
    #define VIS_FP_LOAD_64(expr) expr
#else
    #define VIS_FP_LOAD_64(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_STORE_64
    #define VIS_FP_STORE_64(expr) expr
#else
    #define VIS_FP_STORE_64(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_COMPARE_EXCHANGE_64
    #define VIS_FP_COMPARE_EXCHANGE_64(expr) expr
#else
    #define VIS_FP_COMPARE_EXCHANGE_64(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_EXCHANGE_64
    #define VIS_FP_EXCHANGE_64(expr) expr
#else
    #define VIS_FP_EXCHANGE_64(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_BIT_TEST_64
    #define VIS_FP_BIT_TEST_64(expr) expr
#else
    #define VIS_FP_BIT_TEST_64(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_BIT_TEST_COMPL_64
    #define VIS_FP_BIT_TEST_COMPL_64(expr) expr
#else
    #define VIS_FP_BIT_TEST_COMPL_64(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_BIT_TEST_RESET_64
    #define VIS_FP_BIT_TEST_RESET_64(expr) expr
#else
    #define VIS_FP_BIT_TEST_RESET_64(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_BIT_TEST_SET_64
    #define VIS_FP_BIT_TEST_SET_64(expr) expr
#else
    #define VIS_FP_BIT_TEST_SET_64(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_OR_64
    #define VIS_FP_OR_64(expr) expr
#else
    #define VIS_FP_OR_64(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_XOR_64
    #define VIS_FP_XOR_64(expr) expr
#else
    #define VIS_FP_XOR_64(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_AND_64
    #define VIS_FP_AND_64(expr) expr
#else
    #define VIS_FP_AND_64(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_NOT_64
    #define VIS_FP_NOT_64(expr) expr
#else
    #define VIS_FP_NOT_64(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_ADD_64
    #define VIS_FP_ADD_64(expr) expr
#else
    #define VIS_FP_ADD_64(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_SUB_64
    #define VIS_FP_SUB_64(expr) expr
#else
    #define VIS_FP_SUB_64(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_INC_64
    #define VIS_FP_INC_64(expr) expr
#else
    #define VIS_FP_INC_64(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_DEC_64
    #define VIS_FP_DEC_64(expr) expr
#else
    #define VIS_FP_DEC_64(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_NEG_64
    #define VIS_FP_NEG_64(expr) expr
#else
    #define VIS_FP_NEG_64(expr)
#endif


#if PATOMIC_IMPL_MSVC_HAS_OP_LOAD_128
    #define VIS_FP_LOAD_128(expr) expr
#else
    #define VIS_FP_LOAD_128(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_STORE_128
    #define VIS_FP_STORE_128(expr) expr
#else
    #define VIS_FP_STORE_128(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_COMPARE_EXCHANGE_128
    #define VIS_FP_COMPARE_EXCHANGE_128(expr) expr
#else
    #define VIS_FP_COMPARE_EXCHANGE_128(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_EXCHANGE_128
    #define VIS_FP_EXCHANGE_128(expr) expr
#else
    #define VIS_FP_EXCHANGE_128(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_BIT_TEST_128
    #define VIS_FP_BIT_TEST_128(expr) expr
#else
    #define VIS_FP_BIT_TEST_128(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_BIT_TEST_COMPL_128
    #define VIS_FP_BIT_TEST_COMPL_128(expr) expr
#else
    #define VIS_FP_BIT_TEST_COMPL_128(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_BIT_TEST_RESET_128
    #define VIS_FP_BIT_TEST_RESET_128(expr) expr
#else
    #define VIS_FP_BIT_TEST_RESET_128(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_BIT_TEST_SET_128
    #define VIS_FP_BIT_TEST_SET_128(expr) expr
#else
    #define VIS_FP_BIT_TEST_SET_128(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_OR_128
    #define VIS_FP_OR_128(expr) expr
#else
    #define VIS_FP_OR_128(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_XOR_128
    #define VIS_FP_XOR_128(expr) expr
#else
    #define VIS_FP_XOR_128(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_AND_128
    #define VIS_FP_AND_128(expr) expr
#else
    #define VIS_FP_AND_128(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_NOT_128
    #define VIS_FP_NOT_128(expr) expr
#else
    #define VIS_FP_NOT_128(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_ADD_128
    #define VIS_FP_ADD_128(expr) expr
#else
    #define VIS_FP_ADD_128(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_SUB_128
    #define VIS_FP_SUB_128(expr) expr
#else
    #define VIS_FP_SUB_128(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_INC_128
    #define VIS_FP_INC_128(expr) expr
#else
    #define VIS_FP_INC_128(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_DEC_128
    #define VIS_FP_DEC_128(expr) expr
#else
    #define VIS_FP_DEC_128(expr)
#endif

#if PATOMIC_IMPL_MSVC_HAS_OP_NEG_128
    #define VIS_FP_NEG_128(expr) expr
#else
    #define VIS_FP_NEG_128(expr)
#endif


#endif  /* defined(_MSC_VER) */

#endif  /* PATOMIC_IMPL_MSVC_OPS_H */
