#ifndef PATOMIC_WRAPPED_BASE_H
#define PATOMIC_WRAPPED_BASE_H

#include <patomic/macros/ignore_unused.h>

#include <patomic/stdlib/assert.h>
#include <patomic/stdlib/stdalign.h>

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


#endif  /* PATOMIC_WRAPPED_BASE_H */
