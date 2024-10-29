# ---- Has Keyword ----

# -------------------------------------------------------------------------
# | Variable                  | Check                                     |
# |===========================|===========================================|
# | COMPILER_HAS_EXTN         | '__extension__' is available as a keyword |
# | COMPILER_HAS_RESTRICT     | 'restrict' is available as a keyword      |
# | COMPILER_HAS_MS_RESTRICT  | '__restrict' is available as a keyword    |
# | COMPILER_HAS_GNU_RESTRICT | '__restrict__' is available as a keyword  |
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
