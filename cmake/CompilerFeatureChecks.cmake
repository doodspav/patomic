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
    "__forceinline static int inc(int x) { return ++x; } \n\
     int main(void) { return inc(0); }"
    COMPILER_HAS_FORCEINLINE
)
zero_if_blank(COMPILER_HAS_FORCEINLINE)

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
    "#include <stdatomic.h> \n\
     int main(void) { return (int)sizeof(_Atomic(int)); }"
    COMPILER_HAS_STD_ATOMIC
)
zero_if_blank(COMPILER_HAS_STD_ATOMIC)
