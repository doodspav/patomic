/* Copyright (c) doodspav. */
/* SPDX-License-Identifier: LGPL-3.0-only WITH LGPL-3.0-linking-exception */

#ifndef PATOMIC_UNREACHABLE

#include <patomic/config.h>

/* used internally */
#undef PATOMIC_UNREACHABLE_
#undef PATOMIC_UNREACHABLE_IS_VCZ_

/* availability flag */
#undef PATOMIC_UNREACHABLE_IS_VCZ

#if PATOMIC_HAS_MS_ASSUME
    #define PATOMIC_UNREACHABLE_() __assume(0)
    #define PATOMIC_UNREACHABLE_IS_VCZ_ 0
#elif PATOMIC_HAS_BUILTIN_UNREACHABLE
    #define PATOMIC_UNREACHABLE_() __builtin_unreachable()
    #define PATOMIC_UNREACHABLE_IS_VCZ_ 0
#else
    #define PATOMIC_UNREACHABLE_() ((void) 0)
    #define PATOMIC_UNREACHABLE_IS_VCZ_ 1
#endif

/**
 * @addtogroup macros
 *
 * @brief
 *   The expression informs the compiler that this code path is unreachable.
 *   Invokes undefined behaviour.
 */
#define PATOMIC_UNREACHABLE() PATOMIC_UNREACHABLE_()

/**
 * @addtogroup macros
 *
 * @brief
 *   Boolean flag is set if PATOMIC_UNREACHABLE has no available implementation
 *   and is implemented as a void cast zero ((void) 0).
 */
#define PATOMIC_UNREACHABLE_IS_VCZ PATOMIC_UNREACHABLE_IS_VCZ_

#endif  /* PATOMIC_UNREACHABLE */
