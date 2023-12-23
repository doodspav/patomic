#include <patomic/patomic.h>


PATOMIC_EXPORT int
patomic_example_add(
    int a,
    int b
)
{
    return a + b;
}
