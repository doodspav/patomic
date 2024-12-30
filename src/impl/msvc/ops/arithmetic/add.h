/* Copyright (c) doodspav. */
/* SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception */

#ifndef PATOMIC_IMPL_MSVC_OPS_ARITHMETIC_ADD_H
#define PATOMIC_IMPL_MSVC_OPS_ARITHMETIC_ADD_H

#ifdef _MSC_VER

#include "../base.h"

#include <patomic/macros/ignore_unused.h>

#include <patomic/wrapped/cmpxchg.h>
#include <patomic/wrapped/direct.h>

#include <patomic/api/ops.h>


#define do_fetch_add_explicit_n(n, type, obj, arg, order, res)       \
    do {                                                             \
        switch (order)                                               \
        {                                                            \
            case patomic_RELAXED:                                    \
      VIS_IL_NF((res = _InterlockedExchangeAdd##n##_nf(obj, arg));)  \
      VIS_IL_NF(break;)                                              \
            case patomic_CONSUME:                                    \
            case patomic_ACQUIRE:                                    \
     VIS_IL_ACQ((res = _InterlockedExchangeAdd##n##_acq(obj, arg));) \
     VIS_IL_ACQ(break;)                                              \
            case patomic_RELEASE:                                    \
     VIS_IL_REL((res = _InterlockedExchangeAdd##n##_rel(obj, arg));) \
     VIS_IL_REL(break;)                                              \
            case patomic_ACQ_REL:                                    \
            case patomic_SEQ_CST:                                    \
            default:                                                 \
                res = _InterlockedExchangeAdd##n(obj, arg);          \
        }                                                            \
    }                                                                \
    while (0)

#define do_void_add_explicit_n(n, type, obj, arg, order)        \
    do {                                                        \
        type res;                                               \
        do_fetch_add_explicit_n(n, type, obj, arg, order, res); \
        PATOMIC_IGNORE_UNUSED(res);                             \
    }                                                           \
    while (0)

#define do_make_desired_add(type, exp, arg, des)                      \
    do {                                                              \
        des = (type) (((unsigned type) exp) + ((unsigned type) arg)); \
    }                                                                 \
    while (0)

#define do_make_desired_add_128(type, exp, arg, des) \
    do {                                             \
        des = patomic_msvc128_add(exp, arg);         \
    }                                                \
    while (0)


#endif  /* defined(_MSC_VER) */

#endif  /* PATOMIC_IMPL_MSVC_OPS_ARITHMETIC_ADD_H */
