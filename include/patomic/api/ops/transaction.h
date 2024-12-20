#ifndef PATOMIC_API_OPS_TRANSACTION_H
#define PATOMIC_API_OPS_TRANSACTION_H

#include "../transaction.h"

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @addtogroup ops.transaction
 *
 * @brief
 *   Function signature for an atomic store operation implemented using a
 *   sequentially consistent transaction.
 *
 * @details
 *   Attempts to atomically store a desired value into bytes, however the
 *   transaction may fail.
 *
 * @param obj
 *   Pointer to bytes into which to atomically store a value.
 *
 * @param desired
 *   Pointer to bytes holding value to be atomically stored.
 *
 * @param config
 *   Configuration for transaction.
 *
 * @param result
 *   Pointer to object holding result of transaction, including the status and
 *   attempts made.
 *
 * @note
 *   If config.attempts == 0, the transaction will not be attempted. The status
 *   will be set to { .code=TABORT_EXPLICIT, .info=TINFO_ZERO_ATTEMPTS,
 *   .reason=0 }, and attempts_made to 0.
 *   Parameters (except for "config" and "result") will not be accessed and may
 *   be passed a default value of 0 or NULL.
 *
 * @note
 *   If config.width == 0, the transaction will not be attempted. The status
 *   will be set to TSUCCESS, and attempts_made to 1. Parameters (except for
 *   "config" and "result") will not be accessed and may be passed a default
 *   value of 0 or NULL.
 *
 * @note
 *   The value of config.flag_nullable (when non-null) is read from at the
 *   start of every transaction attempt. If its value is non-zero, the
 *   transaction is aborted and not retried. The status will be set to
 *   { .code=TABORT_EXPLICIT, .info=TINFO_FLAG_SET, .reason=0 }.
 *
 * @note
 *   The check for config.attempts == 0 takes precedence over the check for
 *   config.width == 0.
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
 *   Atomically loads a value from bytes, however the transaction may fail.
 *
 * @param obj
 *   Pointer to bytes from which a value will be atomically loaded.
 *
 * @param ret
 *   Pointer to bytes into which to write the atomically loaded value.
 *
 * @param config
 *   Configuration for transaction.
 *
 * @param result
 *   Pointer to object holding result of transaction, including the status and
 *   attempts made.
 *
 * @note
 *   If config.attempts == 0, the transaction will not be attempted. The status
 *   will be set to { .code=TABORT_EXPLICIT, .info=TINFO_ZERO_ATTEMPTS,
 *   .reason=0 }, and attempts_made to 0.
 *   Parameters (except for "config" and "result") will not be accessed and may
 *   be passed a default value of 0 or NULL.
 *
 * @note
 *   If config.width == 0, the transaction will not be attempted. The status
 *   will be set to TSUCCESS, and attempts_made to 1. Parameters (except for
 *   "config" and "result") will not be accessed and may be passed a default
 *   value of 0 or NULL.
 *
 * @note
 *   The value of config.flag_nullable (when non-null) is read from at the
 *   start of every transaction attempt. If its value is non-zero, the
 *   transaction is aborted and not retried. The status will be set to
 *   { .code=TABORT_EXPLICIT, .info=TINFO_FLAG_SET, .reason=0 }.
 *
 * @note
 *   The check for config.attempts == 0 takes precedence over the check for
 *   config.width == 0.
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
 *   Atomically replaces the value of bytes with a desired value and returns the
 *   value the bytes held previously, in a single read-modify-write atomic
 *   operation, however the transaction may fail.
 *
 * @param obj
 *   Pointer to bytes whose value to atomically exchange with desired value.
 *
 * @param desired
 *   Pointer to bytes whose value will replace the existing value of "obj".
 *
 * @param ret
 *   Pointer to bytes into which to write the existing value held by "obj".
 *
 * @param config
 *   Configuration for transaction.
 *
 * @param result
 *   Pointer to object holding result of transaction, including the status and
 *   attempts made.
 *
 * @note
 *   If config.attempts == 0, the transaction will not be attempted. The status
 *   will be set to { .code=TABORT_EXPLICIT, .info=TINFO_ZERO_ATTEMPTS,
 *   .reason=0 }, and attempts_made to 0.
 *   Parameters (except for "config" and "result") will not be accessed and may
 *   be passed a default value of 0 or NULL.
 *
 * @note
 *   If config.width == 0, the transaction will not be attempted. The status
 *   will be set to TSUCCESS, and attempts_made to 1. Parameters (except for
 *   "config" and "result") will not be accessed and may be passed a default
 *   value of 0 or NULL.
 *
 * @note
 *   The value of config.flag_nullable (when non-null) is read from at the
 *   start of every transaction attempt. If its value is non-zero, the
 *   transaction is aborted and not retried. The status will be set to
 *   { .code=TABORT_EXPLICIT, .info=TINFO_FLAG_SET, .reason=0 }.
 *
 * @note
 *   The check for config.attempts == 0 takes precedence over the check for
 *   config.width == 0.
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
 *   Atomically compares the value of the bytes with the value of expected
 *   bytes; if these compare equal, replaces the value of the bytes with a
 *   desired value and returns 1, otherwise stores the value of the bytes into
 *   the expected bytes, and returns 0. This is done in a single
 *   read-modify-write atomic operation if 1 is returned, otherwise this is a
 *   single atomic read (load) operation. The primary transaction may fail, in
 *   which case the fallback transaction is tried. If either transaction path
 *   fails, 0 is returned.
 *
 * @param obj
 *   Pointer to bytes whose value to atomically cmpxchg with expected bytes
 *   and desired value.
 *
 * @param expected
 *   Pointer to bytes whose value is compared against the existing value of
 *   "obj", and into which "obj"'s existing value will be stored if the
 *   comparison fails.
 *
 * @param desired
 *   Pointer to bytes whose value will replace the existing value of "obj".
 *
 * @param config
 *   Configuration for transaction.
 *
 * @param result
 *   Pointer to object holding result of transaction, including the status and
 *   attempts made.
 *
 * @returns
 *   The value 1 if the existing value of "obj" compares equal to the value of
 *   "expected", otherwise the value 0. If the transaction fails, 0 is returned.
 *
 * @note
 *   If config.attempts == 0, the primary transaction will not be attempted.
 *   The status will be set to TABORT_EXPLICIT with reason 0, and attempts_made
 *   to 0. The parameter "desired" may be passed a default value of 0 or NULL.
 *   Execution will pass to the fallback transaction.
 *
 * @note
 *   If config.fallback_attempts == 0 and execution passes to the fallback
 *   transaction, the fallback transaction will not be attempted. The
 *   fallback_status will be set to TABORT_EXPLICIT with reason 0, and
 *   fallback_attempts_made to 0. If config.attempts also == 0, then parameters
 *   (except for "config" and "result") may be passed a default value of 0 or
 *   NULL.
 *
 * @note
 *   If config.width == 0, the primary or fallback transaction (whichever is
 *   about to be executed when the check takes place) will not be attempted.
 *   The status or fallback_status will be set to TSUCCESS and attempts_made or
 *   fallback_attempts_made will be set to 1. Parameters (except for "config"
 *   and "result") may be passed a default value of 0 or NULL.
 *
 * @note
 *   The check for config.width == 0 only takes place after either
 *   config.attempts > 0 or config.fallback_attempts > 0, and the check for
 *   the value of config.fallback_attempts only takes place if the primary
 *   transaction has failed.
 *
 * @note
 *   If the primary transaction succeeds, then execution will not pass to
 *   the fallback transaction, and the value of fallback_status and
 *   fallback_attempts_made is unspecified.
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
 *   Atomically tests a single bit from bytes, returning 1 if it is set and 0 if
 *   it is not set. All the bytes are atomically loaded in order to test the
 *   bit. The transaction may fail, in which case 0 is returned.
 *
 * @param obj
 *   Pointer to bytes from which bit will be atomically loaded.
 *
 * @param offset
 *   Zero-based bit index.
 *
 * @param config
 *   Configuration for transaction.
 *
 * @param result
 *   Pointer to object holding result of transaction, including the status and
 *   attempts made.
 *
 * @returns
 *   The value of the tested bit, or 0 if the transaction failed.
 *
 * @note
 *   If config.attempts == 0, the transaction will not be attempted. The status
 *   will be set to { .code=TABORT_EXPLICIT, .info=TINFO_ZERO_ATTEMPTS,
 *   .reason=0 }, and attempts_made to 0.
 *   Parameters (except for "config" and "result") will not be accessed and may
 *   be passed a default value of 0 or NULL.
 *
 * @note
 *   If config.width == 0, the transaction will not be attempted. The status
 *   will be set to TSUCCESS, and attempts_made to 1. Parameters (except for
 *   "config" and "result") will not be accessed and may be passed a default
 *   value of 0 or NULL.
 *
 * @note
 *   The value of config.flag_nullable (when non-null) is read from at the
 *   start of every transaction attempt. If its value is non-zero, the
 *   transaction is aborted and not retried. The status will be set to
 *   { .code=TABORT_EXPLICIT, .info=TINFO_FLAG_SET, .reason=0 }.
 *
 * @note
 *   The check for config.attempts == 0 takes precedence over the check for
 *   config.width == 0.
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
 *   Atomically modifies a single bit from bytes with an implicitly known unary
 *   bitwise operation, returning the original value of the bit (0 or 1). All
 *   the bytes are loaded and stored in a single atomic read-modify-write
 *   operation. The transaction may fail, in which case 0 is returned.
 *
 * @param obj
 *   Pointer to bytes from which bit will be atomically modified.
 *
 * @param offset
 *   Zero-based bit index.
 *
 * @param config
 *   Configuration for transaction.
 *
 * @param result
 *   Pointer to object holding result of transaction, including the status and
 *   attempts made.
 *
 * @returns
 *   The original value of the tested bit before modification, or 0 if the
 *   transaction failed.
 *
 * @note
 *   If config.attempts == 0, the transaction will not be attempted. The status
 *   will be set to { .code=TABORT_EXPLICIT, .info=TINFO_ZERO_ATTEMPTS,
 *   .reason=0 }, and attempts_made to 0.
 *   Parameters (except for "config" and "result") will not be accessed and may
 *   be passed a default value of 0 or NULL.
 *
 * @note
 *   If config.width == 0, the transaction will not be attempted. The status
 *   will be set to TSUCCESS, and attempts_made to 1. Parameters (except for
 *   "config" and "result") will not be accessed and may be passed a default
 *   value of 0 or NULL.
 *
 * @note
 *   The value of config.flag_nullable (when non-null) is read from at the
 *   start of every transaction attempt. If its value is non-zero, the
 *   transaction is aborted and not retried. The status will be set to
 *   { .code=TABORT_EXPLICIT, .info=TINFO_FLAG_SET, .reason=0 }.
 *
 * @note
 *   The check for config.attempts == 0 takes precedence over the check for
 *   config.width == 0.
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
 *   Atomically loads bytes, performs an implicitly known binary operation on
 *   the value, and stores the result in a single atomic read-modify-write
 *   operation, however the transaction may fail.
 *
 * @param obj
 *   Pointer to bytes whose value will be atomically modified, and passed as
 *   first parameter in the binary operation.
 *
 * @param arg
 *   Pointer to bytes whose value is passed as the second parameter in the
 *   binary operation.
 *
 * @param ret
 *   Pointer to bytes into which to write the original value of "obj" before
 *   modification.
 *
 * @param config
 *   Configuration for transaction.
 *
 * @param result
 *   Pointer to object holding result of transaction, including the status and
 *   attempts made.
 *
 * @note
 *   If config.attempts == 0, the transaction will not be attempted. The status
 *   will be set to { .code=TABORT_EXPLICIT, .info=TINFO_ZERO_ATTEMPTS,
 *   .reason=0 }, and attempts_made to 0.
 *   Parameters (except for "config" and "result") will not be accessed and may
 *   be passed a default value of 0 or NULL.
 *
 * @note
 *   If config.width == 0, the transaction will not be attempted. The status
 *   will be set to TSUCCESS, and attempts_made to 1. Parameters (except for
 *   "config" and "result") will not be accessed and may be passed a default
 *   value of 0 or NULL.
 *
 * @note
 *   The value of config.flag_nullable (when non-null) is read from at the
 *   start of every transaction attempt. If its value is non-zero, the
 *   transaction is aborted and not retried. The status will be set to
 *   { .code=TABORT_EXPLICIT, .info=TINFO_FLAG_SET, .reason=0 }.
 *
 * @note
 *   The check for config.attempts == 0 takes precedence over the check for
 *   config.width == 0.
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
 *   Atomically loads bytes, performs an implicitly known unary operation on the
 *   value, and stores the result in a single atomic read-modify-write
 *   operation, however the transaction may fail.
 *
 * @param obj
 *   Pointer to bytes whose value will be atomically modified, and passed as
 *   first parameter in the unary operation.
 *
 * @param ret
 *   Pointer to bytes into which to write the original value of "obj" before
 *   modification.
 *
 * @param config
 *   Configuration for transaction.
 *
 * @param result
 *   Pointer to object holding result of transaction, including the status and
 *   attempts made.
 *
 * @note
 *   If config.attempts == 0, the transaction will not be attempted. The status
 *   will be set to { .code=TABORT_EXPLICIT, .info=TINFO_ZERO_ATTEMPTS,
 *   .reason=0 }, and attempts_made to 0.
 *   Parameters (except for "config" and "result") will not be accessed and may
 *   be passed a default value of 0 or NULL.
 *
 * @note
 *   If config.width == 0, the transaction will not be attempted. The status
 *   will be set to TSUCCESS, and attempts_made to 1. Parameters (except for
 *   "config" and "result") will not be accessed and may be passed a default
 *   value of 0 or NULL.
 *
 * @note
 *   The value of config.flag_nullable (when non-null) is read from at the
 *   start of every transaction attempt. If its value is non-zero, the
 *   transaction is aborted and not retried. The status will be set to
 *   { .code=TABORT_EXPLICIT, .info=TINFO_FLAG_SET, .reason=0 }.
 *
 * @note
 *   The check for config.attempts == 0 takes precedence over the check for
 *   config.width == 0.
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
 *   Atomically loads bytes, performs an implicitly known binary operation on
 *   the value, and stores the result in a single atomic read-modify-write
 *   operation, however the transaction may fail.
 *
 * @param obj
 *   Pointer to bytes whose value will be atomically modified, and passed as
 *   first parameter in the binary operation.
 *
 * @param arg
 *   Pointer to bytes whose value is passed as the second parameter in the
 *   binary operation.
 *
 * @param config
 *   Configuration for transaction.
 *
 * @param result
 *   Pointer to object holding result of transaction, including the status and
 *   attempts made.
 *
 * @note
 *   If config.attempts == 0, the transaction will not be attempted. The status
 *   will be set to { .code=TABORT_EXPLICIT, .info=TINFO_ZERO_ATTEMPTS,
 *   .reason=0 }, and attempts_made to 0.
 *   Parameters (except for "config" and "result") will not be accessed and may
 *   be passed a default value of 0 or NULL.
 *
 * @note
 *   If config.width == 0, the transaction will not be attempted. The status
 *   will be set to TSUCCESS, and attempts_made to 1. Parameters (except for
 *   "config" and "result") will not be accessed and may be passed a default
 *   value of 0 or NULL.
 *
 * @note
 *   The value of config.flag_nullable (when non-null) is read from at the
 *   start of every transaction attempt. If its value is non-zero, the
 *   transaction is aborted and not retried. The status will be set to
 *   { .code=TABORT_EXPLICIT, .info=TINFO_FLAG_SET, .reason=0 }.
 *
 * @note
 *   The check for config.attempts == 0 takes precedence over the check for
 *   config.width == 0.
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
 *   Atomically loads bytes, performs an implicitly known unary operation on the
 *   value, and stores the result in a single atomic read-modify-write
 *   operation, however the transaction may fail.
 *
 * @param obj
 *   Pointer to bytes whose value will be atomically modified, and passed as
 *   first parameter in the unary operation.
 *
 * @param config
 *   Configuration for transaction.
 *
 * @param result
 *   Pointer to object holding result of transaction, including the status and
 *   attempts made.
 *
 * @note
 *   If config.attempts == 0, the transaction will not be attempted. The status
 *   will be set to { .code=TABORT_EXPLICIT, .info=TINFO_ZERO_ATTEMPTS,
 *   .reason=0 }, and attempts_made to 0.
 *   Parameters (except for "config" and "result") will not be accessed and may
 *   be passed a default value of 0 or NULL.
 *
 * @note
 *   If config.width == 0, the transaction will not be attempted. The status
 *   will be set to TSUCCESS, and attempts_made to 1. Parameters (except for
 *   "config" and "result") will not be accessed and may be passed a default
 *   value of 0 or NULL.
 *
 * @note
 *   The value of config.flag_nullable (when non-null) is read from at the
 *   start of every transaction attempt. If its value is non-zero, the
 *   transaction is aborted and not retried. The status will be set to
 *   { .code=TABORT_EXPLICIT, .info=TINFO_FLAG_SET, .reason=0 }.
 *
 * @note
 *   The check for config.attempts == 0 takes precedence over the check for
 *   config.width == 0.
 */
