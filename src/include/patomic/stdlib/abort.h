/* Copyright (c) doodspav. */
/* SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception */

#ifndef PATOMIC_STDLIB_ABORT_H
#define PATOMIC_STDLIB_ABORT_H

#include <patomic/macros/noreturn.h>


/**
 * @addtogroup stdlib
 *
 * @brief
 *   Calls abort().
 *
 * @note
 *   Exists so that we can mock this in unit tests with functionality which
 *   does not terminate execution.
 */
PATOMIC_NORETURN void
patomic_abort(void);


#endif  /* PATOMIC_STDLIB_ABORT_H */
