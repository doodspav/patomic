/* Copyright (c) doodspav. */
/* SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception */

#ifndef PATOMIC_IMPL_MSVC_OPS_ARITHMETIC_INC_H
#define PATOMIC_IMPL_MSVC_OPS_ARITHMETIC_INC_H

#define _MSC_VER 2000
#define _M_ARM64
#define __int64 long long
#include "../xchg/cmpxchg.h"
#include "add.h"

#ifdef _MSC_VER

#include "../base.h"

#include <patomic/macros/ignore_unused.h>

#include <patomic/wrapped/cmpxchg.h>
#include <patomic/wrapped/direct.h>

#include <patomic/api/ops.h>


#define do_fetch_inc_raw_explicit_n(n, type, obj, order, res)       \
    do {                                                            \
        switch (order)                                              \
        {                                                           \
            case patomic_RELAXED:                                   \
      VIS_IL_NF((res = _InterlockedIncrement##n##_nf(obj));)        \
      VIS_IL_NF(break;)                                             \
            case patomic_CONSUME:                                   \
            case patomic_ACQUIRE:                                   \
     VIS_IL_ACQ((res = _InterlockedIncrement##n##_acq(obj));)       \
     VIS_IL_ACQ(break;)                                             \
            case patomic_RELEASE:                                   \
     VIS_IL_REL((res = _InterlockedIncrement##n##_rel(obj));)       \
     VIS_IL_REL(break;)                                             \
            case patomic_ACQ_REL:                                   \
            case patomic_SEQ_CST:                                   \
            default:                                                \
                res = _InterlockedIncrement##n(obj);                \
        }                                                           \
        res = (type) (((unsigned type) res) - ((unsigned type) 1)); \
    }                                                               \
    while (0)

#define do_void_inc_raw_explicit_n(n, type, obj, order)        \
    do {                                                       \
        type res;                                              \
        do_fetch_inc_raw_explicit_n(n, type, obj, order, res); \
        PATOMIC_IGNORE_UNUSED(res);                            \
    }                                                          \
    while (0)

#define do_fetch_inc_add_explicit_n(n, type, obj, order, res)   \
    do {                                                        \
        const type arg = (type) 1;                              \
        do_fetch_add_explicit_n(n, type, obj, arg, order, res); \
    }                                                           \
    while (0)

#define do_void_inc_add_explicit_n(n, type, obj, order)        \
    do {                                                       \
        type res;                                              \
        do_fetch_inc_add_explicit_n(n, type, obj, order, res); \
        PATOMIC_IGNORE_UNUSED(res);                            \
    }                                                          \
    while (0)

#define do_make_desired_inc(type, exp, des)                         \
    do {                                                            \
        des = (type) (((unsigned type) exp) + ((unsigned type) 1)); \
    }                                                               \
    while (0)

#define do_make_desired_inc_128(type, exp, des) \
    do {                                        \
        patomic_msvc128_t arg = {0};            \
        arg.low = 1;                            \
        des = patomic_msvc128_add(exp, arg);    \
    }                                           \
    while (0)


#endif  /* defined(_MSC_VER) */

#endif  /* PATOMIC_IMPL_MSVC_OPS_ARITHMETIC_INC_H */
