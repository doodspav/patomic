check_c_source_compiles(
    "int main(void) { long long x = 0; return (int)x; }"
    COMPILER_HAS_LONG_LONG
)
if(COMPILER_HAS_LONG_LONG STREQUAL "")
    set(COMPILER_HAS_LONG_LONG 0)
endif()

check_c_source_compiles(
     "static inline int s(int a, int b) { return a + b; } \n\
     int main(void) { return s(1, 2); }"
    COMPILER_HAS_INLINE
)
if(COMPILER_HAS_INLINE STREQUAL "")
    set(COMPILER_HAS_INLINE 0)
endif()

check_c_source_compiles(
    "int main(void) { _Alignas(0) int x; return (int)_Alignof(int); }"
    COMPILER_HAS_ALIGNOF
)
if(COMPILER_HAS_ALIGNOF STREQUAL "")
    set(COMPILER_HAS_ALIGNOF 0)
endif()

check_c_source_compiles(
    "#include <stdatomic.h> \n\
     int main(void) { return (int)sizeof(_Atomic(int)); }"
    COMPILER_HAS_STD_ATOMIC
)
if(COMPILER_HAS_STD_ATOMIC STREQUAL "")
    set(COMPILER_HAS_STD_ATOMIC 0)
endif()
