#include <patomic/patomic.h>

#include "impl/register.h"

patomic_ops_explicit_t
patomic_create_ops_explicit(size_t byte_width)
{
    return patomic_impl_register[0].fp_create_ops_explicit(byte_width);
}
