#ifndef PATOMIC_OPS_TRANSACTION_H
#define PATOMIC_OPS_TRANSACTION_H

#include "../transaction.h"

// TODO: extern "C"


/**
 * @addtogroup ops.transaction
 *
 * @brief
 *   Function signature for an atomic store operation implemented using a
 *   sequentially consistent transaction.
 *
 * @details
 *   Attempts to atomically store a desired value into an object, however the
 *   transaction may fail.
 *
 * @param obj
 *   Pointer to object into which to atomically store a value.
 *
 * @param desired
 *   Pointer to object holding value to be atomically stored.
 *
 * @param config
 *   Configuration for transaction.
 *
 * @param result
 *   Pointer to object holding result of transaction, including status code and
 *   attempts made.
 *
 * @note
 *   If config.width == 0, the transaction proceeds as normal with no
 *   short-circuiting. Parameters (except for "config" and "result") may be
 *   passed a default value of 0 or NULL.
 *
 * @note
 *   If config.attempts == 0, the transaction will not be attempted. The status
 *   will be set to TABORT_EXPLICIT with reason 0, and attempts_made to 0.
 *   Parameters (except for "config" and "result") may be passed a default value
 *   of 0 or NULL.
 */
typedef void (* patomic_opsig_transaction_store_t) (
    volatile void *obj,
    const void *desired,
    patomic_transaction_config_t config,
    patomic_transaction_result_t *result
);


/**
 * @addtogroup ops.transaction
 *
 * @brief
 *   Function signature for an atomic load operation implemented using a
 *   sequentially consistent transaction.
 *
 * @details
 *   Atomically loads a value from an object, however the transaction may fail.
 *
 * @param obj
 *   Pointer to object from which a value will be atomically loaded.
 *
 * @param ret
 *   Pointer to object into which to write the atomically loaded value.
 *
 * @param config
 *   Configuration for transaction.
 *
 * @param result
 *   Pointer to object holding result of transaction, including status code and
 *   attempts made.
 *
 * @note
 *   If config.width == 0, the transaction proceeds as normal with no
 *   short-circuiting. Parameters (except for "config" and "result") may be
 *   passed a default value of 0 or NULL.
 *
 * @note
 *   If config.attempts == 0, the transaction will not be attempted. The status
 *   will be set to TABORT_EXPLICIT with reason 0, and attempts_made to 0.
 *   Parameters (except for "config" and "result") may be passed a default value
 *   of 0 or NULL.
 */
typedef void (* patomic_opsig_transaction_load_t) (
    const volatile void *obj,
    void *ret,
    patomic_transaction_config_t config,
    patomic_transaction_result_t *result
);


/**
 * @addtogroup ops.transaction
 *
 * @brief
 *   Function signature for an atomic exchange operation implemented using a
 *   sequentially consistent transaction.
 *
 * @details
 *   Atomically replaces the value of an object with a desired value and returns
 *   the value the object held previously, in a single read-modify-write atomic
 *   operation, however the transaction may fail.
 *
 * @param obj
 *   Pointer to object whose value to atomically exchange with desired value.
 *
 * @param desired
 *   Pointer to object whose value will replace the existing value of "obj".
 *
 * @param ret
 *   Pointer to object into which to write the existing value held by "obj".
 *
 * @param config
 *   Configuration for transaction.
 *
 * @param result
 *   Pointer to object holding result of transaction, including status code and
 *   attempts made.
 *
 * @note
 *   If config.width == 0, the transaction proceeds as normal with no
 *   short-circuiting. Parameters (except for "config" and "result") may be
 *   passed a default value of 0 or NULL.
 *
 * @note
 *   If config.attempts == 0, the transaction will not be attempted. The status
 *   will be set to TABORT_EXPLICIT with reason 0, and attempts_made to 0.
 *   Parameters (except for "config" and "result") may be passed a default value
 *   of 0 or NULL.
 */
typedef void (* patomic_opsig_transaction_exchange_t) (
    volatile void *obj,
    const void *desired,
    void *ret,
    patomic_transaction_config_t config,
    patomic_transaction_result_t *result
);