typedef void (* patomic_opsig_transaction_void_noarg_t) (
    volatile void *obj,
    patomic_transaction_config_t config,
    patomic_transaction_result_t *result
);


/**
 * @addtogroup ops.transaction
 *
 * @brief
 *   Function signature for an atomic double compare-exchange (cmpxchg)
 *   operation implemented using a sequentially consistent transaction.
 *
 * @details
 *   Equivalent to performing two compare-exchange operations in a single
 *   atomic transaction, where the primary read-modify-write transaction only
 *   succeeds if both comparisons succeed. The primary transaction may fail, in
 *   which case the fallback read-only (load) transaction is tried. If either
 *   transaction path fails, 0 is returned.
 *
 * @param cxa
 *   Holds pointers to sets of bytes that will be used in first compare-exchange
 *   operation.
 *
 * @param cxb
 *   Holds pointers to sets of bytes that will be used in second
 *   compare-exchange operation.
 *
 * @param config
 *   Configuration for transaction.
 *
 * @param result
 *   Pointer to object holding result of transaction, including the status and
 *   attempts made.
 *
 * @returns
 *   The value 1 if both sets of bytes compare equal to their expected values,
 *   otherwise the value 0. If either transaction fails, 0 is returned.
 *
 * @note
 *   If config.attempts == 0, the primary transaction will not be attempted.
 *   The status will be set to TABORT_EXPLICIT with reason 0, and attempts_made
 *   to 0. The "desired" member of parameters "cxa" and "cxb" may be passed a
 *   default value of 0 or NULL. Execution will pass to the fallback
 *   transaction.
 *
 * @note
 *   If config.fallback_attempts == 0 and execution passes to the fallback
 *   transaction, the fallback transaction will not be attempted. The
 *   fallback_status will be set to TABORT_EXPLICIT with reason 0, and
 *   fallback_attempts_made to 0. If config.attempts also == 0, then parameters
 *   (except for "config" and "result") may be passed a default value of 0 or
 *   NULL.
 *
 * @note
 *   If config.width == 0, the primary or fallback transaction (whichever is
 *   about to be executed when the check takes place) will not be attempted.
 *   The status or fallback_status will be set to TSUCCESS and attempts_made or
 *   fallback_attempts_made will be set to 1. Parameters (except for "config"
 *   and "result") may be passed a default value of 0 or NULL.
 *
 * @note
 *   The check for config.width == 0 only takes place after either
 *   config.attempts > 0 or config.fallback_attempts > 0, and the check for
 *   the value of config.fallback_attempts only takes place if the primary
 *   transaction has failed.
 *
 * @note
 *   If the primary transaction succeeds, then execution will not pass to
 *   the fallback transaction, and the value of fallback_status and
 *   fallback_attempts_made is unspecified.
 */
