/* Copyright (c) doodspav. */
/* SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception */

#ifndef PATOMIC_STDLIB_SORT_H
#define PATOMIC_STDLIB_SORT_H

#include <stdlib.h>


/**
 * @addtogroup stdlib
 *
 * @brief
 *   Sorts the given array pointed to by "ptr" in ascending order. The array
 *   contains "count" elements of "size" bytes. Function pointed to by "comp"
 *   is used for object comparison.
 *
 * @param ptr
 *   Pointer to the array to sort.
 *
 * @param count
 *   Number of elements in the array.
 *
 * @param size
 *   Size of each element in the array in bytes.
 *
 * @param comp
 *   Comparison function which returns a negative integer value if the first
 *   argument is less than the second, a positive integer value if the first
 *   argument is greater than the second and zero if the arguments are
 *   equivalent.
 *
 * @note
 *   The interface is identical to C90's qsort.
 */
void
patomic_array_sort(
    void *ptr,
    size_t count,
    size_t size,
    int (*comp)(const void*, const void*)
);


#endif  /* PATOMIC_STDLIB_SORT_H */
