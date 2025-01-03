/* Copyright (c) doodspav. */
/* SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception */

#ifndef PATOMIC_STATIC_ASSERT

/**
 * @addtogroup macros
 *
 * @brief
 *   Asserts that an expression evaluates to a non-zero value at compile time.
 *
 * @pre
 *   The message must be able to be part of a valid C identifier (but will
 *   never be at the start).
 */
#define PATOMIC_STATIC_ASSERT(msg, expr) \
    typedef char patomic_static_assert_##msg[(expr) ? 1 : -1]

#endif  /* PATOMIC_STATIC_ASSERT */
