#ifndef PATOMIC_PATOMIC_CONFIG_H
#define PATOMIC_PATOMIC_CONFIG_H


/*
 * The following macros should never be modified unless cross-compiling for a
 * target platform which would not define them to have the same value as the
 * host platform
 *
 * Changing these macros incorrectly WILL result in a library which exhibits
 * undefined behaviour and will likely function incorrectly.
 *
 * For this reason, they cannot be overridden via compiler flags; they can only
 * be explicitly modified in this file.
 */

#include <limits.h>

/* integer representation is two's complement */
#undef PATOMIC_HAVE_TWOS_COMPL
#define PATOMIC_HAVE_TWOS_COMPL (-INT_MAX != INT_MIN)


/*
 * The following macros may be modified and overridden safely. At worst, this
 * may cause a compile-time warning/error or limit runtime functionality
 * exposed by the library.
 *
 * The following macros are defined in '_patomic_config.h' which is generated
 * by CMake at build time. If you do not use CMake to build this project,
 * comment out the '#include' and modify the macros manually (or set them via
 * compiler flags).
 *
 * The 'requires:' comments are to be taken as helpful hints when modifying or
 * overriding these macros.
 */

#include <patomic/_patomic_config.h>

/* 'long long' is a fundamental type */
/* requires: C99 */
#ifndef PATOMIC_HAVE_LONG_LONG
    #define PATOMIC_HAVE_LONG_LONG 0
#endif

/* '__func__' identifier is available */
/* requires: C99 */
#ifndef PATOMIC_HAVE_FUNC
    #define PATOMIC_HAVE_FUNC 0
#endif

/* '__FUNCTION__' identifier or macro is available */
/* requires: most compilers will have this, including MS and GNU */
/* note: GCC 5+ doesn't allow this with -Wpedantic (Clang does) */
#ifndef PATOMIC_HAVE_GNU_FUNCTION
    #define PATOMIC_HAVE_GNU_FUNCTION 0
#endif

/* '__extension__ __FUNCTION__' identifier is available */
/* requires: GNU compatible(-ish) compiler */
#ifndef PATOMIC_HAVE_GNU_FUNCTION_EXTN
    #define PATOMIC_HAVE_GNU_FUNCTION_EXTN 0
#endif

/* '__PRETTY_FUNCTION__' identifier is available */
/* requires: GNU compatible(-ish) compiler */
/* note: GCC 5+ doesn't allow this with -Wpedantic (Clang does) */
#ifndef PATOMIC_HAVE_GNU_PRETTY_FUNCTION
    #define PATOMIC_HAVE_GNU_PRETTY_FUNCTION 0
#endif

/* '__extension__ __PRETTY_FUNCTION__' identifier is available */
/* requires: GNU compatible(-ish) compiler */
#ifndef PATOMIC_HAVE_GNU_PRETTY_FUNCTION_EXTN
    #define PATOMIC_HAVE_GNU_PRETTY_FUNCTION_EXTN 0
#endif

/* '__forceinline' keyword is available */
/* requires: Microsoft compatible(-ish) compiler */
#ifndef PATOMIC_HAVE_MS_FORCEINLINE
    #define PATOMIC_HAVE_MS_FORCEINLINE 0
#endif

/* '__attribute__((always_inline)) is available to use without 'inline' */
/* requires: Clang (and maybe other non-GCC GNU compatible compilers) */
#ifndef PATOMIC_HAVE_ALWAYS_INLINE_ATTR
    #define PATOMIC_HAVE_ALWAYS_INLINE_ATTR 0
#endif

/* 'inline __attribute__((always_inline))' is available */
/* requires: 'inline' keyword and GNU compatible(-ish) compiler */
/* note: TCC supports this, but doesn't define __GNUC__ */
#ifndef PATOMIC_HAVE_INLINE_ALWAYS_INLINE_ATTR
    #define PATOMIC_HAVE_INLINE_ALWAYS_INLINE_ATTR 0
#endif

/* '__inline__ __attribute__((always_inline))' is available */
/* requires: GNU compatible(-ish) compiler */
#ifndef PATOMIC_HAVE_GNU_INLINE_ALWAYS_INLINE_ATTR
    #define PATOMIC_HAVE_GNU_INLINE_ALWAYS_INLINE_ATTR 0
#endif

