/* Copyright (c) doodspav. */
/* SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception */

#include <patomic/stdlib/abort.h>

#include <stdlib.h>


void
patomic_abort(void)
{
    abort();
}
