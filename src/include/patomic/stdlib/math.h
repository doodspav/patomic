/* Copyright (c) doodspav. */
/* SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception */

#ifndef PATOMIC_STDLIB_MATH_H
#define PATOMIC_STDLIB_MATH_H


/**
 * @addtogroup stdlib
 *
 * @brief
 *   Checks if 'x' is 0 or a power of 2.
 *
 * @param x
 *   An object of unsigned integer type holding a non-negative value.
 *
 * @return
 *   The value 1 if 'x' is 0 or a power of 2, otherwise the value 0.
 */
#define patomic_unsigned_is_pow2_or_zero(x) \
    (((x) & ((x) - 1u)) == 0)


/**
 * @addtogroup stdlib
 *
 * @brief
 *   Checks if 'x' is a power of 2.
 *
 * @param x
 *   An object of unsigned integer type holding a non-negative value.
 *
 * @return
 *   The value 1 if 'x' is a power of 2, otherwise the value 0.
 */
#define patomic_unsigned_is_pow2(x) \
    (((x) != 0) && patomic_unsigned_is_pow2_or_zero(x))


/**
 * @addtogroup stdlib
 *
 * @brief
 *   Computes 'x % y' assuming that 'y' is a power of 2.
 *
 * @param x
 *   An object of unsigned integer type holding a non-negative value.
 *
 * @param y
 *   An object of unsigned integer type holding a positive power of 2 value.
 *
 * @return
 *   The non-negative value 'x % y' of unsigned integer type.
 */
#define patomic_unsigned_mod_pow2(x, y) \
    ((x) & ((y) - 1u))


/**
 * @addtogroup stdlib
 *
 * @brief
 *   Computes 'x % y' with no assumptions about their values.
 *
 * @param x
 *   An object of unsigned integer type holding a non-negative value.
 *
 * @param y
 *   An object of unsigned integer type holding a positive non-zero value.
 *
 * @return
 *   A non-negative value 'x % y' of unsigned integer type.
 */
#define patomic_unsigned_mod(x, y)         \
    (patomic_unsigned_is_pow2_or_zero(y) ? \
     patomic_unsigned_mod_pow2(x, y)     : \
     ((x) % (y + 0u)))


#endif  /* PATOMIC_STDLIB_MATH_H */
