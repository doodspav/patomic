/* Copyright (c) doodspav. */
/* SPDX-License-Identifier: LGPL-3.0-only WITH LGPL-3.0-linking-exception */

#ifndef PATOMIC_STDLIB_STDALIGN_H
#define PATOMIC_STDLIB_STDALIGN_H

#include <patomic/config.h>

#include <stddef.h>


/**
 * @addtogroup stdlib
 *
 * @brief
 *   Calculates the alignment of an imaginary struct which contains the largest
 *   possible single object with natural alignment.
 *
 * @warning
 *   This will return incorrect values if the actual struct contains members of
 *   over-aligned types, which may result in undefined behaviour if used.
 *
 * @param size
 *   The size of an object or type as returned by sizeof.
 *
 * @return
 *   The largest power of 2 which divides 'size', or the value 0 if 'size' is 0.
 */
size_t
patomic_alignment_from_size(
    size_t size
);


/**
 * @addtogroup stdlib
 *
 * @brief
 *   Checks if the address has a suitable runtime alignment.
 *
 * @param ptr
 *   Pointer holding the address to check.
 *
 * @param alignment
 *   Minimum alignment to check for.
 *
 * @return
 *   The value 1 if the address held by 'ptr' is suitably aligned, otherwise
 *   the value 0.
 */
int
patomic_is_aligned(
    const volatile void *ptr,
    size_t alignment
);


/* used internally */
#undef patomic_alignof_type_

#if PATOMIC_HAS_C23_ALIGNOF
    #define patomic_alignof_type_(type) alignof(type)
#elif PATOMIC_HAS_C23_ALIGNOF_EXTN
    #define patomic_alignof_type_(type) __extension__ alignof(type)
#elif PATOMIC_HAS_C11_ALIGNOF
    #define patomic_alignof_type_(type) _Alignof(type)
#elif PATOMIC_HAS_C11_ALIGNOF_EXTN
    #define patomic_alignof_type_(type) __extension__ _Alignof(type)
#elif PATOMIC_HAS_GNU_ALIGNOF
    #define patomic_alignof_type_(type) __alignof__(type)
#elif PATOMIC_HAS_GNU_ALIGNOF_EXTN
    #define patomic_alignof_type_(type) __extension__ __alignof__(type)
#elif PATOMIC_HAS_MS_ALIGNOF
    #define patomic_alignof_type_(type) __alignof(type)
#elif PATOMIC_HAS_MS_ALIGNOF_EXTN
    #define patomic_alignof_type_(type) __extension__ __alignof__(type)
#else
    #define patomic_alignof_type_(type) \
        patomic_alignment_from_size(sizeof(type))
#endif

/**
 * @addtogroup stdlib
 *
 * @brief
 *   Gets the alignment requirement of a type.
 *
 * @warning
 *   The implementation for this function may fall back to using
 *   'patomic_alignment_from_size' if no other implementation is available.
 *   This will return incorrect values for over-aligned types, which may
 *   result in undefined behaviour if used.
 *
 * @param type
 *   Type from which to get alignment.
 *
 * @return
 *   The alignment of the type 'type'.
 */
#define patomic_alignof_type(type) patomic_alignof_type_(type)


#endif  /* PATOMIC_STDLIB_STDALIGN_H */