typedef int (* patomic_opsig_transaction_double_cmpxchg_t) (
    patomic_transaction_cmpxchg_t cxa,
    patomic_transaction_cmpxchg_t cxb,
    patomic_transaction_config_wfb_t config,
    patomic_transaction_result_wfb_t *result
);


/**
 * @addtogroup ops.transaction
 *
 * @brief
 *   Function signature for an atomic multi compare-exchange (cmpxchg)
 *   operation implemented using a sequentially consistent transaction.
 *
 * @details
 *   Equivalent to performing N compare-exchange operations in a single atomic
 *   transaction, where the primary read-modify-write transaction only succeeds
 *   if all N comparisons succeed. The primary transaction may fail, in which
 *   case the fallback read-only (load) transaction is tried. If either
 *   transaction path fails, 0 is returned.
 *
 * @param cxs_buf
 *   Array of objects holding sets of bytes that will be used in each
 *   compare-exchange operation.
 *
 * @param cxs_len
 *   The number of elements in the cxs_buf array.
 *
 * @param config
 *   Configuration for transaction.
 *
 * @param result
 *   Pointer to object holding result of transaction, including the status and
 *   attempts made.
 *
 * @returns
 *   The value 1 if all N sets of bytes compare equal to their expected values,
 *   otherwise the value 0. If either transaction fails, 0 is returned.
 *
 * @note
 *   If config.attempts == 0, the primary transaction will not be attempted.
 *   The status will be set to TABORT_EXPLICIT with reason 0, and attempts_made
 *   to 0. The "desired" member of all elements of parameter "cxs_buf" may be
 *   passed a default value of 0 or NULL. Execution will pass to the fallback
 *   transaction.
 *
 * @note
 *   If config.fallback_attempts == 0 and execution passes to the fallback
 *   transaction, the fallback transaction will not be attempted. The
 *   fallback_status will be set to TABORT_EXPLICIT with reason 0, and
 *   fallback_attempts_made to 0. If config.attempts also == 0, then parameters
 *   (except for "config" and "result") may be passed a default value of 0 or
 *   NULL.
 *
 * @note
 *   If config.width == 0, the primary or fallback transaction (whichever is
 *   about to be executed when the check takes place) will not be attempted.
 *   The status or fallback_status will be set to TSUCCESS and attempts_made or
 *   fallback_attempts_made will be set to 1. Parameters (except for "config"
 *   and "result") may be passed a default value of 0 or NULL.
 *
 * @note
 *   The check for config.width == 0 only takes place after either
 *   config.attempts > 0 or config.fallback_attempts > 0, and the check for
 *   the value of config.fallback_attempts only takes place if the primary
 *   transaction has failed.
 *
 * @note
 *   If the primary transaction succeeds, then execution will not pass to
 *   the fallback transaction, and the value of fallback_status and
 *   fallback_attempts_made is unspecified.
 */
