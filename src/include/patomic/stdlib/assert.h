/* Copyright (c) doodspav. */
/* SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception */

#ifdef PATOMIC_STDLIB_ASSERT_H
    #undef PATOMIC_STDLIB_ASSERT_H
    #undef patomic_assert
    #undef patomic_assert_always
    #undef patomic_assert_unreachable
#endif
#define PATOMIC_STDLIB_ASSERT_H

#include <patomic/macros/func_name.h>
#include <patomic/macros/ignore_unused.h>
#include <patomic/macros/noreturn.h>
#include <patomic/macros/unreachable.h>


PATOMIC_NORETURN void
__patomic_assert_fail(
    const char *expr,
    const char *file,
    const char *func,
    unsigned int line
);


/**
 * @addtogroup stdlib
 *
 * @brief
 *   Will always fatally assert if the expression evaluates as false, even if
 *   NDEBUG is defined.
 *
 * @param expr
 *   Runtime expression to evaluate as a boolean value.
 */
#define patomic_assert_always(expr) (PATOMIC_IGNORE_UNUSED((expr) || \
    (__patomic_assert_fail(#expr, __FILE__, PATOMIC_FUNC_NAME, __LINE__), 0)))


#if defined(NDEBUG) && !defined(NNDEBUG)
    #define patomic_assert_(expr) (PATOMIC_IGNORE_UNUSED(0))
#else
    #define patomic_assert_(expr) patomic_assert_always(expr)
#endif

/**
 * @addtogroup stdlib
 *
 * @brief
 *   Will fatally assert if the expression evaluates as false. If NDEBUG is
 *   defined and NNDEBUG is not defined, no evaluation will take place and this
 *   is a no-op.
 *
 * @param expr
 *   Runtime expression to evaluate as a boolean value.
 */
#define patomic_assert(expr) patomic_assert_(expr)


#if defined(NDEBUG) && !defined(NNDEBUG)
    #define patomic_assert_unreachable_(expr) (PATOMIC_IGNORE_UNUSED((expr || \
        (PATOMIC_UNREACHABLE(), 0))))
#else
    #define patomic_assert_unreachable_(expr) patomic_assert_always(expr)
#endif

/**
 * @addtogroup stdlib
 *
 * @brief
 *   Will fatally assert if the expression evaluates as false. If NDEBUG is
 *   defined and NNDEBUG is not defined, the expression is instead marked as
 *   unreachable.
 *
 * @param expr
 *   Runtime expression to evaluate as a boolean value.
 */
#define patomic_assert_unreachable(expr) patomic_assert_unreachable_(expr)
