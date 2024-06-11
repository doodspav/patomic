# ---- Has Identifiers ----

# -----------------------------------------------------------------------------------------------------------
# | Variable                              | Check                                                           |
# |=======================================|=================================================================|
# | COMPILER_HAS_FUNC                     | '__func__' is a pre-defined identifier                          |
# | COMPILER_HAS_FUNC_EXTN                | '__extension__ __func__' is a pre-defined identifier            |
# | COMPILER_HAS_GNU_FUNCTION             | '__FUNCTION__' is a pre-defined identifier                      |
# | COMPILER_HAS_GNU_FUNCTION_EXTN        | '__extension__ __FUNCTION__' is a pre-defined identifier        |
# | COMPILER_HAS_GNU_PRETTY_FUNCTION      | '__PRETTY_FUNCTION__' is a pre-defined identifier               |
# | COMPILER_HAS_GNU_PRETTY_FUNCTION_EXTN | '__extension__ __PRETTY_FUNCTION__' is a pre-defined identifier |
# -----------------------------------------------------------------------------------------------------------


# '__func__' is a pre-defined identifier
check_c_source_compiles_or_zero(
    SOURCE
        "int main(void) { return (int) __func__[0]; }"
    OUTPUT_VARIABLE
        COMPILER_HAS_FUNC
)

# '__extension__ __func__' is a pre-defined identifier
check_c_source_compiles_or_zero(
    SOURCE
        "int main(void) { return (int) __extension__ __func__[0]; }"
    OUTPUT_VARIABLE
        COMPILER_HAS_FUNC_EXTN
    WILL_SUCCEED_IF_ALL
        ${COMPILER_HAS_FUNC}
        ${COMPILER_HAS_EXTN}
    WILL_FAIL_IF_ANY_NOT
        ${COMPILER_HAS_EXTN}
)

# '__FUNCTION__' is a pre-defined identifier
check_c_source_compiles_or_zero(
    SOURCE
        "int main(void) { return (int) __FUNCTION__[0]; }"
    OUTPUT_VARIABLE
        COMPILER_HAS_GNU_FUNCTION
)

# '__extension__ __FUNCTION__' is a pre-defined identifier
check_c_source_compiles_or_zero(
    SOURCE
        "int main(void) { return (int) __extension__ __FUNCTION__[0]; }"
    OUTPUT_VARIABLE
        COMPILER_HAS_GNU_FUNCTION_EXTN
    WILL_SUCCEED_IF_ALL
        ${COMPILER_HAS_GNU_FUNCTION}
        ${COMPILER_HAS_EXTN}
    WILL_FAIL_IF_ANY_NOT
        ${COMPILER_HAS_EXTN}
)

# '__PRETTY_FUNCTION__' is a pre-defined identifier
check_c_source_compiles_or_zero(
    SOURCE
        "int main(void) { return (int) __PRETTY_FUNCTION__[0]; }"
    OUTPUT_VARIABLE
        COMPILER_HAS_GNU_PRETTY_FUNCTION
)

# '__extension__ __PRETTY_FUNCTION__' is a pre-defined identifier
check_c_source_compiles_or_zero(
    SOURCE
        "int main(void) { return (int) __extension__ __PRETTY_FUNCTION__[0]; }"
    OUTPUT_VARIABLE
        COMPILER_HAS_GNU_PRETTY_FUNCTION_EXTN
    WILL_SUCCEED_IF_ALL
        ${COMPILER_HAS_GNU_PRETTY_FUNCTION}
        ${COMPILER_HAS_EXTN}
    WILL_FAIL_IF_ANY_NOT
        ${COMPILER_HAS_EXTN}
)