typedef int (* patomic_opsig_transaction_multi_cmpxchg_t) (
    const patomic_transaction_cmpxchg_t *cxs_buf,
    size_t cxs_len,
    patomic_transaction_config_wfb_t config,
    patomic_transaction_result_wfb_t *result
);


/**
 * @addtogroup ops.transaction
 *
 * @brief
 *   Function signature for a generic atomic operation implemented using a
 *   sequentially consistent transaction.
 *
 * @details
 *   The function "fn" is called inside a transaction and is directly passed
 *   "ctx", which is not dereferenced, accessed, or modified in any way before
 *   being passed to the function.
 *
 * @param fn
 *   Function to be called inside transaction.
 *
 * @param ctx
 *   Opaque data to be passed to function.
 *
 * @param config
 *   Configuration for transaction.
 *
 * @param result
 *   Pointer to object holding result of transaction, including the status and
 *   attempts made.
 *
 * @note
 *   If config.attempts == 0, the transaction will not be attempted. The status
 *   will be set to { .code=TABORT_EXPLICIT, .info=TINFO_ZERO_ATTEMPTS,
 *   .reason=0 }, and attempts_made to 0.
 *   Parameters (except for "config" and "result") will not be accessed and may
 *   be passed a default value of 0 or NULL.
 *
 * @note
 *   If config.width == 0, the transaction will not be attempted. The status
 *   will be set to TSUCCESS, and attempts_made to 1. Parameters (except for
 *   "config" and "result") will not be accessed and may be passed a default
 *   value of 0 or NULL. This is still checked even though the width is not
 *   used in this operation.
 *
 * @note
 *   The value of config.flag_nullable (when non-null) is read from at the
 *   start of every transaction attempt. If its value is non-zero, the
 *   transaction is aborted and not retried. The status will be set to
 *   { .code=TABORT_EXPLICIT, .info=TINFO_FLAG_SET, .reason=0 }.
 *
 * @note
 *   The check for config.attempts == 0 takes precedence over the check for
 *   config.width == 0.
 */
