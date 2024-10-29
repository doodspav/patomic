# ---- Has Function ----

# -----------------------------------------------------------------------------------------------------------------------------------------------
# | Variable                         | Check                                                                                                    |
# |==================================|==========================================================================================================|
# | COMPILER_HAS_MS_ASSUME           | '__assume(int)' is available as a function                                                               |
# | COMPILER_HAS_BUILTIN_UNREACHABLE | '__builtin_unreachable(void)' is available as a function                                                 |
# | COMPILER_HAS_WCHAR_FWIDE         | '<wchar.h>' header is available and makes 'fwide(FILE*, int)' available as a function                    |
# | COMPILER_HAS_WCHAR_FWPRINTF      | '<wchar.h>' header is available and makes 'fwprintf(FILE*, const wchar_t*, ...)' available as a function |
# | COMPILER_HAS_C23_ALIGNOF         | 'alignof(T)' is available as a function                                                                  |
# | COMPILER_HAS_C23_ALIGNOF_EXTN    | '__extension__ alignof(T)' is available as a function                                                    |
# | COMPILER_HAS_C11_ALIGNOF         | '_Alignof(T)' is available as a function                                                                 |
# | COMPILER_HAS_C11_ALIGNOF_EXTN    | '__extension__ _Alignof(T)' is available as a function                                                   |
# | COMPILER_HAS_MS_ALIGNOF          | '__alignof(T)' is available as a function                                                                |
# | COMPILER_HAS_MS_ALIGNOF_EXTN     | '__extension__ __alignof(T)' is available as a function                                                  |
# | COMPILER_HAS_GNU_ALIGNOF         | '__alignof__(T)' is available as a function                                                              |
# | COMPILER_HAS_GNU_ALIGNOF_EXTN    | '__extension__ __alignof__(T)' is available as a function                                                |
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

# '__builtin_unreachable(void)' is available as a function
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

# 'alignof(T)' is available as a function
check_c_source_compiles_or_zero(
    SOURCE
        "int main(void) { return (int) alignof(int); }"
    OUTPUT_VARIABLE
        COMPILER_HAS_C23_ALIGNOF
)

# '__extension__ alignof(T)' is available as a function
check_c_source_compiles_or_zero(
    SOURCE
        "int main(void) { return (int) __extension__ alignof(int); }"
    OUTPUT_VARIABLE
        COMPILER_HAS_C23_ALIGNOF_EXTN
    WILL_SUCCEED_IF_ALL
        ${COMPILER_HAS_C23_ALIGNOF}
        ${COMPILER_HAS_EXTN}
    WILL_FAIL_IF_ANY_NOT
        ${COMPILER_HAS_EXTN}
)

# '_Alignof(T)' is available as a function
check_c_source_compiles_or_zero(
    SOURCE
        "int main(void) { return (int) _Alignof(int); }"
    OUTPUT_VARIABLE
        COMPILER_HAS_C11_ALIGNOF
)

# '__extension__ _Alignof(T)' is available as a function
check_c_source_compiles_or_zero(
    SOURCE
        "int main(void) { return (int) __extension__ _Alignof(int); }"
    OUTPUT_VARIABLE
        COMPILER_HAS_C11_ALIGNOF_EXTN
    WILL_SUCCEED_IF_ALL
        ${COMPILER_HAS_C11_ALIGNOF}
        ${COMPILER_HAS_EXTN}
    WILL_FAIL_IF_ANY_NOT
        ${COMPILER_HAS_EXTN}
)

# '__alignof(T)' is available as a function
check_c_source_compiles_or_zero(
    SOURCE
        "int main(void) { return (int) __alignof(int); }"
    OUTPUT_VARIABLE
        COMPILER_HAS_MS_ALIGNOF
)

# '__extension__ __alignof(T)' is available as a function
check_c_source_compiles_or_zero(
    SOURCE
        "int main(void) { return (int) __extension__ __alignof(int); }"
    OUTPUT_VARIABLE
        COMPILER_HAS_MS_ALIGNOF_EXTN
    WILL_SUCCEED_IF_ALL
        ${COMPILER_HAS_MS_ALIGNOF}
        ${COMPILER_HAS_EXTN}
    WILL_FAIL_IF_ANY_NOT
        ${COMPILER_HAS_EXTN}
)

# '__alignof__(T)' is available as a function
check_c_source_compiles_or_zero(
    SOURCE
        "int main(void) { return (int) __alignof__(int); }"
    OUTPUT_VARIABLE
        COMPILER_HAS_GNU_ALIGNOF
)

# '__extension__ __alignof__(T)' is available as a function
check_c_source_compiles_or_zero(
    SOURCE
        "int main(void) { return (int) __extension__ __alignof__(int); }"
    OUTPUT_VARIABLE
        COMPILER_HAS_GNU_ALIGNOF_EXTN
    WILL_SUCCEED_IF_ALL
        ${COMPILER_HAS_GNU_ALIGNOF}
        ${COMPILER_HAS_EXTN}
    WILL_FAIL_IF_ANY_NOT
        ${COMPILER_HAS_EXTN}
)
