# ---- Has Builtins ----

# ------------------------------------------------------------------------------------------
# | Variable                         | Check                                               |
# |==================================|=====================================================|
# | COMPILER_HAS_MS_ASSUME           | '__assume(int)' is available as a builtin           |
# | COMPILER_HAS_BUILTIN_UNREACHABLE | '__builtin_unreachable()' is available as a builtin |
# ------------------------------------------------------------------------------------------


# '__assume(int)' is available as a builtin
check_c_source_compiles_or_zero(
    SOURCE
        "int main(int argc, char **argv) {  \n\
             if (argc < 1) { __assume(0); } \n\
             return (int) argv[0][0];       \n\
         }"
    OUTPUT_VARIABLE
        COMPILER_HAS_MS_ASSUME
)

# '__builtin_unreachable()' is available as a builtin
check_c_source_compiles_or_zero(
    SOURCE
    "int main(int argc, char **argv) {                  \n\
             if (argc < 1) { __builtin_unreachable(); } \n\
             return (int) argv[0][0];                   \n\
         }"
    OUTPUT_VARIABLE
        COMPILER_HAS_BUILTIN_UNREACHABLE
)
