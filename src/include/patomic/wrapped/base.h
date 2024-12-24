/* Copyright (c) doodspav. */
/* SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception */

#ifndef PATOMIC_WRAPPED_BASE_H
#define PATOMIC_WRAPPED_BASE_H

#include <patomic/api/memory_order.h>
#include <patomic/api/transaction.h>

#include <patomic/internal/transaction.h>

#include <patomic/macros/ignore_unused.h>
#include <patomic/macros/static_assert.h>

#include <patomic/stdlib/assert.h>
#include <patomic/stdlib/stdalign.h>

#include <limits.h>
#include <stddef.h>
#include <string.h>


/**
 * @addtogroup wrapped.base
 *
 * @brief
 *   Hides a token from the compiler.
 */
#define HIDE(token)


/**
 * @addtogroup wrapped.base
 *
 * @brief
 *   Hides two comma-separated tokens from the compiler.
 *
 * @note
 *   Can be used for conditionally hiding a function parameter.
 */
#define HIDE_P(token_a, token_b)


/**
 * @addtogroup wrapped.base
 *
 * @brief
 *   Makes a token visible to the compiler.
 */
#define SHOW(token) token


/**
 * @addtogroup wrapped.base
 *
 * @brief
 *   Makes the second of two comma-separated tokens visible to the compiler,
 *   including the comma.
 *
 * @note
 *   Can be used for conditionally making a function parameter visible.
 */
#define SHOW_P(token_a, token_b) ,token_b


/**
 * @addtogroup wrapped.base
 *
 * @brief
 *   Asserts that an expression evaluates to a non-zero value.
 */
#define PATOMIC_WRAPPED_DO_ASSERT(expr) \
    patomic_assert_unreachable(expr)


/**
 * @addtogroup wrapped.base
 *
 * @brief
 *   Asserts that a pointed-to object meets a type's alignment requirements.
 */
#define PATOMIC_WRAPPED_DO_ASSERT_ALIGNED(obj_ptr, type)        \
    patomic_assert_unreachable(                                 \
        patomic_is_aligned(obj_ptr, patomic_alignof_type(type)) \
    )


/**
 * @addtogroup wrapped.base
 *
 * @brief
 *   Acts as a transparent wrapper around memcpy where the return value is
 *   discarded.
 *
 * @note
 *   Necessary because some compiler options may cause an error if the return
 *   value of memcpy is not used.
 */
#define PATOMIC_WRAPPED_DO_MEMCPY(dest, src, count) \
    PATOMIC_IGNORE_UNUSED(memcpy(dest, src, count))


/**
 * @addtogroup wrapped.base
 *
 * @brief
 *   Acts as memcpy would but in a transaction safe manner, and discards the
 *   return value.
 *
 * @note
 *   Necessary because on some platforms memcpy uses instructions which will
 *   cause a transaction to abort.
 */
#define PATOMIC_WRAPPED_DO_TSX_MEMCPY(dest, src, count)                                      \
    do {                                                                                     \
        volatile unsigned char *const uc_dest = (volatile unsigned char *) (dest);           \
        const volatile unsigned char *const uc_src = (const volatile unsigned char *) (src); \
        for (size_t i = 0; i < ((size_t) (count)); ++i)                                      \
        {                                                                                    \
            uc_dest[i] = uc_src[i];                                                          \
        }                                                                                    \
    }                                                                                        \
    while (0)


/**
 * @addtogroup wrapped.base
 *
 * @brief
 *   Acts as memcmp would but in a transaction safe manner, and discards the
 *   return value.
 *
 * @note
 *   Necessary because on some platforms memcmp uses instructions which will
 *   cause a transaction to abort.
 */
#define PATOMIC_WRAPPED_DO_TSX_MEMCMP(res, lhs, rhs, count) \
    do {                                                    \
        const volatile unsigned char *const uc_lhs =        \
            (const volatile unsigned char *) (lhs);         \
        const volatile unsigned char *const uc_rhs =        \
            (const volatile unsigned char *) (rhs);         \
        size_t i;                                           \
        (res) = 0;                                          \
        for (i = 0; i < ((size_t) (count)); ++i)            \
        {                                                   \
            const unsigned char a = uc_lhs[i];              \
            const unsigned char b = uc_rhs[i];              \
            if (a < b)                                      \
            {                                               \
                (res) = -1;                                 \
                break;                                      \
            }                                               \
            else if (a > b)                                 \
            {                                               \
                (res) = 1;                                  \
                break;                                      \
            }                                               \
        }                                                   \
    }                                                       \
    while (0)


