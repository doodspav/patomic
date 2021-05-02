#include <patomic/patomic.h>

#include <patomic/macros/force_inline.h>
#include <patomic/macros/ignore_unused.h>

#include <patomic/macros/have_std_atomic.h>
#include <patomic/macros/have_std_alignof.h>
#include <patomic/macros/have_long_long.h>

PATOMIC_FORCE_INLINE int check(void)
{
    typedef patomic_ops_explicit_t poe_t;
    poe_t ops = patomic_create_ops_explicit(32);

    return ops.binary_ops.fp_and == NULL;
}

int main(int argc, char** argv)
{
    PATOMIC_IGNORE_UNUSED(argc);
    PATOMIC_IGNORE_UNUSED(argv);

    return !check();
}
