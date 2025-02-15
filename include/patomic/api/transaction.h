/* Copyright (c) doodspav. */
/* SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception */

#ifndef PATOMIC_API_TRANSACTION_H
#define PATOMIC_API_TRANSACTION_H

#include "align.h"

#include <patomic/api/export.h>

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @addtogroup transaction
 *
 * @brief
 *   Writing to this flag when it has been provided to a transaction will cause
 *   the transaction to abort. If this flag is set, the transaction will abort.
 *
 * @details
 *   Any modification by another thread to any memory in a cache line that is
 *   being used in a transaction will cause the transaction to abort.
 */
typedef volatile unsigned char patomic_transaction_flag_t;


/**
 * @addtogroup transaction
 *
 * @brief
 *   Ensures that the transaction flag has its own cache line to avoid false
 *   sharing (which may cause a live transaction to unexpectedly abort).
 *
 * @warning
 *   The value of PATOMIC_MAX_CACHE_LINE_SIZE_ABI_UNSTABLE may change without
 *   notice, causing an ABI break with this type.
 *
 * @note
 *   You are not required to align/pad your transaction flag.                   \n
 *   If you align/pad your transaction flag, you are not required to use this
 *   helper type. E.g. in C11 you may use _Alignas or create your own flag
 *   holder type.
 */
typedef struct {
    unsigned char _padding_head[PATOMIC_MAX_CACHE_LINE_SIZE_ABI_UNSTABLE - 1];
    patomic_transaction_flag_t flag;
    unsigned char _padding_tail[PATOMIC_MAX_CACHE_LINE_SIZE_ABI_UNSTABLE];
} patomic_transaction_padded_flag_holder_abi_unstable_t;


/**
 * @addtogroup transaction
 *
 * @brief
 *   Used in double and multi variants of cmpxchg to pass multiple memory
 *   locations.
 *
 * @note
 *   The byte width of all objects at all the memory locations is obtained
 *   from an object of type patomic_transaction_config(_wfb)_t which is
 *   provided separately.
 */
typedef struct {

    /** @brief Object on which to perform transaction. */
    volatile void *obj;

    /** @brief Value obj must have for transaction to succeed. */
    void *expected;

    /** @brief Value obj will be set to if transaction succeeds. */
    const void *desired;

} patomic_transaction_cmpxchg_t;


/**
 * @addtogroup transaction
 *
 * @brief
 *   Used to configure the execution limits of a transaction, pass user-provided
 *   flags, and determine the byte width of objects at memory locations provided
 *   by patomic_transaction_cmpxchg_t objects.
 *
 * @details
 *   The flag may be NULL.
 */
typedef struct {

    /** @brief Size in bytes of objects to operate on. */
    size_t width;

    /** @brief Number of attempts to make committing transaction. */
    unsigned long attempts;

    /** @brief Read from at the start of each transaction attempt, which is
     *         aborted if the value is non-zero. */
    const patomic_transaction_flag_t *flag_nullable;

} patomic_transaction_config_t;


/**
 * @addtogroup transaction
 *
 * @brief
 *   Used to configure the execution limits of a transaction, pass user-provided
 *   flags, and determine the byte width of objects at memory locations provided
 *   by patomic_transaction_cmpxchg_t objects.
 *
 * @details
 *   The flag and fallback_flag may point to the same flag, or may be NULL.     \n
 *   The flag tends to guard a read-write code path, and the fallback flag tends
 *   to guard a read-only code path.                                            \n
 *   With this in mind, it is recommended to use flag as a unique writer lock
 *   and fallback_flag as a shared reader lock.
 */
typedef struct {

    /** @brief Size in bytes of objects to operate on. */
    size_t width;

    /** @brief Number of attempts to make committing transaction. */
    unsigned long attempts;

    /** @brief Number of attempts to make committing fallback transaction. */
    unsigned long fallback_attempts;

    /** @brief Read from at the start of each transaction attempt, which is
     *         aborted if the value is non-zero. */
    const patomic_transaction_flag_t *flag_nullable;

    /** @brief Read from at the start of each fallback transaction attempt,
     *         which is aborted if the value is non-zero. */
    const patomic_transaction_flag_t *fallback_flag_nullable;

} patomic_transaction_config_wfb_t;


/**
 * @addtogroup transaction
 *
 * @brief
 *   Represents the outcome of a transaction operation.
 *
 * @note
 *   More information can be obtained from the status using the
 *   patomic_transaction_status_* functions or the
 *   PATOMIC_TRANSACTION_STATUS_* macros.
 */
typedef struct {

    /** @brief Status from the final attempt at committing the transaction.
     *         The value is always zero on success. */
    unsigned long status;

    /** @brief Attempts made to commit the transaction. */
    unsigned long attempts_made;

} patomic_transaction_result_t;


/**
 * @addtogroup transaction
 *
 * @brief
 *   Represents the outcome of a transaction operation with a fallback path.
 *
 * @note
 *   More information can be obtained from the status and fallback_status using
 *   the patomic_transaction_status_* functions or the
 *   PATOMIC_TRANSACTION_STATUS_* macros.
 */