/**
 * @addtogroup wrapped.base
 *
 * @brief
 *   Checks if either config.attempts or config.width is zero. If either are
 *   zero, sets result appropriately and either goes to the fallback label for
 *   config.attempts or goes to the cleanup label for config.width.
 */
#define PATOMIC_WRAPPED_TSX_CHECK_CONFIG_ZERO_WFB(                    \
    config, result, fallback, cleanup                                 \
)                                                                     \
    /* zero attempts */                                               \
    if ((config).attempts == 0ul)                                     \
    {                                                                 \
        (result).status = PATOMIC_INTERNAL_TRANSACTION_STATUS_CREATE( \
            patomic_TABORT_EXPLICIT, patomic_TINFO_ZERO_ATTEMPTS, 0   \
        );                                                            \
        (result).attempts_made = 0ul;                                 \
        goto fallback;                                                \
    }                                                                 \
    if ((config).width == (size_t) 0)                                 \
    {                                                                 \
        (result).status = PATOMIC_INTERNAL_TRANSACTION_STATUS_CREATE( \
            patomic_TSUCCESS, patomic_TINFO_NONE, 0                   \
        );                                                            \
        (result).attempts_made = 1ul;                                 \
        goto cleanup;                                                 \
    }                                                                 \
    /* require semicolon */                                           \
    do {} while (0)


/**
 * @addtogroup wrapped.base
 *
 * @brief
 *   Checks if either config.attempts or config.width is zero. If either are
 *   zero, sets result appropriately and goes to the cleanup label.
 */
#define PATOMIC_WRAPPED_TSX_CHECK_CONFIG_ZERO(config, result, cleanup) \
    PATOMIC_WRAPPED_TSX_CHECK_CONFIG_ZERO_WFB(config, result, cleanup, cleanup)


/**
 * @addtogroup wrapped.base
 *
 * @brief
 *   Checks if either config.fallback_attempts or config.width is zero. if
 *   either are zero, sets result appropriately and goes to the cleanup label.
 */
#define PATOMIC_WRAPPED_TSX_CHECK_CONFIG_ZERO_FALLBACK(                        \
    config, result, cleanup                                                    \
)                                                                              \
    /* zero attempts */                                                        \
    if ((config).fallback_attempts == 0ul)                                     \
    {                                                                          \
        (result).fallback_status = PATOMIC_INTERNAL_TRANSACTION_STATUS_CREATE( \
            patomic_TABORT_EXPLICIT, patomic_TINFO_ZERO_ATTEMPTS, 0            \
        );                                                                     \
        (result).fallback_attempts_made = 0ul;                                 \
        goto cleanup;                                                          \
    }                                                                          \
    if ((config).width == (size_t) 0)                                          \
    {                                                                          \
        (result).fallback_status = PATOMIC_INTERNAL_TRANSACTION_STATUS_CREATE( \
            patomic_TSUCCESS, patomic_TINFO_NONE, 0                            \
        );                                                                     \
        (result).fallback_attempts_made = 1ul;                                 \
        goto cleanup;                                                          \
    }                                                                          \
    /* require semicolon */                                                    \
    do {} while (0)


/**
 * @addtogroup wrapped.base
 *
 * @brief
 *   Update status to appropriate value if flag is set.
 */
#define PATOMIC_WRAPPED_TSX_UPDATE_STATUS_FOR_FLAG(            \
    status, flag_value                                         \
)                                                              \
    if ((flag_value) != 0)                                     \
    {                                                          \
        (status) = PATOMIC_INTERNAL_TRANSACTION_STATUS_CREATE( \
            patomic_TABORT_EXPLICIT, patomic_TINFO_FLAG_SET, 0 \
        );                                                     \
    }                                                          \
    /* require semicolon */                                    \
    do {} while (0)


#endif  /* PATOMIC_WRAPPED_BASE_H */
