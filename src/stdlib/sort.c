#include <patomic/stdlib/sort.h>


void
patomic_sort(
    void *ptr,
    size_t count,
    size_t size,
    int (*comp)(const void*, const void*)
)
{
    qsort(ptr, count, size, comp);
}
