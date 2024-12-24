/* Copyright (c) doodspav. */
/* SPDX-License-Identifier: LGPL-3.0-only WITH LGPL-3.0-linking-exception */

#ifndef PATOMIC_INTERNAL_COMBINE_H
#define PATOMIC_INTERNAL_COMBINE_H

#include <patomic/api/core.h>


/**
 * @addtogroup internal
 *
 * @brief
 *   Internal implementation of patomic_combine which is not exported.
 *
 * @note
 *   Called internally instead of exported version to avoid PLT indirection
 *   when building as a shared library.
 */
void
patomic_internal_combine(
    patomic_t *priority,
    const patomic_t *other
);


/**
 * @addtogroup internal
 *
 * @brief
 *   Internal implementation of patomic_combine_explicit which is not exported.
 *
 * @note
 *   Called internally instead of exported version to avoid PLT indirection
 *   when building as a shared library.
 */
void
patomic_internal_combine_explicit(
    patomic_explicit_t *priority,
    const patomic_explicit_t *other
);


#endif  /* PATOMIC_INTERNAL_COMBINE_H */
