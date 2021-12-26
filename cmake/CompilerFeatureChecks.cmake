include(CheckCSourceCompiles)

function(zero_if_blank var)
    if(${var} STREQUAL "")
        set(${var} 0 PARENT_SCOPE)
    endif()
endfunction()

check_c_source_compiles(
    "int main(void) { long long x = 0; return (int)x; }"
    COMPILER_HAS_LONG_LONG
)
zero_if_blank(COMPILER_HAS_LONG_LONG)

check_c_source_compiles(
    "int main(void) { __extension__ long long x = 0; return (int)x; }"
    COMPILER_HAS_LONG_LONG_EXTN
)
zero_if_blank(COMPILER_HAS_LONG_LONG_EXTN)

check_c_source_compiles(
    "int main(void) { __int128 x = 0; return (int)x; }"
    COMPILER_HAS_MS_INT128
)
zero_if_blank(COMPILER_HAS_MS_INT128)

check_c_source_compiles(
    "int main(void) { __extension__ __int128 x = 0; return (int)x; }"
    COMPILER_HAS_MS_INT128_EXTN
)
zero_if_blank(COMPILER_HAS_MS_INT128_EXTN)

check_c_source_compiles(
    "int main(void) { return __func__[0] == 0; }"
    COMPILER_HAS_FUNC
)
zero_if_blank(COMPILER_HAS_FUNC)

check_c_source_compiles(
    "int main(void) { return __FUNCTION__[0] == 0; }"
    COMPILER_HAS_GNU_FUNCTION
)
zero_if_blank(COMPILER_HAS_GNU_FUNCTION)

check_c_source_compiles(
    "int main(void) { return (__extension__ __FUNCTION__)[0] == 0; }"
    COMPILER_HAS_GNU_FUNCTION_EXTN
)
zero_if_blank(COMPILER_HAS_GNU_FUNCTION_EXTN)

check_c_source_compiles(
    "int main(void) { return __PRETTY_FUNCTION__[0] == 0; }"
    COMPILER_HAS_GNU_PRETTY_FUNCTION
)
zero_if_blank(COMPILER_HAS_GNU_PRETTY_FUNCTION)

check_c_source_compiles(
    "int main(void) { return (__extension__ __PRETTY_FUNCTION__)[0] == 0; }"
    COMPILER_HAS_GNU_PRETTY_FUNCTION_EXTN
)
zero_if_blank(COMPILER_HAS_GNU_PRETTY_FUNCTION_EXTN)

check_c_source_compiles(
    "__forceinline static int inc(int x) { return ++x; } \n\
     int main(void) { return inc(0); }"
    COMPILER_HAS_MS_FORCEINLINE
)
zero_if_blank(COMPILER_HAS_MS_FORCEINLINE)

check_c_source_compiles(
    "__attribute__((always_inline)) static \n\
     int inc(int x) { return ++x; } \n\
     int main(void) { return inc(0); }"
    COMPILER_HAS_ALWAYS_INLINE_ATTR
)
zero_if_blank(COMPILER_HAS_ALWAYS_INLINE_ATTR)

check_c_source_compiles(
    "inline __attribute__((always_inline)) static \n\
     int inc(int x) { return ++x; } \n\
     int main(void) { return inc(0); }"
    COMPILER_HAS_INLINE_ALWAYS_INLINE_ATTR
)
zero_if_blank(COMPILER_HAS_INLINE_ALWAYS_INLINE_ATTR)

check_c_source_compiles(
     "__inline__ __attribute__((always_inline)) static \n\
      int inc(int x) { return ++x; } \n\
      int main(void) { return inc(0); }"
     COMPILER_HAS_GNU_INLINE_ALWAYS_INLINE_ATTR
)
zero_if_blank(COMPILER_HAS_GNU_INLINE_ALWAYS_INLINE_ATTR)

check_c_source_compiles(
    "__attribute__((noinline)) static int inc(int x) { return ++x; } \n\
     int main(void) { return inc(0); }"
    COMPILER_HAS_NOINLINE_ATTR
)
zero_if_blank(COMPILER_HAS_NOINLINE_ATTR)

check_c_source_compiles(
    "__declspec(noinline) static int inc(int x) { return ++x; } \n\
     int main(void) { return inc(0); }"
    COMPILER_HAS_NOINLINE_DSPC
)
zero_if_blank(COMPILER_HAS_NOINLINE_DSPC)

check_c_source_compiles(
    "_Noreturn static void loop(void) { while (1); } \n\
     int main(void) { loop(); }"
    COMPILER_HAS_NORETURN
)
zero_if_blank(COMPILER_HAS_NORETURN)

check_c_source_compiles(
    "__declspec(noreturn) static void loop(void) { while (1); } \n\
     int main(void) { loop(); }"
    COMPILER_HAS_NORETURN_DSPC
)
zero_if_blank(COMPILER_HAS_NORETURN_DSPC)