/**
 * @addtogroup ops.transaction
 *
 * @brief
 *   Function signature for an atomic compare-exchange (cmpxchg) operation
 *   implemented using a sequentially consistent transaction.
 *
 * @details
 *   Atomically compares the value of the object with with the value of an
 *   expected object; if these compare equal, replaces the value of the object
 *   with a desired value and returns 1, otherwise stores the value of the
 *   object into the expected object, and returns 0. This is done in a single
 *   read-modify-write atomic operation if 1 is returned, otherwise this is a
 *   single atomic read (load) operation. The primary transaction may fail, in
 *   which case the fallback transaction is tried. If either transaction path
 *   fails, 0 is returned.
 *
 * @param obj
 *   Pointer to object whose value to atomically cmpxchg with expected object
 *   and desired value.
 *
 * @param expected
 *   Pointer to object whose value is compared against the existing value of
 *   "obj", and into which "obj"'s existing value will be stored if the
 *   comparison fails.
 *
 * @param desired
 *   Pointer to object whose value will replace the existing value of "obj".
 *
 * @param config
 *   Configuration for transaction.
 *
 * @param result
 *   Pointer to object holding result of transaction, including status code and
 *   attempts made.
 *
 * @returns
 *   The value 1 if the existing value of "obj" compares equal to the value of
 *   "expected", otherwise the value 0. If the transaction fails, 0 is returned.
 *
 * @note
 *   If config.width == 0, the transaction proceeds as normal with no
 *   short-circuiting. Parameters (except for "config" and "result") may be
 *   passed a default value of 0 or NULL.
 *
 * @note
 *   If config.attempts == 0, the transaction will not be attempted. The status
 *   will be set to TABORT_EXPLICIT with reason 0, and attempts_made to 0.
 *   The parameter "desired" may be passed a default value of 0 or NULL.
 *   Execution will pass to the fallback transaction.
 *
 * @note
 *   If config.fallback_attempts == 0, the fallback transaction will not be
 *   attempted. The fallback status will be set to TABORT_EXPLICIT with reason
 *   0, and fallback_attempts_made to 0. If config.attempts also == 0, then
 *   parameters (except for "config" and "result") may be passed a default value
 *   of 0 or NULL.
 */
typedef int (* patomic_opsig_transaction_cmpxchg_t) (
    volatile void *obj,
    void *expected,
    const void *desired,
    patomic_transaction_config_wfb_t config,
    patomic_transaction_result_wfb_t *result
);


/**
 * @addtogroup ops.transaction
 *
 * @brief
 *   Function signature for an atomic bit test operation implemented using a
 *   sequentially consistent transaction.
 *
 * @details
 *   Atomically tests a single bit from an object, returning 1 if it is set and
 *   0 if it is not set. The whole object is atomically loaded in order to test
 *   the bit. The transaction may fail, in which case 0 is returned.
 *
 * @param obj
 *   Pointer to object from which bit will be atomically loaded.
 *
 * @param offset
 *   Zero-based bit index.
 *
 * @param config
 *   Configuration for transaction.
 *
 * @param result
 *   Pointer to object holding result of transaction, including status code and
 *   attempts made.
 *
 * @returns
 *   The value of the tested bit, or 0 if the transaction failed.
 *
 * @note
 *   If config.width == 0, the transaction proceeds as normal with no
 *   short-circuiting. Parameters (except for "config" and "result") may be
 *   passed a default value of 0 or NULL.
 *
 * @note
 *   If config.attempts == 0, the transaction will not be attempted. The status
 *   will be set to TABORT_EXPLICIT with reason 0, and attempts_made to 0.
 *   Parameters (except for "config" and "result") may be passed a default value
 *   of 0 or NULL.
 */
typedef int (* patomic_opsig_transaction_test_t) (
    const volatile void *obj,
    int offset,
    patomic_transaction_config_t config,
    patomic_transaction_result_t *result
);


/**
 * @addtogroup ops.transaction
 *
 * @brief
 *   Function signature for an atomic bit test-and-set operation implemented
 *   using a sequentially consistent transaction.
 *
 * @details
 *   Atomically modifies a single bit from an object with an implicitly known
 *   unary bitwise operation, returning the original value of the bit (0 or 1).
 *   The whole object is loaded and stored in a single atomic read-modify-write
 *   operation. The transaction may fail, in which case 0 is returned.
 *
 * @param obj
 *   Pointer to object from which bit will be atomically modified.
 *
 * @param offset
 *   Zero-based bit index.
 *
 * @param config
 *   Configuration for transaction.
 *
 * @param result
 *   Pointer to object holding result of transaction, including status code and
 *   attempts made.
 *
 * @returns
 *   The original value of the tested bit before modification, or 0 if the
 *   transaction failed.
 *
 * @note
 *   If config.width == 0, the transaction proceeds as normal with no
 *   short-circuiting. Parameters (except for "config" and "result") may be
 *   passed a default value of 0 or NULL.
 *
 * @note
 *   If config.attempts == 0, the transaction will not be attempted. The status
 *   will be set to TABORT_EXPLICIT with reason 0, and attempts_made to 0.
 *   Parameters (except for "config" and "result") may be passed a default value
 *   of 0 or NULL.
 */
typedef int (* patomic_opsig_transaction_test_modify_t) (
    volatile void *obj,
    int offset,
    patomic_transaction_config_t config,
    patomic_transaction_result_t *result
);


