#ifndef PATOMIC_MEMORY_ORDER_H
#define PATOMIC_MEMORY_ORDER_H

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

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* !PATOMIC_MEMORY_ORDER_H */
