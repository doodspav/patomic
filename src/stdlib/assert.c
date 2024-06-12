#include <patomic/stdlib/assert.h>

#include <patomic/macros/ignore_unused.h>
#include <patomic/macros/unreachable.h>

#include <patomic/patomic_config.h>

#include <stdio.h>
#include <stdlib.h>


/* check if we can make use of wide functionality */
#define PATOMIC_ASSERT_WIDE \
    PATOMIC_HAS_WCHAR_FWIDE && PATOMIC_HAS_WCHAR_FWPRINTF
#if PATOMIC_ASSERT_WIDE
    #include <wchar.h>
#endif


static void
patomic_assert_fxprint(
    FILE *stream,
    const char *expr,
    const char *file,
    const char *func,
    const unsigned int line
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
    const unsigned int line
)
{
    int _;
    _ = setvbuf(stderr, NULL, _IONBF, 0);
    PATOMIC_IGNORE_UNUSED(_);
    patomic_assert_fxprint(stderr, expr, file, func, line);
    _ = fflush(stderr);
    PATOMIC_IGNORE_UNUSED(_);
    abort();

    /* if unreachable is vcz ((void) 0), we may get a compiler warning for
     * unreachable code */
#if !PATOMIC_UNREACHABLE_IS_VCZ
    PATOMIC_UNREACHABLE();
#endif
}
