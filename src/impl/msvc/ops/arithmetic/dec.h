/* Copyright (c) doodspav. */
/* SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception */

#ifndef PATOMIC_IMPL_MSVC_OPS_ARITHMETIC_DEC_H
#define PATOMIC_IMPL_MSVC_OPS_ARITHMETIC_DEC_H

#ifdef _MSC_VER

#include "../base.h"

#include <patomic/macros/ignore_unused.h>

#include <patomic/wrapped/cmpxchg.h>
#include <patomic/wrapped/direct.h>

#include <patomic/api/ops.h>


#define do_fetch_dec_raw_explicit_n(n, type, obj, order, res)       \
    do {                                                            \
        switch (order)                                              \
        {                                                           \
            case patomic_RELAXED:                                   \
      VIS_IL_NF((res = _InterlockedDecrement##n##_nf(obj));)        \
      VIS_IL_NF(break;)                                             \
            case patomic_CONSUME:                                   \
            case patomic_ACQUIRE:                                   \
     VIS_IL_ACQ((res = _InterlockedDecrement##n##_acq(obj));)       \
     VIS_IL_ACQ(break;)                                             \
            case patomic_RELEASE:                                   \
     VIS_IL_REL((res = _InterlockedDecrement##n##_rel(obj));)       \
     VIS_IL_REL(break;)                                             \
            case patomic_ACQ_REL:                                   \
            case patomic_SEQ_CST:                                   \
            default:                                                \
                res = _InterlockedDecrement##n(obj);                \
        }                                                           \
        res = (type) (((unsigned type) res) + ((unsigned type) 1)); \
    }                                                               \
    while (0)

#define do_void_dec_raw_explicit_n(n, type, obj, order)        \
    do {                                                       \
        type res;                                              \
        do_fetch_dec_raw_explicit_n(n, type, obj, order, res); \
        PATOMIC_IGNORE_UNUSED(res);                            \
    }                                                          \
    while (0)

#define do_fetch_dec_sub_explicit_n(n, type, obj, order, res)   \
    do {                                                        \
        const type arg = (type) 1;                              \
        do_fetch_sub_explicit_n(n, type, obj, arg, order, res); \
    }                                                           \
    while (0)

#define do_void_dec_sub_explicit_n(n, type, obj, order)        \
    do {                                                       \
        type res;                                              \
        do_fetch_dec_sub_explicit_n(n, type, obj, order, res); \
        PATOMIC_IGNORE_UNUSED(res);                            \
    }                                                          \
    while (0)

#define do_make_desired_dec(type, exp, des)                         \
    do {                                                            \
        des = (type) (((unsigned type) exp) - ((unsigned type) 1)); \
    }                                                               \
    while (0)

#define do_make_desired_dec_128(type, exp, des) \
    do {                                        \
        patomic_msvc128_t arg = {0};            \
        arg.low = 1;                            \
        des = patomic_msvc128_sub(exp, arg);    \
    }                                           \
    while (0)


#endif  /* defined(_MSC_VER) */

#endif  /* PATOMIC_IMPL_MSVC_OPS_ARITHMETIC_DEC_H */