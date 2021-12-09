#ifdef PATOMIC_STDLIB_ASSERT_H
    #undef PATOMIC_STDLIB_ASSERT_H
    #undef patomic_assert
    #undef patomic_assert_always
#endif
#define PATOMIC_STDLIB_ASSERT_H

#include <patomic/patomic_export.h>

#include <patomic/macros/noinline.h>
#include <patomic/macros/noreturn.h>

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

#define patomic_assert_always(expr) (void)((expr) || \
    (__patomic_assert_fail(#expr, __FILE__, "", __LINE__), 0))

#define patomic_assert(expr) (void)((expr) || \
    (__patomic_assert_fail(#expr, __FILE__, "", __LINE__), 0))

#if defined(NDEBUG) && !defined(NNDEBUG)
    #undef patomic_assert
    #define patomic_assert(expr) ((void) 0)
#endif