/**
 * @addtogroup ops.transaction
 *
 * @brief
 *   Function signature for an atomic fetch binary operation implemented using
 *   a sequentially consistent transaction.
 *
 * @details
 *   Atomically loads an object, performs an implicitly known binary operation
 *   on the value, and stores the result in a single atomic read-modify-write
 *   operation, however the transaction may fail.
 *
 * @param obj
 *   Pointer to object whose value will be atomically modified, and passed as
 *   first parameter in the binary operation.
 *
 * @param arg
 *   Pointer to object whose value is passed as the second parameter in the
 *   binary operation.
 *
 * @param ret
 *   Pointer to object into which to write the original value of "obj" before
 *   modification.
 *
 * @param config
 *   Configuration for transaction.
 *
 * @param result
 *   Pointer to object holding result of transaction, including status code and
 *   attempts made.
 *
 * @note
 *   If config.width == 0, the transaction proceeds as normal with no
 *   short-circuiting. Parameters (except for "config" and "result") may be
 *   passed a default value of 0 or NULL.
 *
 * @note
 *   If config.attempts == 0, the transaction will not be attempted. The status
 *   will be set to TABORT_EXPLICIT with reason 0, and attempts_made to 0.
 *   Parameters (except for "config" and "result") may be passed a default value
 *   of 0 or NULL.
 */
typedef void (* patomic_opsig_transaction_fetch_t) (
    volatile void *obj,
    const void *arg,
    void *ret,
    patomic_transaction_config_t config,
    patomic_transaction_result_t *result
);


/**
 * @addtogroup ops.transaction
 *
 * @brief
 *   Function signature for an atomic fetch unary operation implemented using a
 *   sequentially consistent transaction.
 *
 * @details
 *   Atomically loads an object, performs an implicitly known unary operation on
 *   the value, and stores the result in a single atomic read-modify-write
 *   operation, however the transaction may fail.
 *
 * @param obj
 *   Pointer to object whose value will be atomically modified, and passed as
 *   first parameter in the unary operation.
 *
 * @param ret
 *   Pointer to object into which to write the original value of "obj" before
 *   modification.
 *
 * @param config
 *   Configuration for transaction.
 *
 * @param result
 *   Pointer to object holding result of transaction, including status code and
 *   attempts made.
 *
 * @note
 *   If config.width == 0, the transaction proceeds as normal with no
 *   short-circuiting. Parameters (except for "config" and "result") may be
 *   passed a default value of 0 or NULL.
 *
 * @note
 *   If config.attempts == 0, the transaction will not be attempted. The status
 *   will be set to TABORT_EXPLICIT with reason 0, and attempts_made to 0.
 *   Parameters (except for "config" and "result") may be passed a default value
 *   of 0 or NULL.
 */
typedef void (* patomic_opsig_transaction_fetch_noarg_t) (
    volatile void *obj,
    void *ret,
    patomic_transaction_config_t config,
    patomic_transaction_result_t *result
);


/**
 * @addtogroup ops.transaction
 *
 * @brief
 *   Function signature for an atomic binary operation implemented using a
 *   sequentially consistent transaction.
 *
 * @details
 *   Atomically loads an object, performs an implicitly known binary operation
 *   on the value, and stores the result in a single atomic read-modify-write
 *   operation, however the transaction may fail.
 *
 * @param obj
 *   Pointer to object whose value will be atomically modified, and passed as
 *   first parameter in the binary operation.
 *
 * @param arg
 *   Pointer to object whose value is passed as the second parameter in the
 *   binary operation.
 *
 * @param config
 *   Configuration for transaction.
 *
 * @param result
 *   Pointer to object holding result of transaction, including status code and
 *   attempts made.
 *
 * @note
 *   If config.width == 0, the transaction proceeds as normal with no
 *   short-circuiting. Parameters (except for "config" and "result") may be
 *   passed a default value of 0 or NULL.
 *
 * @note
 *   If config.attempts == 0, the transaction will not be attempted. The status
 *   will be set to TABORT_EXPLICIT with reason 0, and attempts_made to 0.
 *   Parameters (except for "config" and "result") may be passed a default value
 *   of 0 or NULL.
 */
typedef void (* patomic_opsig_transaction_void_t) (
    volatile void *obj,
    const void *arg,
    patomic_transaction_config_t config,
    patomic_transaction_result_t *result
);


/**
 * @addtogroup ops.transaction
 *
 * @brief
 *   Function signature for an atomic unary operation implemented using a
 *   sequentially consistent transaction.
 *
 * @details
 *   Atomically loads an object, performs an implicitly known unary operation on
 *   the value, and stores the result in a single atomic read-modify-write
 *   operation, however the transaction may fail.
 *
 * @param obj
 *   Pointer to object whose value will be atomically modified, and passed as
 *   first parameter in the unary operation.
 *
 * @param config
 *   Configuration for transaction.
 *
 * @param result
 *   Pointer to object holding result of transaction, including status code and
 *   attempts made.
 *
 * @note
 *   If config.width == 0, the transaction proceeds as normal with no
 *   short-circuiting. Parameters (except for "config" and "result") may be
 *   passed a default value of 0 or NULL.
 *
 * @note
 *   If config.attempts == 0, the transaction will not be attempted. The status
 *   will be set to TABORT_EXPLICIT with reason 0, and attempts_made to 0.
 *   Parameters (except for "config" and "result") may be passed a default value
 *   of 0 or NULL.
 */
typedef void (* patomic_opsig_transaction_void_noarg_t) (
    volatile void *obj,
    patomic_transaction_config_t config,
    patomic_transaction_result_t *result
);


#endif  /* PATOMIC_OPS_TRANSACTION_H */