check_c_source_compiles(
    "__attribute__((noreturn)) static void loop(void) { while (1); } \n\
     int main(void) { loop(); }"
    COMPILER_HAS_NORETURN_ATTR
)
zero_if_blank(COMPILER_HAS_NORETURN_ATTR)

check_c_source_compiles(
    "static void inc(int *restrict p) { ++(*p); } \n\
     int main(void) { int i = 0; inc(&i); return i; }"
    COMPILER_HAS_RESTRICT
)
zero_if_blank(COMPILER_HAS_RESTRICT)

check_c_source_compiles(
    "static void inc(int *__restrict p) { ++(*p); } \n\
     int main(void) { int i = 0; inc(&i); return i; }"
    COMPILER_HAS_MS_RESTRICT
)
zero_if_blank(COMPILER_HAS_MS_RESTRICT)

check_c_source_compiles(
    "static void inc(int *__restrict__ p) { ++(*p); } \n\
     int main(void) { int i = 0; inc(&i); return i; }"
    COMPILER_HAS_GNU_RESTRICT
)
zero_if_blank(COMPILER_HAS_GNU_RESTRICT)

check_c_source_compiles(
    "int main(void) { _Alignas(_Alignof(int)) int x = 0; return x; }"
    COMPILER_HAS_ALIGNOF
)
zero_if_blank(COMPILER_HAS_ALIGNOF)

check_c_source_compiles(
    "int main(void) { \n\
        int y = 0;
        __declspec(align(1)) int x; \n\
        x = __alignof(int); return x + y; }"
    COMPILER_HAS_MS_ALIGNOF_ALIGN_DSPC
)
zero_if_blank(COMPILER_HAS_MS_ALIGNOF_ALIGN_DSPC)

check_c_source_compiles(
    "int main(void) { \n\
        __attribute__((aligned(__alignof__(int)))) int x; \n\
        x = 0; return x; }"
    COMPILER_HAS_GNU_ALIGNOF_ALIGNED_ATTR
)
zero_if_blank(COMPILER_HAS_GNU_ALIGNOF_ALIGNED_ATTR)

check_c_source_compiles(
    "int main(int argc, char **argv) { \n\
         if (argc < 1) { __assume(0); } \n\
         return argv[0][0]; }"
    COMPILER_HAS_MS_ASSUME
)
zero_if_blank(COMPILER_HAS_MS_ASSUME)

check_c_source_compiles(
    "int main(int argc, char **argv) { \n\
         if (argc < 1) { __builtin_unreachable(); } \n\
         return argv[0][0]; }"
    COMPILER_HAS_BUILTIN_UNREACHABLE
)
zero_if_blank(COMPILER_HAS_BUILTIN_UNREACHABLE)

check_c_source_compiles(
    "int main(int argc, char **argv) { \n\
         if (__builtin_expect(argc < 1, 0)) { return -1; } \n\
         else { return argv[0][0]; } }"
    COMPILER_HAS_BUILTIN_EXPECT
)
zero_if_blank(COMPILER_HAS_BUILTIN_EXPECT)

check_c_source_compiles(
    "#include <stdint.h> \n\
     int main(void) { uintptr_t x = 0; return (int)x; }"
    COMPILER_HAS_STD_INT_UINTPTR
)
zero_if_blank(COMPILER_HAS_STD_INT_UINTPTR)

check_c_source_compiles(
    "#include <stddef.h> \n\
     int main(void) { uintptr_t x = 0; return (int)x; }"
    COMPILER_HAS_STD_DEF_UINTPTR
)
zero_if_blank(COMPILER_HAS_STD_DEF_UINTPTR)

check_c_source_compiles(
    "#include <stdatomic.h> \n\
     int main(void) { return (int)sizeof(_Atomic(int)); }"
    COMPILER_HAS_STD_ATOMIC
)
zero_if_blank(COMPILER_HAS_STD_ATOMIC)

check_c_source_compiles(
    "int main(void) { \n\
         int x = 0; \n\
         return (int) __sync_fetch_and_add(&x, 1); }"
    COMPILER_HAS_GNU_SYNC
)
zero_if_blank(COMPILER_HAS_GNU_SYNC)

check_c_source_compiles(
    "int main(void) { \n\
         int x = 1; \n\
         int y = 0; \n\
         __atomic_store(&y, &x, __ATOMIC_SEQ_CST); \n\
         (void) __atomic_fetch_add(&x, 5, __ATOMIC_ACQ_REL); \n\
         return __atomic_load_n(&x, __ATOMIC_ACQUIRE); }"
    COMPILER_HAS_GNU_ATOMIC
)
zero_if_blank(COMPILER_HAS_GNU_ATOMIC)