/* '__attribute__((noinline))' is available */
/* requires: GNU compatible(-ish) compiler */
#ifndef PATOMIC_HAVE_NOINLINE_ATTR
    #define PATOMIC_HAVE_NOINLINE_ATTR 0
#endif

/* '__declspec(noinline)' is available */
/* requires: Microsoft compatible(-ish) compiler */
#ifndef PATOMIC_HAVE_NOINLINE_DSPC
    #define PATOMIC_HAVE_NOINLINE_DSPC 0
#endif

/* '_Noreturn' keyword is available */
/* requires: C11 */
#ifndef PATOMIC_HAVE_NORETURN
    #define PATOMIC_HAVE_NORETURN 0
#endif

/* '__declspec(noreturn)' is available */
/* requires: Microsoft compatible(-ish) compiler */
#ifndef PATOMIC_HAVE_NORETURN_DSPC
    #define PATOMIC_HAVE_NORETURN_DSPC 0
#endif

/* '__attribute__((noreturn))' is available */
/* requires: GNU compatible(-ish) compiler */
#ifndef PATOMIC_HAVE_NORETURN_ATTR
    #define PATOMIC_HAVE_NORETURN_ATTR 0
#endif

/* 'restrict' keyword is available */
/* requires: C99 */
#ifndef PATOMIC_HAVE_RESTRICT
    #define PATOMIC_HAVE_RESTRICT 0
#endif

/* '__restrict' keyword is available */
/* requires: most compilers will have this, including MS and GNU */
#ifndef PATOMIC_HAVE_MS_RESTRICT
    #define PATOMIC_HAVE_MS_RESTRICT 0
#endif

/* '__restrict__' keyword is available */
/* requires: GNU compatible(-ish) compiler */
#ifndef PATOMIC_HAVE_GNU_RESTRICT
    #define PATOMIC_HAVE_GNU_RESTRICT 0
#endif

/* '_Alignof' and '_Alignas' keywords are available */
/* requires: C11 */
#ifndef PATOMIC_HAVE_ALIGNOF
    #define PATOMIC_HAVE_ALIGNOF 0
#endif

/* '__alignof' keyword and '__declspec(align(#))' are available */
/* requires: MS compatible(-ish) compiler */
#ifndef PATOMIC_HAVE_MS_ALIGNOF_ALIGN_DSPC
    #define PATOMIC_HAVE_MS_ALIGNOF_ALIGN_DSPC 0
#endif

/* '__alignof__' keyword and '__attribute__((aligned(#)))' are available */
/* requires: GNU compatible(-ish) compiler */
#ifndef PATOMIC_HAVE_GNU_ALIGNOF_ALIGNED_ATTR
    #define PATOMIC_HAVE_GNU_ALIGNOF_ALIGNED_ATTR 0
#endif

/* '__assume(int)' is available */
/* requires: MS compatible(-ish) compiler */
#ifndef PATOMIC_HAVE_MS_ASSUME
    #define PATOMIC_HAVE_MS_ASSUME 0
#endif

/* '__builtin_unreachable(void)' is available */
/* requires: GNU compatible(-ish) compiler */
#ifndef PATOMIC_HAVE_BUILTIN_UNREACHABLE
    #define PATOMIC_HAVE_BUILTIN_UNREACHABLE 0
#endif

/* <stdint.h> header available and 'uintptr_t' available in <stdint.h> */
/* requires: C99 (optional) */
#ifndef PATOMIC_HAVE_STD_INT_UINTPTR
    #define PATOMIC_HAVE_STD_INT_UINTPTR 0
#endif

/* 'uintptr_t' available in <stddef.h> */
/* requires: old compiler versions not supporting <stdint.h> (pre-C99) */
/* note: MSVC puts uintptr_t in here in old versions */
#ifndef PATOMIC_HAVE_STD_DEF_UINTPTR
    #define PATOMIC_HAVE_STD_DEF_UINTPTR 0
#endif

/* <stdatomic.h> header and '_Atomic' keyword are available */
/* requires: C11 and __STD_NO_ATOMICS__ to not be defined */
/* note: currently MSVC defines __STD_NO_ATOMICS__ */
#ifndef PATOMIC_HAVE_STD_ATOMIC
    #define PATOMIC_HAVE_STD_ATOMIC 0
#endif

