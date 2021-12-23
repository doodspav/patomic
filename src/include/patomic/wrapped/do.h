#include <patomic/macros/ignore_unused.h>

#include <patomic/stdlib/assert.h>
#include <patomic/stdlib/stdalign.h>
#include <patomic/stdlib/string.h>


#undef HIDE
#undef SHOW

#define HIDE(x)
#define SHOW(x) x

#undef HIDE_P
#undef SHOW_P

#define HIDE_P(x, y)
#define SHOW_P(x, y) ,y


#undef PATOMIC_WRAPPED_DO_ASSERT
#define PATOMIC_WRAPPED_DO_ASSERT(expr) \
    patomic_assert_unreachable(expr)

#undef PATOMIC_WRAPPED_DO_ASSERT_ALIGNED
#define PATOMIC_WRAPPED_DO_ASSERT_ALIGNED(obj, type)        \
    patomic_assert_unreachable(                             \
        patomic_is_aligned(obj, patomic_alignof_type(type)) \
    )

#undef PATOMIC_WRAPPED_DO_MEMCPY
#define PATOMIC_WRAPPED_DO_MEMCPY(dst, src, sz) \
    PATOMIC_IGNORE_UNUSED(patomic_memcpy(dst, src, sz))
