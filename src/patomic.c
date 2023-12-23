#include <patomic/patomic.h>


PATOMIC_EXPORT int
patomic_example_add(
    int a,
    int b
)
{
    if (a == 0 && b == 0)
    {
        return 0;
    }
    else
    {
        return a + b;
    }
}
