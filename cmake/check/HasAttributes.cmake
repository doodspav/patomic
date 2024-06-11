# ---- Has Attributes ----

# -------------------------------------------------------------------------------------------------
# | Variable                   | Check                                                            |
# |============================|==================================================================|
# | COMPILER_HAS_NORETURN      | '_Noreturn' is available as a function attribute                 |
# | COMPILER_HAS_NORETURN_ATTR | '__attribute__((noreturn))' is available as a function attribute |
# | COMPILER_HAS_NORETURN_DSPC | '__declspec(noreturn)' is available as a function attribute      |
# -------------------------------------------------------------------------------------------------


# '_Noreturn' is available as a function attribute
check_c_source_compiles_or_zero(
    SOURCE
        "_Noreturn static void loop(void) { while (1); } \n\
         int main(void) { loop(); }"
    OUTPUT_VARIABLE
        COMPILER_HAS_NORETURN
)

# '__attribute__((noreturn))' is available as a function attribute
check_c_source_compiles_or_zero(
    SOURCE
        "__attribute__((noreturn)) static void loop(void) { while (1); } \n\
         int main(void) { loop(); }"
    OUTPUT_VARIABLE
        COMPILER_HAS_NORETURN_ATTR
)

# '__declspec(noreturn)' is available as a function attribute
check_c_source_compiles_or_zero(
    SOURCE
        "__declspec(noreturn) static void loop(void) { while (1); } \n\
         int main(void) { loop(); }"
    OUTPUT_VARIABLE
        COMPILER_HAS_NORETURN_DSPC
)