typedef void (* patomic_opsig_transaction_generic_t) (
    void (* fn) (void *),
    void *ctx,
    patomic_transaction_config_t config,
    patomic_transaction_result_t *result
);

/**
 * @addtogroup ops.transaction
 *
 * @brief
 *   Function signature for a generic atomic operation with a fallback path
 *   implemented using a sequentially consistent transaction.
 *
 * @details
 *   The function "fn" is called inside a transaction and is directly passed
 *   "ctx", which is not dereferenced, access, or modified in any way before
 *   being passed to the function. If this transaction fails, "result" is
 *   updated and then the same is attempted with "fallback_fn" and
 *   "fallback_ctx". If either transaction path fails, 0 is returned.
 *
 * @param fn
 *   Function to be called inside primary transaction.
 *
 * @param ctx
 *   Opaque data to be passed to function called inside primary transaction.
 *
 * @param fallback_fn
 *   Function to be called inside fallback transaction.
 *
 * @param fallback_ctx
 *   Opaque data to be passed to function called inside fallback transaction.
 *
 * @param config
 *   Configuration for transaction.
 *
 * @param result
 *   Pointer to object holding result of transaction, including the status and
 *   attempts made.
 *
 * @returns
 *   The value 1 if the primary transaction succeeds, otherwise the value 0.
 *
 * @note
 *   If config.attempts == 0, the primary transaction will not be attempted.
 *   The status will be set to TABORT_EXPLICIT with reason 0, and attempts_made
 *   to 0. Parameters "fn" and "ctx" may be passed a default value of 0 or NULL.
 *   Execution will pass to the fallback transaction.
 *
 * @note
 *   If config.fallback_attempts == 0 and execution passes to the fallback
 *   transaction, the fallback transaction will not be attempted. The
 *   fallback_status will be set to TABORT_EXPLICIT with reason 0, and
 *   fallback_attempts_made to 0. Parameters "fallback_fn" and "fallback_ctx"
 *   may be passed a default value of 0 or NULL. If config.attempts also == 0,
 *   then parameters (except for "config" and "result") may be passed a default
 *   value of 0 or NULL.
 *
 * @note
 *   If config.width == 0, the primary or fallback transaction (whichever is
 *   about to be executed when the check takes place) will not be attempted.
 *   The status or fallback_status will be set to TSUCCESS and attempts_made or
 *   fallback_attempts_made will be set to 1. Parameters (except for "config"
 *   and "result") may be passed a default value of 0 or NULL.
 *
 * @note
 *   The check for config.width == 0 only takes place after either
 *   config.attempts > 0 or config.fallback_attempts > 0, and the check for
 *   the value of config.fallback_attempts only takes place if the primary
 *   transaction has failed.
 *
 * @note
 *   If the primary transaction succeeds, then execution will not pass to
 *   the fallback transaction, and the value of fallback_status and
 *   fallback_attempts_made is unspecified.
 */
