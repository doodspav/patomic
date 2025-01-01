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


#endif  /* defined(_MSC_VER) */

#endif  /* PATOMIC_IMPL_MSVC_OPS_H */
