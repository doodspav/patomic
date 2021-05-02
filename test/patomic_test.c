#include <patomic/patomic.h>

int main(int argc, char** argv)
{
    (void) argv;
    (void) argc;

    typedef patomic_ops_explicit_t poe_t;
    poe_t ops = patomic_create_ops_explicit(32);
    return ops.binary_ops.fp_and != NULL;  /* expected false */
}
