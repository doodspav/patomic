#ifndef PATOMIC_MEMORY_ORDER_H
#define PATOMIC_MEMORY_ORDER_H

#include <patomic/macros/force_inline.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * MEMORY ORDERS
 *
 * - consume is only there for compatibility
 * - consume is not and will not be implemented
 * - consume will always be treated as acquire
 */

typedef enum {
    patomic_RELAXED,
    patomic_CONSUME,
    patomic_ACQUIRE,
    patomic_RELEASE,
    patomic_ACQ_REL,
    patomic_SEQ_CST
} patomic_memory_order_t;


static PATOMIC_FORCE_INLINE int
patomic_cmpxchg_fail_order(int succ)
{
    switch (succ)
    {
        case patomic_RELEASE:
        case patomic_ACQ_REL: return patomic_ACQUIRE;
        default: return succ;
    }
}

static PATOMIC_FORCE_INLINE int
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

static PATOMIC_FORCE_INLINE int
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


#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* !PATOMIC_MEMORY_ORDER_H */
