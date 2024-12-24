/* Copyright (c) doodspav. */
/* SPDX-License-Identifier: LGPL-3.0-only WITH LGPL-3.0-linking-exception */

#ifndef PATOMIC_IGNORE_UNUSED

/**
* @addtogroup macros
 *
 * @brief
 *   Suppresses compiler warnings on unused expressions.
*/
#define PATOMIC_IGNORE_UNUSED(expr) ((void) (expr))

#endif  /* PATOMIC_IGNORE_UNUSED */
