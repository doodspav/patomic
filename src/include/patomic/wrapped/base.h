#ifndef PATOMIC_WRAPPED_BASE_H
#define PATOMIC_WRAPPED_BASE_H

#include <patomic/macros/ignore_unused.h>

#include <patomic/stdlib/assert.h>
#include <patomic/stdlib/stdalign.h>
#include <patomic/stdlib/string.h>


/* redefined below */
#undef HIDE
#undef HIDE_P
#undef SHOW
#undef SHOW_P
#undef PATOMIC_WRAPPED_DO_ASSERT
#undef PATOMIC_WRAPPED_DO_ASSERT_ALIGNED
#undef PATOMIC_WRAPPED_DO_MEMCPY


/**
 * @addtogroup wrapped
 *
 * @brief
 *   Hides a token from the compiler.
 */
#define HIDE(token)


/**
 * @addtogroup wrapped
 *
 * @brief
 *   Hides two comma-separated tokens from the compiler.
 *
 * @note
 *   Can be used for conditionally hiding a function parameter.
 */
#define HIDE_P(tokenA, tokenB)


/**
 * @addtogroup wrapped
 *
 * @brief
 *   Makes a token visible to the compiler.
 */
#define SHOW(token) token


/**
 * @addtogroup wrapped
 *
 * @brief
 *   Makes the second of two comma-separated tokens visible to the compiler,
 *   including the comma.
 *
 * @note
 *   Can be used for conditionally making a function parameter visible.
 */
#define SHOW_P(tokenA, tokenB) ,tokenB


/**
 * @addtogroup wrapped
 *
 * @brief
 *   Asserts that an expression evaluates to a non-zero value.
 */
#define PATOMIC_WRAPPED_DO_ASSERT(expr) \
    patomic_assert_unreachable(expr)


/**
 * @addtogroup wrapped
 *
 * @brief
 *   Asserts that an object meets a types alignment requirements.
 */
#define PATOMIC_WRAPPED_DO_ASSERT_ALIGNED(obj, type)        \
    patomic_assert_unreachable(                             \
        patomic_is_aligned(obj, patomic_alignof_type(type)) \
    )


/**
 * @addtogroup wrapped
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
    PATOMIC_IGNORE_UNUSED(patomic_memcpy(dest, src, count))
///

#endif  /* PATOMIC_WRAPPED_BASE_H */
