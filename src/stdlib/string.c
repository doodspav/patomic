#include <patomic/stdlib/string.h>

#include <string.h>


void *
patomic_memcpy(
    void *const PATOMIC_RESTRICT dest,
    const void *const PATOMIC_RESTRICT src,
    const size_t count
)
{
    return memcpy(dest, src, count);
}
