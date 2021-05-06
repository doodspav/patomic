#include <patomic/patomic.h>

static int check(void)
{
    typedef patomic_explicit_t poe_t;
    poe_t a32 = patomic_create_explicit(32, patomic_options_DEFAULT, 0);

    return patomic_nonnull_ops_count_explicit(&a32.ops);
}

int main(int argc, char** argv)
{
    (void) argc;
    (void) argv;

    return check();
}
