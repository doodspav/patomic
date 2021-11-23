#include <patomic/types/memory_order.h>
#include <patomic/macros/force_inline.h>


/*
 * FORCE_INLINE NOTE
 *
 * - gcc will convert any runtime memory order value to SEQ_CST
 * - passing a compile-time value __atomic_compare_exchange(_n)'s success
 *   memory order and a runtime value for the failure order will cause a
 *   warning unless success is SEQ_CST
 * - if these functions aren't marked FORCE_INLINE, gcc might not inline them
 * - this is especially important for patomic_cmpxchg_fail_order which will
 *   cause the aforementioned warning if not inlined
 */


PATOMIC_FORCE_INLINE int
patomic_is_valid_order(int order)
{
    switch (order)
    {
        case patomic_RELAXED:
        case patomic_CONSUME:
        case patomic_ACQUIRE:
        case patomic_RELEASE:
        case patomic_ACQ_REL:
        case patomic_SEQ_CST: return 1;
        default: return 0;
    }
}

PATOMIC_FORCE_INLINE int
patomic_is_valid_store_order(int order)
{
    switch (order)
    {
        case patomic_RELAXED:
        case patomic_RELEASE:
        case patomic_SEQ_CST: return 1;
        default: return 0;
    }
}

PATOMIC_FORCE_INLINE int
patomic_is_valid_load_order(int order)
{
    switch (order)
    {
        case patomic_RELAXED:
        case patomic_CONSUME:
        case patomic_ACQUIRE:
        case patomic_SEQ_CST: return 1;
        default: return 0;
    }
}

PATOMIC_FORCE_INLINE int
patomic_is_valid_fail_order(int succ, int fail)
{
    if ((fail > succ)
        || !patomic_is_valid_order(succ)
        || !patomic_is_valid_order(fail)
        || (fail == patomic_RELEASE)
        || (fail == patomic_ACQ_REL))
    { return 0; }
    else { return 1; }
}

PATOMIC_FORCE_INLINE int
patomic_cmpxchg_fail_order(int succ)
{
    switch (succ)
    {
        case patomic_RELEASE:
        case patomic_ACQ_REL: return patomic_ACQUIRE;
        default: return succ;
    }
}