typedef int (* patomic_opsig_transaction_generic_wfb_t) (
    void (* fn) (void *),
    void *ctx,
    void (* fallback_fn) (void *),
    void *fallback_ctx,
    patomic_transaction_config_wfb_t config,
    patomic_transaction_result_wfb_t *result
);


/**
 * @addtogroup ops.transaction
 *
 * @brief
 *   Function signature for atomic flag test operation implemented without a
 *   transaction with at least patomic_ACQUIRE memory order.
 *
 * @details
 *   This function can potentially be available on platforms where an atomic
 *   load operation is not.
 *
 * @param flag
 *   Pointer to flag object whose value to test.
 *
 * @returns
 *   The value 0 if the flag object not set, otherwise the value 1.
 */
typedef int (* patomic_opsig_transaction_flag_test_t) (
    const patomic_transaction_flag_t *flag
);


/**
 * @addtogroup ops.transaction
 *
 * @brief
 *   Function signature for atomic flag test-and-set operation implemented
 *   without a transaction with at least patomic_ACQ_REL memory order.
 *
 * @details
 *   This function can potentially be available on platforms where an atomic
 *   exchange operation is not.
 *
 * @param flag
 *   Pointer to flag object whose value to test and set.
 *
 * @returns
 *   The value 0 if the flag object was not set when being tested, otherwise the
 *   value 1.
 */
typedef int (* patomic_opsig_transaction_flag_test_set_t) (
    patomic_transaction_flag_t *flag
);


/**
 * @addtogroup ops.transaction
 *
 * @brief
 *   Function signature for atomic flag clear operation implemented without a
 *   transaction with at least patomic_RELEASE memory order.
 *
 * @details
 *   This function can potentially be available on platforms where an atomic
 *   exchange operation is not.
 *
 * @param flag
 *   Pointer to flag object whose value to clear.
 */
typedef void (* patomic_opsig_transaction_flag_clear_t) (
    patomic_transaction_flag_t *flag
);


/**
 * @addtogroup ops.transaction
 *
 * @brief
 *   Function signature for starting a transaction.
 *
 * @details
 *   Starts a transaction. The execution of the transaction will start when this
 *   function is invoked, and end by returning from this function from the same
 *   invocation.
 *
 * @returns
 *   A status which will be 0 if the transaction has been successfully
 *   committed.
 *
 * @note
 *   More information can be obtained from the status using the
 *   patomic_transaction_status_* functions or the
 *   PATOMIC_TRANSACTION_STATUS_* macros.
 */
typedef unsigned long (* patomic_opsig_transaction_tbegin_t) (
    void
);


/**
 * @addtogroup ops.transaction
 *
 * @brief
 *   Function signature for committing a transaction.
 *
 * @details
 *   Commits a transaction, successfully terminating it. Execution control will
 *   pass back to the caller of tbegin.
 *
 * @warning
 *   Calling this outside of an active transaction results in undefined
 *   behaviour.
 */
typedef void (* patomic_opsig_transaction_tcommit_t) (
    void
);


/**
 * @addtogroup ops.transaction
 *
 * @brief
 *   Function signature for aborting all transaction layers.
 *
 * @details
 *   Explicitly aborts a live transaction with a reason that is passed in the
 *   status and an exit code of patomic_TABORT_EXPLICIT. Only the 8 least
 *   significant bits of reason are used (if CHAR_BIT happens to be larger
 *   than 8).
 *
 * @warning
 *   Aborting a nested transaction will abort ALL nested transactions, and
 *   execution control will pass back to the caller of the outermost tbegin.
 *
 * @note
 *   Calling this outside of an active transaction is a no-op.
 */
typedef void (* patomic_opsig_transaction_tabort_all_t) (
    unsigned char reason
);


/**
 * @addtogroup ops.transaction
 *
 * @brief
 *   Function signature for aborting a single transaction layer.
 *
 * @details
 *   Explicitly aborts a live transaction with a reason that is passed in the
 *   status and an exit code of patomic_TABORT_EXPLICIT. Only the 8 least
 *   significant bits of reason are used (if CHAR_BIT happens to be larger
 *   than 8).
 *
 * @warning
 *   Aborting a nested transaction will only abort the current transaction
 *   layer, and execution control will pass back to the last caller of tbegin.
 *
 * @note
 *   Calling this outside of an active transaction is a no-op.
 */
typedef void (* patomic_opsig_transaction_tabort_single_t) (
    unsigned char reason,
    int unused_tag_type_parameter
);


/**
 * @addtogroup ops.transaction
 *
 * @brief
 *   Function signature for testing if currently executing inside a transaction.
 *
 * @returns
 *   A non-zero value if currently executing inside a transaction, otherwise 0.
 *
 * @note
 *   The returned value represents the transaction nesting depth on supported
 *   platforms.
 */
typedef int (* patomic_opsig_transaction_ttest_t) (
    void
);


/**
 * @addtogroup ops.transaction
 *
 * @brief
 *   Set of function pointers for atomic arithmetic operations implemented using
 *   a sequentially consistent transaction. Pointers are NULL if operation is
 *   not supported.
 */
