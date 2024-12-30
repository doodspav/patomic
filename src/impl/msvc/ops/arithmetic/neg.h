/* Copyright (c) doodspav. */
/* SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception */

#ifndef PATOMIC_IMPL_MSVC_OPS_ARITHMETIC_NEG_H
#define PATOMIC_IMPL_MSVC_OPS_ARITHMETIC_NEG_H

#ifdef _MSC_VER

#include "../base.h"

#include <patomic/wrapped/cmpxchg.h>

#include <patomic/api/ops.h>


#define do_make_desired_neg(type, exp, des)                          \
    do {                                                             \
        des = (type) (~((unsigned type) exp) + ((unsigned type) 1)); \
    }                                                                \
    while (0)

#define do_make_desired_neg_128(type, exp, des)        \
    do {                                               \
        patomic_msvc128_t arg = {0};                   \
        arg.low = 1;                                   \
        des.low = (type) ~((unsigned type) exp.low);   \
        des.high = (type) ~((unsigned type) exp.high); \
        des = patomic_msvc128_add(des, arg);           \
    }                                                  \
    while (0)


#endif  /* defined(_MSC_VER) */

#endif  /* PATOMIC_IMPL_MSVC_OPS_ARITHMETIC_NEG_H */
