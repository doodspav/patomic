/* Copyright (c) doodspav. */
/* SPDX-License-Identifier: LGPL-3.0-only WITH LGPL-3.0-linking-exception */

#include <patomic/stdlib/sort.h>


void
patomic_array_sort(
    void *const ptr,
    const size_t count,
    const size_t size,
    int (*const comp)(const void*, const void*)
)
{
    qsort(ptr, count, size, comp);
}
