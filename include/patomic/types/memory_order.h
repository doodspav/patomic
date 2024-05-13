#ifndef PATOMIC_MEMORY_ORDER_H
#define PATOMIC_MEMORY_ORDER_H

#include <patomic/patomic_export.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @addtogroup mem_order
 *
 * @brief
 *   Enum constants specifying how memory accesses are to be ordered around an
 *   atomic operation.
 *
 * @details
 *   Each enum label's value and semantics are identical to C++11's
 *   std::memory_order's values and semantics.
 *
 * @note
 *   Consume is only present for compatibility. It is not and will not be
 *   implemented, and will always be treated as patomic_ACQUIRE.
 */
typedef enum {
    patomic_RELAXED,
    /** @note Implemented as patomic_ACQUIRE. */
    patomic_CONSUME,
    patomic_ACQUIRE,
    patomic_RELEASE,
    patomic_ACQ_REL,
    patomic_SEQ_CST
} patomic_memory_order_t;


/** @addtogroup mem_order
 *
 * @brief
 *   Checks that order has a value corresponding to a label in
 *   patomic_memory_order_t.
 */
#define PATOMIC_IS_VALID_ORDER(order) \
    ( (order == patomic_RELAXED) ||   \
      (order == patomic_CONSUME) ||   \
      (order == patomic_ACQUIRE) ||   \
      (order == patomic_RELEASE) ||   \
      (order == patomic_ACQ_REL) ||   \
      (order == patomic_SEQ_CST) )


/**
 * @addtogroup mem_order
 *
 * @brief
 *   Checks that order has a value corresponding to a label in
 *   patomic_memory_order_t, and is valid to use for an atomic store operation.
 */
#define PATOMIC_IS_VALID_STORE_ORDER(order) \
    ( (order == patomic_RELAXED) ||         \
      (order == patomic_RELEASE) ||         \
      (order == patomic_ACQ_REL) ||         \
      (order == patomic_SEQ_CST) )


/**
 * @addtogroup mem_order
 *
 * @brief
 *   Checks that order has a value corresponding to a label in
 *   patomic_memory_order_t, and is valid to use for an atomic load operation.
 */
#define PATOMIC_IS_VALID_LOAD_ORDER(order) \
    ( (order == patomic_RELAXED) ||        \
      (order == patomic_CONSUME) ||        \
      (order == patomic_ACQUIRE) ||        \
      (order == patomic_SEQ_CST) )


/**
 * @addtogroup mem_order
 *
 * @brief
 *   Checks that both succ and fail have a value corresponding to a label in
 *   patomic_memory_order_t, and fail is not stronger than succ or equal to
 *   patomic_RELEASE or patomic_ACQ_REL.
 */
#define PATOMIC_IS_VALID_FAIL_ORDER(succ, fail) \
    ( (succ >= fail)               &&           \
      PATOMIC_IS_VALID_ORDER(succ) &&           \
      PATOMIC_IS_VALID_LOAD_ORDER(fail) )


/**
 * @addtogroup mem_order
 *
 * @brief
 *   Returns the strictest memory order that would satisfy the checks done by
 *   PATOMIC_IS_VALID_FAIL_ORDER(succ, <return>).
 *
 * @warning
 *   If an invalid memory order is passed to this macro, it will be returned
 *   unmodified.
 */
#define PATOMIC_CMPXCHG_FAIL_ORDER(succ)                   \
    ( (succ == patomic_ACQ_REL || succ == patomic_RELEASE) \
      ? patomic_ACQUIRE                                    \
      : succ )


/**
 * @addtogroup mem_order
 *
 * @brief
 *   Checks that order has a value corresponding to a label in
 *   patomic_memory_order_t. This value is identical to the
 *   PATOMIC_IS_VALID_ORDER macro value.
 *
 * @returns If the check succeeds returns 1, else 0.
 */
PATOMIC_EXPORT int
patomic_is_valid_order(int order);


/**
 * @addtogroup mem_order
 *
 * @brief
 *   Checks that order has a value corresponding to a label in
 *   patomic_memory_order_t, and is valid to use for an atomic store operation.
 *   This value is identical to the PATOMIC_IS_VALID_STORE_ORDER macro value.
 *
 * @returns If the check succeeds returns 1, else 0.
 */
PATOMIC_EXPORT int
patomic_is_valid_store_order(int order);


/**
 * @addtogroup mem_order
 *
 * @brief
 *   Checks that order has a value corresponding to a label in
 *   patomic_memory_order_t, and is valid to use for an atomic load operation.
 *   This value is identical to the PATOMIC_IS_VALID_LOAD_ORDER macro value.
 *
 * @returns If the check succeeds returns 1, else 0.
 */
PATOMIC_EXPORT int
patomic_is_valid_load_order(int order);


/**
 * @addtogroup mem_order
 *
 * @brief
 *   Checks that both succ and fail have a value corresponding to a label in
 *   patomic_memory_order_t, and fail is not stronger than succ or equal to
 *   patomic_RELEASE or patomic_ACQ_REL. This value is identical to the
 *   PATOMIC_IS_VALID_FAIL_ORDER macro value.
 *
 * @returns If the check succeeds returns 1, else 0.
 */
PATOMIC_EXPORT int
patomic_is_valid_fail_order(int succ, int fail);


/**
 * @addtogroup mem_order
 *
 * @brief
 *   Returns the strictest memory order that would satisfy the checks done by
 *   patomic_is_valid_fail_order(succ, <return>). This value is identical to the
 *   PATOMIC_CMPXCHG_FAIL_ORDER macro value.
 *
 * @warning
 *   If an invalid memory order is passed to this function, it will be returned
 *   unmodified.
 */
PATOMIC_EXPORT int
patomic_cmpxchg_fail_order(int succ);


#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* PATOMIC_MEMORY_ORDER_H */
