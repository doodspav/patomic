#include <patomic/patomic.h>

int check(void)
{
    typedef patomic_explicit_t poe_t;
    poe_t a32 = patomic_create_explicit(32, patomic_options_DEFAULT, 0);

    return a32.ops.binary_ops.fp_and == NULL;
}

int main(int argc, char** argv)
{
    (void) argc;
    (void) argv;

    return !check();
}
