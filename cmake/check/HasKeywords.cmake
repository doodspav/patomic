# ---- Has Keywords ----

# ----------------------------------------------------------------------
# | Variable               | Check                                     |
# |========================|===========================================|
# | COMPILER_HAS_EXTN      | '__extension__' is available as a keyword |
# ----------------------------------------------------------------------


# '__extension__' is available as a keyword
check_c_source_compiles_or_zero(
    "int main(void) { __extension__ int x = 0; return x; }"
    COMPILER_HAS_EXTN
)