typedef struct {

    /** @brief Atomic addition using two's complement representation implemented
     *         using a sequentially consistent transaction. There is no
     *         undefined behaviour. */
    patomic_opsig_transaction_void_t fp_add;

    /** @brief Atomic subtraction using two's complement representation
     *         implemented using a sequentially consistent transaction. There is
     *         no undefined behaviour. */
    patomic_opsig_transaction_void_t fp_sub;

    /** @brief Atomic increment using two's complement representation
     *         implemented using a sequentially consistent transaction. There is
     *         no undefined behaviour. */
    patomic_opsig_transaction_void_noarg_t fp_inc;

    /** @brief Atomic decrement using two's complement representation
     *         implemented using a sequentially consistent transaction. There is
     *         no undefined behaviour. */
    patomic_opsig_transaction_void_noarg_t fp_dec;

    /** @brief Atomic negation using two's complement representation implemented
     *         using a sequentially consistent transaction. There is no
     *         undefined behaviour. */
    patomic_opsig_transaction_void_noarg_t fp_neg;

    /** @brief Atomic addition using two's complement representation implemented
     *         using a sequentially consistent transaction, returning original
     *         value from before operation. There is no undefined behaviour. */
    patomic_opsig_transaction_fetch_t fp_fetch_add;

    /** @brief Atomic subtraction using two's complement representation
     *         implemented using a sequentially consistent transaction,
     *         returning original value from before operation. There is no
     *         undefined behaviour. */
    patomic_opsig_transaction_fetch_t fp_fetch_sub;

    /** @brief Atomic increment using two's complement representation
     *         implemented using a sequentially consistent transaction,
     *         returning original value from before operation. There is no
     *         undefined behaviour. */
    patomic_opsig_transaction_fetch_noarg_t fp_fetch_inc;

    /** @brief Atomic decrement using two's complement representation
     *         implemented using a sequentially consistent transaction,
     *         returning original value from before operation. There is no
     *         undefined behaviour. */
    patomic_opsig_transaction_fetch_noarg_t fp_fetch_dec;

    /** @brief Atomic negation using two's complement representation implemented
     *         using a sequentially consistent transaction, returning original
     *         value from before operation. There is no undefined behaviour. */
    patomic_opsig_transaction_fetch_noarg_t fp_fetch_neg;

} patomic_ops_transaction_arithmetic_t;


/**
 * @addtogroup ops.transaction
 *
 * @brief
 *   Set of function pointers for atomic binary operations implemented using
 *   a sequentially consistent transaction. Pointers are NULL if operation is
 *   not supported.
 */
typedef struct {

    /** @brief Atomic OR implemented using a sequentially consistent
     *         transaction. */
    patomic_opsig_transaction_void_t fp_or;

    /** @brief Atomic XOR implemented using a sequentially consistent
     *         transaction. */
    patomic_opsig_transaction_void_t fp_xor;

    /** @brief Atomic AND implemented using a sequentially consistent
     *         transaction. */
    patomic_opsig_transaction_void_t fp_and;

    /** @brief Atomic NOT implemented using a sequentially consistent
     *         transaction. */
    patomic_opsig_transaction_void_noarg_t fp_not;

    /** @brief Atomic OR implemented using a sequentially consistent
     *         transaction, returning original value from before operation. */
    patomic_opsig_transaction_fetch_t fp_fetch_or;

    /** @brief Atomic XOR implemented using a sequentially consistent
     *         transaction, returning original value from before operation. */
    patomic_opsig_transaction_fetch_t fp_fetch_xor;

    /** @brief Atomic AND implemented using a sequentially consistent
     *         transaction, returning original value from before operation. */
    patomic_opsig_transaction_fetch_t fp_fetch_and;

    /** @brief Atomic NOT implemented using a sequentially consistent
     *         transaction, returning original value from before operation. */
    patomic_opsig_transaction_fetch_noarg_t fp_fetch_not;

} patomic_ops_transaction_binary_t;


/**
 * @addtogroup ops.transaction
 *
 * @brief
 *   Set of function pointers for atomic bitwise operations implemented using
 *   a sequentially consistent transaction. Pointers are NULL if operation is
 *   not supported.
 */
typedef struct {

    /** @brief Atomic bit test implemented using a sequentially consistent
     *         transaction, returning original bit value. */
    patomic_opsig_transaction_test_t fp_test;

    /** @brief Atomic bit test-and-complement implemented using a sequentially
     *         consistent transaction, returning original bit value from before
     *         operation. */
    patomic_opsig_transaction_test_modify_t fp_test_compl;

    /** @brief Atomic bit test-and-set implemented using a sequentially
     *         consistent transaction, returning original bit value from before
     *         operation. */
    patomic_opsig_transaction_test_modify_t fp_test_set;

    /** @brief Atomic bit test-and-reset implemented using a sequentially
     *         consistent transaction, returning original bit value from before
     *         operation. */
    patomic_opsig_transaction_test_modify_t fp_test_reset;

} patomic_ops_transaction_bitwise_t;


/**
 * @addtogroup ops.transaction
 *
 * @brief
 *   Set of function pointers for atomic exchange operations implemented using
 *   a sequentially consistent transaction. Pointers are NULL if operation is
 *   not supported.
 */
typedef struct {

    /** @brief Atomic exchange implemented using a sequentially consistent
     *         transaction. */
    patomic_opsig_transaction_exchange_t fp_exchange;

    /** @brief Atomic compare-exchange implemented using a sequentially
     *         consistent transaction. Operation may spuriously fail and act as
     *         if the bytes' value does not compare equal to the expected
     *         value. */
    patomic_opsig_transaction_cmpxchg_t fp_cmpxchg_weak;

    /** @brief Atomic compare-exchange implemented using a sequentially
     *         consistent transaction. Operation will never spuriously fail.
     *
     *  @note This will always be NULL as transactions can always spuriously
     *        fail. It is kept for consistency with implicit/explicit ops. */
    patomic_opsig_transaction_cmpxchg_t fp_cmpxchg_strong;

} patomic_ops_transaction_xchg_t;


