# ---- Has Function ----

# -----------------------------------------------------------------------------------------------------------------------------------------------
# | Variable                         | Check                                                                                                    |
# |==================================|==========================================================================================================|
# | COMPILER_HAS_MS_ASSUME           | '__assume(int)' is available as a function                                                               |
# | COMPILER_HAS_BUILTIN_UNREACHABLE | '__builtin_unreachable()' is available as a function                                                     |
# | COMPILER_HAS_WCHAR_FWIDE         | '<wchar.h>' header is available and makes 'fwide(FILE*, int)' available as a function                    |
# | COMPILER_HAS_WCHAR_FWPRINTF      | '<wchar.h>' header is available and makes 'fwprintf(FILE*, const wchar_t*, ...)' available as a function |
# -----------------------------------------------------------------------------------------------------------------------------------------------


# '__assume(int)' is available as a function
check_c_source_compiles_or_zero(
    SOURCE
        "int main(int argc, char **argv) {  \n\
             if (argc < 1) { __assume(0); } \n\
             return (int) argv[0][0];       \n\
         }"
    OUTPUT_VARIABLE
        COMPILER_HAS_MS_ASSUME
)

# '__builtin_unreachable()' is available as a function
check_c_source_compiles_or_zero(
    SOURCE
        "int main(int argc, char **argv) {              \n\
             if (argc < 1) { __builtin_unreachable(); } \n\
             return (int) argv[0][0];                   \n\
         }"
    OUTPUT_VARIABLE
        COMPILER_HAS_BUILTIN_UNREACHABLE
)

# '<wchar.h>' header is available and makes 'fwide(FILE*, int)' available as a function
check_c_source_compiles_or_zero(
    SOURCE
        "#include <wchar.h> \n\
         #include <stdio.h> \n\
         int main(void) { return fwide(stdout, 0); }"
    OUTPUT_VARIABLE
        COMPILER_HAS_WCHAR_FWIDE
    WILL_FAIL_IF_ANY_NOT
        ${COMPILER_HAS_WCHAR_H}
)

# '<wchar.h>' header is available and makes 'fwprintf(FILE*, const wchar_t*, ...)' available as a function
check_c_source_compiles_or_zero(
    SOURCE
        "#include <wchar.h> \n\
         #include <stdio.h> \n\
         int main(void) { return fwprintf(stdout, L\"test\"); }"
    OUTPUT_VARIABLE
        COMPILER_HAS_WCHAR_FWPRINTF
    WILL_FAIL_IF_ANY_NOT
        ${COMPILER_HAS_WCHAR_H}
)
