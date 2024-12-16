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


/* redefined below */
#undef HIDE
#undef HIDE_P
#undef SHOW
#undef SHOW_P
#undef PATOMIC_WRAPPED_DO_ASSERT
#undef PATOMIC_WRAPPED_DO_ASSERT_ALIGNED
#undef PATOMIC_WRAPPED_DO_MEMCPY


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
            patomic_TABORT_EXPLICIT, 0, 0                             \
        );                                                            \
        (result).attempts_made = 0ul;                                 \
        goto fallback;                                                \
    }                                                                 \
    if ((config).width == (size_t) 0)                                 \
    {                                                                 \
        (result).status = PATOMIC_INTERNAL_TRANSACTION_STATUS_CREATE( \
            patomic_TSUCCESS, 0, 0                                    \
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
            patomic_TABORT_EXPLICIT, 0, 0                                      \
        );                                                                     \
        (result).fallback_attempts_made = 0ul;                                 \
        goto cleanup;                                                          \
    }                                                                          \
    if ((config).width == (size_t) 0)                                          \
    {                                                                          \
        (result).fallback_status = PATOMIC_INTERNAL_TRANSACTION_STATUS_CREATE( \
            patomic_TSUCCESS, 0, 0                                             \
        );                                                                     \
        (result).fallback_attempts_made = 1ul;                                 \
        goto cleanup;                                                          \
    }                                                                          \
    /* require semicolon */                                                    \
    do {} while (0)


#endif  /* PATOMIC_WRAPPED_BASE_H */
