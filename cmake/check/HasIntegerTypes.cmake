# ---- Has Integer Types ----

# ---------------------------------------------------------------------------
# | Variable                    | Check                                     |
# |=============================|===========================================|
# | COMPILER_HAS_LONG_LONG      | long long is supported                    |
# | COMPILER_HAS_LONG_LONG_EXTN | long long is supported with __extension__ |
# | COMPILER_HAS_MS_INT128      | __int128 is supported                     |
# | COMPILER_HAS_MS_INT128_EXTN | __int128 is supported with __extension__  |
# | COMPILER_HAS_STDINT_INTPTR  | <stdint.h> provides intptr_t              |
# | COMPILER_HAS_STDDEF_INTPTR  | <stddef.h> provides intptr_t              |
# ---------------------------------------------------------------------------


# long long is supported
check_c_source_compiles_or_zero(
    "int main(void) { long long x = 0; return (int) x; }"
    COMPILER_HAS_LONG_LONG
)

# long long is supported with __extension__
check_c_source_compiles_or_zero(
    "int main(void) { __extension__ long long x = 0; return (int) x; }"
    COMPILER_HAS_LONG_LONG_EXTN
)

# __int128 is supported
check_c_source_compiles_or_zero(
    "int main(void) { __int128 x = 0; return (int) x; }"
    COMPILER_HAS_MS_INT128
)

# __int128 is supported with __extension__
check_c_source_compiles_or_zero(
    "int main(void) { __extension__ __int128 x = 0; return (int) x; }"
    COMPILER_HAS_MS_INT128_EXTN
)

# <stdint.h> provides intptr_t
check_c_source_compiles_or_zero(
    "#include <stdint.h> \n\
     int main(void) { intptr_t x = 0; return (int) x; }"
    COMPILER_HAS_STDINT_INTPTR
)

# <stddef.h> provides intptr_t
check_c_source_compiles_or_zero(
    "#include <stddef.h> \n\
     int main(void) { intptr_t x = 0; return (int) x; }"
    COMPILER_HAS_STDDEF_INTPTR
)
