# ---- Has Integer Types ----

# ---------------------------------------------------------------------------------------------------------------------------
# | Variable                        | Check                                                                                 |
# |=================================|=======================================================================================|
# | COMPILER_HAS_LONG_LONG          | 'long long' is available as a type                                                    |
# | COMPILER_HAS_LONG_LONG_EXTN     | '__extension__ long long' is available as a type                                      |
# | COMPILER_HAS_MS_INT128          | '__int128' is available as a type                                                     |
# | COMPILER_HAS_MS_INT128_EXTN     | '__extension__ __int128' is available as a type                                       |
# | COMPILER_HAS_STDINT             | <stdint.h> header is available                                                        |
# | COMPILER_HAS_STDINT_INTPTR      | <stdint.h> header is available and makes 'intptr_t' available as a type               |
# | COMPILER_HAS_STDINT_INTPTR_EXTN | <stdint.h> header is available and makes '__extension__ intptr_t' available as a type |
# | COMPILER_HAS_STDDEF_INTPTR      | <stddef.h> header is available and makes 'intptr_t' available as a type               |
# | COMPILER_HAS_STDDEF_INTPTR_EXTN | <stddef.h> header is available and makes '__extension__ intptr_t' available as a type |
# ---------------------------------------------------------------------------------------------------------------------------


# 'long long' is available as a type
check_c_source_compiles_or_zero(
    SOURCE
        "int main(void) { long long x = 0; return (int) x; }"
    OUTPUT_VARIABLE
        COMPILER_HAS_LONG_LONG
)

# '__extension__ long long' is available as a type
check_c_source_compiles_or_zero(
    SOURCE
        "int main(void) { __extension__ long long x = 0; return (int) x; }"
    OUTPUT_VARIABLE
        COMPILER_HAS_LONG_LONG_EXTN
    WILL_SUCCEED_IF_ALL
        ${COMPILER_HAS_LONG_LONG}
        ${COMPILER_HAS_EXTN}
)

# '__int128' is available as a type
check_c_source_compiles_or_zero(
    SOURCE
        "int main(void) { __int128 x = 0; return (int) x; }"
    OUTPUT_VARIABLE
        COMPILER_HAS_MS_INT128
)

# '__extension__ __int128' is available as a type
check_c_source_compiles_or_zero(
    SOURCE
        "int main(void) { __extension__ __int128 x = 0; return (int) x; }"
    OUTPUT_VARIABLE
        COMPILER_HAS_MS_INT128_EXTN
    WILL_SUCCEED_IF_ALL
        ${COMPILER_HAS_MS_INT128}
        ${COMPILER_HAS_EXTN}
)

# <stdint.h> header is available
check_c_source_compiles_or_zero(
    SOURCE
        "#include <stdint.h> \n\
         int main(void) {}"
    OUTPUT_VARIABLE
        COMPILER_HAS_STDINT
)

# <stdint.h> header is available and makes 'intptr_t' available as a type
check_c_source_compiles_or_zero(
    SOURCE
        "#include <stdint.h> \n\
         int main(void) { intptr_t x = 0; return (int) x; }"
    OUTPUT_VARIABLE
        COMPILER_HAS_STDINT_INTPTR
)

# <stdint.h> header is available and makes '__extension__ intptr_t' available as a type
check_c_source_compiles_or_zero(
    SOURCE
        "#include <stdint.h> \n\
         int main(void) { __extension__ intptr_t x = 0; return (int) x; }"
    OUTPUT_VARIABLE
        COMPILER_HAS_STDINT_INTPTR_EXTN
    WILL_SUCCEED_IF_ALL
        ${COMPILER_HAS_STDINT_INTPTR}
        ${COMPILER_HAS_EXTN}
)

# <stddef.h> header is available and makes 'intptr_t' available as a type
check_c_source_compiles_or_zero(
    SOURCE
        "#include <stddef.h> \n\
         int main(void) { intptr_t x = 0; return (int) x; }"
    OUTPUT_VARIABLE
        COMPILER_HAS_STDDEF_INTPTR
)

# <stddef.h> header is available and makes '__extension__ intptr_t' available as a type
check_c_source_compiles_or_zero(
    SOURCE
        "#include <stddef.h> \n\
         int main(void) { __extension__ intptr_t x = 0; return (int) x; }"
    OUTPUT_VARIABLE
        COMPILER_HAS_STDDEF_INTPTR_EXTN
    WILL_SUCCEED_IF_ALL
        ${COMPILER_HAS_STDDEF_INTPTR}
        ${COMPILER_HAS_EXTN}

    # fail: COMPILER_HAS_STDINT_INTPTR and not COMPILER_HAS_STDDEF_INTPTR
    # fail: not COMPILER_HAS_EXTN and not COMPILER_HAS_STDDEF_
)
