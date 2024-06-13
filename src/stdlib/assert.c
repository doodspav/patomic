#ifdef _MSC_VER
    #define _CRT_SECURE_NO_WARNINGS
#endif


#include <patomic/stdlib/assert.h>

#include <patomic/macros/ignore_unused.h>
#include <patomic/macros/unreachable.h>

#include <patomic/patomic_config.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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
    /* possibly unused wide declarations */
    wchar_t *expr_wide;
    wchar_t *file_wide;
    wchar_t *func_wide;

    /* possibly unused sizes */
    const size_t expr_size = strlen(expr) + 1;
    const size_t file_size = strlen(file) + 1;
    const size_t func_size = strlen(func) + 1;

    /* print to a wide stream if conversion to narrow isn't possible */
    if (fwide(stream, -1) > 0)
    {
        /* allocate wide strings */
        /* assume that a wide string will never be longer than a narrow one */
        expr_wide = malloc(sizeof(wchar_t) * expr_size);
        file_wide = malloc(sizeof(wchar_t) * file_size);
        func_wide = malloc(sizeof(wchar_t) * func_size);

        /* convert narrow strings to wide strings */
        PATOMIC_IGNORE_UNUSED(mbstowcs(expr_wide, expr, expr_size));
        PATOMIC_IGNORE_UNUSED(mbstowcs(file_wide, file, file_size));
        PATOMIC_IGNORE_UNUSED(mbstowcs(func_wide, func, func_size));

        /* print assertion */
        PATOMIC_IGNORE_UNUSED(fwprintf(
            stream, L"%s:%d: %s: Assertion `%s` failed.\n",
            file_wide, line, func_wide, expr_wide
        ));

        /* free wide strings */
        free(expr_wide);
        free(file_wide);
        free(func_wide);
    }
    /* print to a narrow stream */
    else
    {
#endif
        /* wide declarations are unused */
        PATOMIC_IGNORE_UNUSED(expr_wide);
        PATOMIC_IGNORE_UNUSED(file_wide);
        PATOMIC_IGNORE_UNUSED(func_wide);

        /* size values are unused */
        PATOMIC_IGNORE_UNUSED(expr_size);
        PATOMIC_IGNORE_UNUSED(file_size);
        PATOMIC_IGNORE_UNUSED(func_size);

        /* print assertion */
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
