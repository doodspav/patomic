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
    "int main(void) { _Alignas(_Alignof(int)) int x = 0; return x; }"
    COMPILER_HAS_ALIGNOF
)
zero_if_blank(COMPILER_HAS_ALIGNOF)

check_c_source_compiles(
    "#include <stdatomic.h> \n\
     int main(void) { return (int)sizeof(_Atomic(int)); }"
    COMPILER_HAS_STD_ATOMIC
)
zero_if_blank(COMPILER_HAS_STD_ATOMIC)

check_c_source_compiles(
    "#include <immintrin.h> \n\
     int main(void) { \n\
         unsigned status; \n\
         if ((status = _xbegin()) == _XBEGIN_STARTED) { _xend(); } \n\
         return (int) status; \n\
     }"
    COMPILER_HAS_IMMINTRIN_RTM
)
zero_if_blank(COMPILER_HAS_IMMINTRIN_RTM)

check_c_source_compiles(
    "#include <intrin.h> \n\
     int main(void) { \n\
         int ex[4]; \n\
         __cpuid(ex, 0); \n\
         __cpuidex(ex, 0, 0); \n\
         (void) ex; \n\
         return 0; \n\
     }"
    COMPILER_HAS_INTRIN_CPUID
)
zero_if_blank(COMPILER_HAS_INTRIN_CPUID)

check_c_source_compiles(
    "#include <cpuid.h> \n\
     int main(void) { \n\
         unsigned int ex[4]; \n\
         __cpuid(0, ex[0], ex[1], ex[2], ex[3]); \n\
         __cpuid_count(0, 0, ex[0], ex[1], ex[2], ex[3]); \n\
         (void) ex; \n\
         return 0; \n\
     }"
    COMPILER_HAS_CPUID_CPUID
)
zero_if_blank(COMPILER_HAS_CPUID_CPUID)
