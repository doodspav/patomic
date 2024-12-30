/* Copyright (c) doodspav. */
/* SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception */

#ifndef PATOMIC_IMPL_MSVC_OPS_BASE_MACROS_H
#define PATOMIC_IMPL_MSVC_OPS_BASE_MACROS_H

#include "config.h"
#include "m128.h"


#if PATOMIC_IMPL_MSVC_HAS_IL_ACQ_REL
    #define VIS_IL_ACQ(expr) expr
    #define VIS_IL_REL(expr) expr
#else
    #define VIS_IL_ACQ(expr)
    #define VIS_IL_REL(expr)
#endif


#if PATOMIC_IMPL_MSVC_HAS_IL_NF
    #define VIS_IL_NF(expr) expr
#else
    #define VIS_IL_NF(expr)
#endif


#define CMP_EQ(a, b) \
    ((a) == (b))

#define CMP_EQ_128(a, b) \
    ((a).low == (b).low && (a).high == (b).high)


#endif  /* PATOMIC_IMPL_MSVC_OPS_BASE_MACROS_H */
