#include <patomic/types/align.h>
#include <patomic/macros/force_inline.h>


PATOMIC_FORCE_INLINE size_t
patomic_max_cache_line_size(void)
{
    return (size_t) PATOMIC_MAX_CACHE_LINE_SIZE;
}
