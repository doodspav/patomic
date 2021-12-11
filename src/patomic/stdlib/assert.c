#include <stdio.h>
#include <stdlib.h>

#include <patomic/stdlib/assert.h>

#include <patomic/macros/ignore_unused.h>
#include <patomic/macros/unreachable.h>


#undef PATOMIC_ASSERT_WIDE
#ifdef __STDC_VERSION__
    #if __STDC_VERSION__ >= 199409L
        #define PATOMIC_ASSERT_WIDE 1
    #endif
#endif
#ifndef PATOMIC_ASSERT_WIDE
    #define PATOMIC_ASSERT_WIDE 0
#endif


#if PATOMIC_ASSERT_WIDE
    #include <wchar.h>
#endif


static void
patomic_assert_fxprint(
    FILE *stream,
    const char *expr,
    const char *file,
    const char *func,
    unsigned int line
)
{
#if PATOMIC_ASSERT_WIDE
    if (fwide(stream, -1) > 0)
    {
        PATOMIC_IGNORE_UNUSED(fwprintf(
            stream, L"%s:%d: %s: Assertion `%s` failed.\n",
            file, line, func, expr
        ));
    }
    else
    {
#endif
        PATOMIC_IGNORE_UNUSED(fprintf(
            stream, "%s:%d: %s: Assertion `%s` failed.\n",
            file, line, func, expr
        ));
#if PATOMIC_ASSERT_WIDE
    }
#endif
}


void
__patomic_assert_fail(
    const char *expr,
    const char *file,
    const char *func,
    unsigned int line
)
{
    int _;
    _ = setvbuf(stderr, NULL, _IONBF, 0);
    PATOMIC_IGNORE_UNUSED(_);
    patomic_assert_fxprint(stderr, expr, file, func, line);
    _ = fflush(stderr);
    PATOMIC_IGNORE_UNUSED(_);
    abort();
#if !PATOMIC_UNREACHABLE_IS_VCZ
    PATOMIC_UNREACHABLE();
#endif
}
