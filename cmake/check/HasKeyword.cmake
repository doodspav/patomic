# Copyright (c) doodspav.
# SPDX-License-Identifier: LGPL-3.0-only WITH LGPL-3.0-linking-exception

# ---- Has Keyword ----

# -------------------------------------------------------------------------
# | Variable                  | Check                                     |
# |===========================|===========================================|
# | COMPILER_HAS_EXTN         | '__extension__' is available as a keyword |
# | COMPILER_HAS_RESTRICT     | 'restrict' is available as a keyword      |
# | COMPILER_HAS_MS_RESTRICT  | '__restrict' is available as a keyword    |
# | COMPILER_HAS_GNU_RESTRICT | '__restrict__' is available as a keyword  |
# | COMPILER_HAS_ATOMIC       | '_Atomic' is available as a keyword       |
# -------------------------------------------------------------------------


# '__extension__' is available as a keyword
check_c_source_compiles_or_zero(
    SOURCE
        "int main(void) { __extension__ int x = 0; return x; }"
    OUTPUT_VARIABLE
        COMPILER_HAS_EXTN
)

# 'restrict' is available as a keyword
check_c_source_compiles_or_zero(
    SOURCE
        "static int get(const int *const restrict p) { return *p; } \n\
         int main(void) { int x = 0; return get(&x); }"
    OUTPUT_VARIABLE
        COMPILER_HAS_RESTRICT
)

# '__restrict' is available as a keyword
check_c_source_compiles_or_zero(
    SOURCE
        "static int get(const int *const __restrict p) { return *p; } \n\
         int main(void) { int x = 0; return get(&x); }"
    OUTPUT_VARIABLE
        COMPILER_HAS_MS_RESTRICT
)

# '__restrict__' is available as a keyword
check_c_source_compiles_or_zero(
    SOURCE
        "static int get(const int *const __restrict__ p) { return *p; } \n\
         int main(void) { int x = 0; return get(&x); }"
    OUTPUT_VARIABLE
        COMPILER_HAS_GNU_RESTRICT
)

# '_Atomic' is available as a keyword
check_c_source_compiles_or_zero(
    SOURCE
        "#include <stdatomic.h> \n\
         int main(void) { \n\
             static const _Atomic(unsigned char)  a1; \n\
             static const _Atomic(unsigned short) a2; \n\
             static const _Atomic(unsigned int)   a3; \n\
             static const _Atomic(unsigned long)  a4; \n\
             const volatile _Atomic(unsigned char)  *const p1 = &a1; \n\
             const volatile _Atomic(unsigned short) *const p2 = &a2; \n\
             const volatile _Atomic(unsigned int)   *const p3 = &a3; \n\
             const volatile _Atomic(unsigned long)  *const p4 = &a4; \n\
             unsigned long sum = 0; \n\
             sum += (unsigned long) atomic_load(p1); \n\
             sum += (unsigned long) atomic_load(p2); \n\
             sum += (unsigned long) atomic_load(p3); \n\
             sum += (unsigned long) atomic_load(p4); \n\
             return (int) sum; \n\
         }"
    OUTPUT_VARIABLE
        COMPILER_HAS_ATOMIC
    WILL_FAIL_IF_ANY_NOT
        ${COMPILER_HAS_STDATOMIC_H}
)
