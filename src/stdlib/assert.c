#ifdef _MSC_VER
    #define _CRT_SECURE_NO_WARNINGS
#endif


#include <patomic/stdlib/assert.h>

#include <patomic/macros/ignore_unused.h>
#include <patomic/macros/unreachable.h>

#include <patomic/config.h>

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
    /* print to a wide stream if conversion to narrow isn't possible */
    if (fwide(stream, -1) > 0)
    {
        /* print assertion */
        const int _ = fwprintf(
#ifdef _MSC_VER
            /* msvc uses %S for narrow strings in wide functions */
            stream, L"%S:%u: %S: Assertion `%S` failed.\n",
#else
            /* standard treats %s as normal even in wide functions */
            stream, L"%s:%u: %s: Assertion `%s` failed.\n",
#endif
            file, line, func, expr
        );
        PATOMIC_IGNORE_UNUSED(_);
    }
    /* print to a narrow stream */
    else
    {
#endif
        /* print assertion */
        PATOMIC_IGNORE_UNUSED(fprintf(
            stream, "%s:%u: %s: Assertion `%s` failed.\n",
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
    /* attempt to make stderr unbuffered */
    int _;
    _ = setvbuf(stderr, NULL, _IONBF, 0);
    PATOMIC_IGNORE_UNUSED(_);

    /* print assertion to stderr */
    patomic_assert_fxprint(stderr, expr, file, func, line);

    /* flush stderr in case it is still buffered */
    _ = fflush(stderr);
    PATOMIC_IGNORE_UNUSED(_);

    /* failed assertions should not return */
    abort();

    /* if unreachable is vcz ((void) 0), we may get a compiler warning for
     * unreachable code */
#if !PATOMIC_UNREACHABLE_IS_VCZ
    PATOMIC_UNREACHABLE();
#endif
}
