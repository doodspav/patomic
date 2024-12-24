/* Copyright (c) doodspav. */
/* SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception */

#ifndef PATOMIC_INTERNAL_ALIGN_H
#define PATOMIC_INTERNAL_ALIGN_H

#include <patomic/api/core.h>


/**
 * @addtogroup internal
 *
 * @brief
 *   Compares two patomic_align_t objects.
 *
 * @details
 *   Priority is given to "recommended", then "minimum", then "size_within".
 *
 * @return
 *   Returns -1 if lhs is a looser requirement than rhs, 1 if lhs is a stricter
 *   requirement than rhs, otherwise 0.
 */
int
patomic_internal_compare_align(
    patomic_align_t lhs,
    patomic_align_t rhs
);


#endif  /* PATOMIC_INTERNAL_ALIGN_H */
