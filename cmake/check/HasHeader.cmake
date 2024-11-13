# ---- Has Header ----

# ----------------------------------------------------------------
# | Variable                 | Check                             |
# |==========================|===================================|
# | COMPILER_HAS_STDATOMIC_H | <stdatomic.h> header is available |
# | COMPILER_HAS_STDINT_H    | <stdint.h> header is available    |
# | COMPILER_HAS_WCHAR_H     | <wchar.h> header is available     |
# ----------------------------------------------------------------


# <stdatomic.h> header is available
check_c_source_compiles_or_zero(
    SOURCE
        "#include <stdatomic.h> \n\
         int main(void) {}"
    OUTPUT_VARIABLE
        COMPILER_HAS_STDATOMIC_H
)

# <stdint.h> header is available
check_c_source_compiles_or_zero(
    SOURCE
        "#include <stdint.h> \n\
         int main(void) {}"
    OUTPUT_VARIABLE
        COMPILER_HAS_STDINT_H
)

# <wchar.h> header is available
check_c_source_compiles_or_zero(
    SOURCE
        "#include <wchar.h> \n\
         int main(void) {}"
    OUTPUT_VARIABLE
        COMPILER_HAS_WCHAR_H
)