/**
 * @addtogroup ops.transaction
 *
 * @brief
 *   Set of function pointers for atomic extended transaction-specific
 *   operations implemented using a sequentially consistent transaction.
 *   Pointers are NULL if operation is not supported.
 */
typedef struct {

    /** @brief Atomic double compare-exchange implemented using a sequentially
     *         consistent transaction. */
    patomic_opsig_transaction_double_cmpxchg_t fp_double_cmpxchg;

    /** @brief Atomic multi compare-exchange implemented using a sequentially
     *         consistent transaction. */
    patomic_opsig_transaction_multi_cmpxchg_t fp_multi_cmpxchg;

    /** @brief Atomic generic operation implemented using a sequentially
     *         consistent transaction. */
    patomic_opsig_transaction_generic_t fp_generic;

    /** @brief Atomic generic operation with a fallback path implemented using
     *         a sequentially consistent transaction. */
    patomic_opsig_transaction_generic_wfb_t fp_generic_wfb;

} patomic_ops_transaction_special_t;


/**
 * @addtogroup ops.transaction
 *
 * @brief
 *   Set of function pointers for atomic flag operations with fixed memory
 *   order. Pointers are NULL if operation is not supported.
 *
 * @note
 *   If any non-flag operation is supported, all flag operations will also be
 *   supported, otherwise no flag operations will be supported. This is so that
 *   there is always a way to write to the flag from another thread without
 *   incurring undefined behaviour.
 */
typedef struct {

    /** @brief Atomic flag test operation with fixed memory order. */
    patomic_opsig_transaction_flag_test_t fp_test;

    /** @brief Atomic flag test-and-set operation with fixed memory order. */
    patomic_opsig_transaction_flag_test_set_t fp_test_set;

    /** @brief Atomic flag clear operation with fixed memory order. */
    patomic_opsig_transaction_flag_clear_t fp_clear;

} patomic_ops_transaction_flag_t;


/**
 * @addtogroup ops.transaction
 *
 * @brief
 *   Set of function pointers for raw transaction-specific operations. Pointers
 *   are NULL if operation is not supported.
 *
 * @note
 *   If any raw operations are supported, then both fp_tbegin and fp_tcommit
 *   will also be supported. Otherwise, patomic_feature_check_any_transaction
 *   on opcat_TRAW would be useless.
 *
 * @note
 *   If fp_tabort_single is supported, then fp_tabort_all will also be
 *   supported.
 *
 * @note
 *   The operation fp_ttest is always supported if fp_tdepth is supported. If
 *   both are supported, they will point to the same function and can be use
 *   interchangeably.
 */
typedef struct {

    /** @brief Start a transaction. */
    patomic_opsig_transaction_tbegin_t fp_tbegin;

    /** @brief Commit a transaction. */
    patomic_opsig_transaction_tcommit_t fp_tcommit;

    /** @brief Explicitly abort all transaction layers */
    patomic_opsig_transaction_tabort_all_t fp_tabort_all;

    /** @brief Explicitly abort a single transaction layer. */
    patomic_opsig_transaction_tabort_single_t fp_tabort_single;

    /** @brief Test if currently executing inside a transaction. */
    patomic_opsig_transaction_ttest_t fp_ttest;

    /** @brief Get the current transaction nesting depth. */
    patomic_opsig_transaction_ttest_t fp_tdepth;

} patomic_ops_transaction_raw_t;


/** @addtogroup ops.transaction
 *
 * @brief
 *   Set of function pointers for atomic load and store operations and other
 *   sets of atomic operations implemented using a sequentially consistent
 *   transaction, as well as transaction specific operations. Pointers are NULL
 *   if operation is not supported.
 */
typedef struct {

    /** @brief Atomic store implemented using a sequentially consistent
     *         transaction. */
    patomic_opsig_transaction_store_t fp_store;

    /** @brief Atomic load implemented using a sequentially consistent
     *         transaction. */
    patomic_opsig_transaction_load_t fp_load;

    /** @brief Set of atomic xchg operations implemented using a sequentially
     *         consistent transaction. */
    patomic_ops_transaction_xchg_t xchg_ops;

    /** @brief Set of atomic bitwise operations implemented using a sequentially
     *         consistent transaction. */
    patomic_ops_transaction_bitwise_t bitwise_ops;

    /** @brief Set of atomic binary operations implemented using a sequentially
     *         consistent transaction. */
    patomic_ops_transaction_binary_t binary_ops;

    /** @brief Set of atomic arithmetic operations using two's complement
     *         representation implemented using a sequentially consistent
     *         transaction. */
    patomic_ops_transaction_arithmetic_t arithmetic_ops;

    /** @brief Set of function pointers for atomic extended
     *         transaction-specific operations implemented using a sequentially
     *         consistent transaction. */
    patomic_ops_transaction_special_t special_ops;

    /** @brief Set of function pointers for atomic flag operations with fixed
     *         memory order. */
    patomic_ops_transaction_flag_t flag_ops;

    /** @brief Set of function pointers for raw transaction-specific
     *         operations. */
    patomic_ops_transaction_raw_t raw_ops;

} patomic_ops_transaction_t;


#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* PATOMIC_API_OPS_TRANSACTION_H */
