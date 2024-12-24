/* Copyright (c) doodspav. */
/* SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception */

#ifndef PATOMIC_API_COMBINE_H
#define PATOMIC_API_COMBINE_H

#include "core.h"

#include <patomic/api/export.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @addtogroup combine
 *
 * @brief
 *   Combines two atomic structs with implicit memory order operations. The
 *   first struct always takes priority, and only missing members are copied
 *   over from the second struct.
 *
 * @details
 *   Initially, the .ops member may be modified. If it is modified, the .align
 *   member may also be modified to meet the requirements for all the operations
 *   now present.
 *
 * @note
 *   It is advisable to sort structs by the .align member when combining more
 *   than two structs, in order to end up with the least restrictive values for
 *   the .align member.
 *
 * @param priority
 *   Struct which takes priority if both structs support an operation, and into
 *   which unsupported operations are added from the other struct.
 *
 * @param other
 *   Struct to combine into priority struct.
 */
PATOMIC_EXPORT void
patomic_combine(
    patomic_t *priority,
    const patomic_t *other
);


/**
 * @addtogroup combine
 *
 * @brief
 *   Combines two atomic structs with explicit memory order operations. The
 *   first struct always takes priority, and only missing members are copied
 *   over from the second struct.
 *
 * @details
 *   Initially, the .ops member may be modified. If it is modified, the .align
 *   member may also be modified to meet the requirements for all the operations
 *   now present.
 *
 * @note
 *   It is advisable to sort structs by the .align member when combining more
 *   than two structs, in order to end up with the least restrictive values for
 *   the .align member.
 *
 * @param priority
 *   Struct which takes priority if both structs support an operation, and into
 *   which unsupported operations are added from the other struct.
 *
 * @param other
 *   Struct to combine into priority struct.
 */
PATOMIC_EXPORT void
patomic_combine_explicit(
    patomic_explicit_t *priority,
    const patomic_explicit_t *other
);


#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* PATOMIC_API_COMBINE_H */