typedef struct {

    /** @brief Status from the final attempt at committing the primary
     *         transaction. The value is always zero on success. */
    unsigned long status;

    /** @brief Status from the final attempt at committing the fallback
     *         transaction. The value is always zero on success. */
    unsigned long fallback_status;

    /** @brief Attempts made to commit the primary transaction. */
    unsigned long attempts_made;

    /** @brief Attempts made to commit the fallback transaction. */
    unsigned long fallback_attempts_made;

} patomic_transaction_result_wfb_t;


/**
 * @addtogroup transaction
 *
 * @brief
 *   A set of constants used to denote the success or failure of a transaction.
 *   The exit code always has 8 significant bits and is non-negative.
 *
 * @note
 *   The status will contain a single exit code value.
 *
 * @warning
 *   In transactional operations with a fallback path, an explicit abort will
 *   immediately shift execution to the fallback path, regardless of whether
 *   all attempts on the primary path have been exhausted.
 */
typedef enum {

    /** @brief The transaction was started and committed successfully. */
    patomic_TSUCCESS = 0

    /** @brief The transaction failed for an unknown or implementation specific
     *         reason. */
    ,patomic_TABORT_UNKNOWN = 255

    /** @brief The transaction was explicitly aborted by the user. */
    ,patomic_TABORT_EXPLICIT = 1

    /** @brief The transaction encountered a memory conflict with another
     *         thread. */
    ,patomic_TABORT_CONFLICT

    /** @brief The transaction accessed too much memory. */
    ,patomic_TABORT_CAPACITY

    /** @brief The transaction encountered a debug trap or exception. */
    ,patomic_TABORT_DEBUG

} patomic_transaction_exit_code_t;


/**
 * @addtogroup transaction
 *
 * @brief
 *   A set of constants providing additional transaction abort information to
 *   supplement what is provided by the exit code.
 *   The exit information always has 8 significant bits and is non-negative.
 *
 * @warning
 *   The information should be taken as a hint and not depended upon. Some
 *   implementations may provided more information for certain scenarios than
 *   other implementations.
 *
 * @note
 *   The status may contain zero or more combined exit info values.
 */
typedef enum {

    /** @brief No extra information is available.
     *  @note  This will always be the case if the transaction succeeds. */
    patomic_TINFO_NONE = 0

    /** @brief The transaction explicitly aborted because attempts or fallback
     *         attempts was set to zero.
     *  @note  This is set by this library rather than by an implementation. */
    ,patomic_TINFO_ZERO_ATTEMPTS = (1 << 0)

    /** @brief The transaction explicitly aborted because the flag was set.
     *  @note  This is set by this library rather than by an implementation. */
    ,patomic_TINFO_FLAG_SET = (1 << 1)

    /** @brief The transaction might not fail if retried. */
    ,patomic_TINFO_RETRY = (1 << 0)

    /** @brief The transaction was aborted from an inner nested transaction. */
    ,patomic_TINFO_NESTED = (1 << 1)

} patomic_transaction_exit_info_t;


/**
 * @addtogroup transaction
 *
 * @brief
 *   Obtains the exit code from the status of a transaction.
 *
 * @details
 *   This is bits [0:7] of status.
 */
#define PATOMIC_TRANSACTION_STATUS_EXIT_CODE(status) \
    ((patomic_transaction_exit_code_t) (((unsigned long) (status)) & 0xFFul))


/**
 * @addtogroup transaction
 *
 * @brief
 *   Obtains the exit information from the status of a transaction.
 *
 * @details
 *   This is bits [16:23] of status.
 */
#define PATOMIC_TRANSACTION_STATUS_EXIT_INFO(status) \
    ((unsigned int) ((((unsigned long) (status)) >> 16ul) & 0xFFul))


/**
 * @addtogroup transaction
 *
 * @brief
 *   Obtains the abort reason from the status of a transaction.
 *
 * @details
 *   This is bits [8:15] of status.
 */
#define PATOMIC_TRANSACTION_STATUS_ABORT_REASON(status) \
    ((unsigned char) ((((unsigned long) (status)) >> 8ul) & 0xFFul))


/**
 * @addtogroup transaction
 *
 * @brief
 *   Obtains the exit code from the status of a transaction.
 *
 * @note
 *   The value returned by this function is identical to the
 *   PATOMIC_TRANSACTION_STATUS_EXIT_CODE macro value.
 */
PATOMIC_EXPORT patomic_transaction_exit_code_t
patomic_transaction_status_exit_code(unsigned long status);


/**
 * @addtogroup transaction
 *
 * @brief
 *   Obtains the exit information from the status of a transaction.
 *
 * @note
 *   The value returned by this function is identical to the
 *   PATOMIC_TRANSACTION_STATUS_EXIT_INFO macro value.
 */
PATOMIC_EXPORT unsigned int
patomic_transaction_status_exit_info(unsigned long status);


/**
 * @addtogroup transaction
 *
 * @brief
 *   Obtains the abort reason from the status of a transaction.
 *
 * @note
 *   The value returned by this function is identical to the
 *   PATOMIC_TRANSACTION_STATUS_ABORT_REASON macro value.
 */
PATOMIC_EXPORT unsigned char
patomic_transaction_status_abort_reason(unsigned long status);


#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* PATOMIC_API_TRANSACTION_H */