/* '__sync' builtins are available */
/* requires: GNU compatible(-ish) compiler */
/* note: check the PATOMIC_HAVE_GNU_SYNC_LOCK_FREE macros */
#ifndef PATOMIC_HAVE_GNU_SYNC
    #define PATOMIC_HAVE_GNU_SYNC 0
#endif

/* '__atomic' builtins and '__ATOMIC' constants are available */
/* requires: GNU compatible (-ish) compiler */
#ifndef PATOMIC_HAVE_GNU_ATOMIC
    #define PATOMIC_HAVE_GNU_ATOMIC 0
#endif


/*
 * The following macros may be modified or overridden, and in same cases it may
 * be required, but extreme caution should be taken when doing so.
 *
 * Changing these macros incorrectly may result in this library not working as
 * expected in some situations (e.g. due to calling a function that isn't lock
 * free), or may prevent this library from compiling.
 */

/* GNU builtin __sync op for sizeof(type) == 1 is lock-free */
/* note: this must be modified/overridden to use __sync with GCC < 4.4 */
#ifndef PATOMIC_HAVE_GNU_SYNC_LOCK_FREE_1
    #if PATOMIC_HAVE_GNU_SYNC && defined(__GCC_HAVE_SYNC_COMPARE_AND_SWAP_1)
        #define PATOMIC_HAVE_GNU_SYNC_LOCK_FREE_1 __GCC_HAVE_SYNC_COMPARE_AND_SWAP_1
    #else
        #define PATOMIC_HAVE_GNU_SYNC_LOCK_FREE_1 0
    #endif
#endif

/* GNU builtin __sync op for sizeof(type) == 2 is lock-free */
/* note: this must be modified/overridden to use __sync with GCC < 4.4 */
#ifndef PATOMIC_HAVE_GNU_SYNC_LOCK_FREE_2
    #if PATOMIC_HAVE_GNU_SYNC && defined(__GCC_HAVE_SYNC_COMPARE_AND_SWAP_2)
        #define PATOMIC_HAVE_GNU_SYNC_LOCK_FREE_2 __GCC_HAVE_SYNC_COMPARE_AND_SWAP_2
    #else
        #define PATOMIC_HAVE_GNU_SYNC_LOCK_FREE_2 0
    #endif
#endif

/* GNU builtin __sync op for sizeof(type) == 4 is lock-free */
/* note: this must be modified/overridden to use __sync with GCC < 4.4 */
#ifndef PATOMIC_HAVE_GNU_SYNC_LOCK_FREE_4
    #if PATOMIC_HAVE_GNU_SYNC && defined(__GCC_HAVE_SYNC_COMPARE_AND_SWAP_4)
        #define PATOMIC_HAVE_GNU_SYNC_LOCK_FREE_4 __GCC_HAVE_SYNC_COMPARE_AND_SWAP_4
    #else
        #define PATOMIC_HAVE_GNU_SYNC_LOCK_FREE_4 0
    #endif
#endif

/* GNU builtin __sync op for sizeof(type) == 8 is lock-free */
/* note: this must be modified/overridden to use __sync with GCC < 4.4 */
#ifndef PATOMIC_HAVE_GNU_SYNC_LOCK_FREE_8
    #if PATOMIC_HAVE_GNU_SYNC && defined(__GCC_HAVE_SYNC_COMPARE_AND_SWAP_8)
        #define PATOMIC_HAVE_GNU_SYNC_LOCK_FREE_8 __GCC_HAVE_SYNC_COMPARE_AND_SWAP_8
    #else
        #define PATOMIC_HAVE_GNU_SYNC_LOCK_FREE_8 0
    #endif
#endif

/* GNU builtin __sync op for sizeof(type) == 16 is lock-free */
/* note: this must be modified/overridden to use __sync with GCC < 4.4 */
#ifndef PATOMIC_HAVE_GNU_SYNC_LOCK_FREE_16
    #if PATOMIC_HAVE_GNU_SYNC && defined(__GCC_HAVE_SYNC_COMPARE_AND_SWAP_16)
        #define PATOMIC_HAVE_GNU_SYNC_LOCK_FREE_16 __GCC_HAVE_SYNC_COMPARE_AND_SWAP_16
    #else
        #define PATOMIC_HAVE_GNU_SYNC_LOCK_FREE_16 0
    #endif
#endif


#endif  /* !PATOMIC_PATOMIC_CONFIG_H */
