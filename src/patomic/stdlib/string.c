#include <patomic/stdlib/string.h>

#include <string.h>


void *
__patomic_string_memcpy(
    void *PATOMIC_RESTRICT dst,
    const void *PATOMIC_RESTRICT src,
    size_t size
)
{
    return memcpy(dst, src, size);
}

void *
__patomic_string_memmove(
    void *PATOMIC_RESTRICT dst,
    const void *PATOMIC_RESTRICT src,
    size_t size
)
{
    return memmove(dst, src, size);
}

void *
__patomic_string_memset(
    void *dst,
    int ch,
    size_t size
)
{
    return memset(dst, ch, size);
}

int
__patomic_string_memcmp(
    const void *lhs,
    const void *rhs,
    size_t size
)
{
    return memcmp(lhs, rhs, size);
}
