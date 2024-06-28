#include <patomic/stdlib/terminate.h>

#include <stdlib.h>


void
patomic_terminate(void)
{
    abort();
}
