#ifdef PATOMIC_STDLIB_ASSERT_H
    #undef PATOMIC_STDLIB_ASSERT_H
    #undef patomic_assert
    #undef patomic_assert_always
#endif
#define PATOMIC_STDLIB_ASSERT_H

#include <patomic/patomic_export.h>

#include <patomic/macros/func_name.h>
#include <patomic/macros/noinline.h>
#include <patomic/macros/noreturn.h>
#include <patomic/macros/unreachable.h>

PATOMIC_NO_EXPORT
PATOMIC_NOINLINE
PATOMIC_NORETURN
extern void
__patomic_assert_fail(
    const char *expr,
    const char *file,
    const char *func,
    unsigned int line
);

#define patomic_assert_always(expr) ((void)((expr) || \
    (__patomic_assert_fail(#expr, __FILE__, PATOMIC_FUNC_NAME, __LINE__), 0)))

#define patomic_assert(expr) ((void)((expr) || \
    (__patomic_assert_fail(#expr, __FILE__, PATOMIC_FUNC_NAME, __LINE__), 0)))

#define patomic_assert_unreachable(expr) ((void)((expr) || \
    (__patomic_assert_fail(#expr, __FILE__, PATOMIC_FUNC_NAME, __LINE__), 0)))

#if defined(NDEBUG) && !defined(NNDEBUG)
    #undef patomic_assert
    #undef patomic_assert_unreachable
    #define patomic_assert(expr) ((void) 0)
    #define patomic_assert_unreachable(expr) ((void)((expr) || \
        (PATOMIC_UNREACHABLE(), 0)))
#endif
