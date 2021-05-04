#include <patomic/patomic.h>

int check(void)
{
    typedef patomic_ops_explicit_t poe_t;
    poe_t ops = patomic_create_ops_explicit(32, patomic_options_DEFAULT, 0);

    return ops.binary_ops.fp_and == NULL;
}

int main(int argc, char** argv)
{
    (void) argc;
    (void) argv;

    return !check();
}
