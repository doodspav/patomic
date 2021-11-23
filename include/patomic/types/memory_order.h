#ifndef PATOMIC_MEMORY_ORDER_H
#define PATOMIC_MEMORY_ORDER_H

#include <patomic/patomic_export.h>

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


PATOMIC_EXPORT int
patomic_is_valid_order(int order);

PATOMIC_EXPORT int
patomic_is_valid_store_order(int order);

PATOMIC_EXPORT int
patomic_is_valid_load_order(int order);

PATOMIC_EXPORT int
patomic_is_valid_fail_order(int succ, int fail);

PATOMIC_EXPORT int
patomic_cmpxchg_fail_order(int succ);


#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* !PATOMIC_MEMORY_ORDER_H */
