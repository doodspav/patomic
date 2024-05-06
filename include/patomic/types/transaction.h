#ifndef PATOMIC_TRANSACTION_H
#define PATOMIC_TRANSACTION_H

#include "align.h"

#include <patomic/patomic_export.h>

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @addtogroup transaction
 *
 * @brief
 *   Writing to this flag when it has been provided to a transaction will cause
 *   the transaction to abort.
 *
 * @details
 *   Any modification  to any memory in a cache line that is being used in a
 *   transaction will cause it to abort.
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
 *   The value of PATOMIC_MAX_CACHE_LINE_SIZE may change on major version bumps
 *   which may cause an ABI break with this type.
 *
 * @note
 *   You are not required to align/pad your transaction flag.                   \n
 *   If you align/pad your transaction flag, you are not required to use this
 *   helper type. E.g. in C11 you may use _Alignas or create your own flag
 *   holder type.
 */

typedef struct {
    unsigned char _padding_head[PATOMIC_MAX_CACHE_LINE_SIZE - 1];
    patomic_transaction_flag_t flag;
    unsigned char _padding_tail[PATOMIC_MAX_CACHE_LINE_SIZE];
} patomic_transaction_padded_flag_holder_t;


/**
 * @addtogroup transaction
 *
 * @brief
 *   Ensures that the transaction flag has its own cache line to avoid false
 *   sharing (which may cause a live transaction to unexpectedly abort).
 *
 * @warning
 *   The value of PATOMIC_MAX_CACHE_LINE_SIZE may change without notice, causing
 *   an ABI break with this type. If this is undesirable, use the stable
 *   variant of this type.
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
 *   The flag may be NULL, in which case a locally allocated flag is used.
 *
 * @note
 *   Width is expected to be non-zero, so the zero value is not explicitly
 *   checked and optimised for, however zero is still a valid value.
 */

typedef struct {

    /** @brief Size in bytes of objects to operate on. */
    size_t width;

    /** @brief Number of attempts to make committing transaction. */
    unsigned long attempts;

    /** @brief Read from at the start of each transaction attempt. */
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
 *   The flag and fallback_flag may point to the same flag, or may be NULL, in
 *   which case a locally allocated flag is used.                               \n
 *   The flag tends to guard a read-write code path, and the fallback flag tends
 *   to guard a read-only code path.                                            \n
 *   With this in mind, it is recommended to use flag as a unique read-write
 *   lock and fallback_flag as a shared read-only lock.
 *
 * @note
 *   Width is expected to be non-zero, so the zero value is not explicitly
 *   checked and optimised for, however zero is still a valid value.
 */

typedef struct {

    /** @brief Size in bytes of objects to operate on. */
    size_t width;

    /** @brief Number of attempts to make committing transaction. */
    unsigned long attempts;

    /** @brief Number of attempts to make committing fallback transaction. */
    unsigned long fallback_attempts;

    /** @brief Read from at the start of each transaction attempt. */
    const patomic_transaction_flag_t *flag_nullable;

    /** @brief Read from at the start of each fallback transaction attempt. */
    const patomic_transaction_flag_t *fallback_flag_nullable;

} patomic_transaction_config_wfb_t;


/**
 * @addtogroup transaction
 *
 * @brief
 *   A set of constants used to denote the status of a transaction. The status
 *   is always 8 significant bits.
 *
 * @note
 *   In transactional operations with a fallback path, an explicit abort will
 *   immediately shift execution to the fallback path, regardless of whether
 *   all attempts on the primary path have been exhausted.
 */

typedef enum {

    /** @brief The transaction was committed. */
    patomic_TSUCCESS = 0

    /** @brief The transaction was not committed. */
    ,patomic_TABORTED = 1

    /** @brief The transaction was explicitly aborted by the user. */
    ,patomic_TABORT_EXPLICIT = 0x2  | patomic_TABORTED

    /** @brief There was a memory conflict with another thread. */
    ,patomic_TABORT_CONFLICT = 0x4  | patomic_TABORTED

    /** @brief The transaction accessed too much memory. */
    ,patomic_TABORT_CAPACITY = 0x8  | patomic_TABORTED

    /** @brief An inner nested transaction aborted. */
    ,patomic_TABORT_NESTED   = 0x10 | patomic_TABORTED

    /** @brief A debug trap caused the transaction to abort. */
    ,patomic_TABORT_DEBUG    = 0x20 | patomic_TABORTED

    /** @brief An interrupt caused the transaction to abort. */
    ,patomic_TABORT_INT      = 0x40 | patomic_TABORTED

} patomic_transaction_status_t;


/**
 * @addtogroup transaction
 *
 * @brief
 *   Obtains the abort reason from the status of an explicitly aborted
 *   transaction. If the transaction was not explicitly aborted, returns 0.
 *
 * @note
 *   The abort reason has 8 significant bits.
 */

PATOMIC_EXPORT unsigned char
patomic_transaction_abort_reason(
    unsigned int status
);


/**
 * @addtogroup transaction
 *
 * @brief
 *   Represents the outcome of a transaction operation.
 *
 * @note
 *   If the transaction was configured to run zero attempts, then the status
 *   will default to patomic_TSUCCESS.
 */

typedef struct {

    /** @brief Status from the final attempt at committing the transaction. */
    unsigned int status;

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
 *   If the transaction was configured to run zero attempts, then the status
 *   will default to patomic_TSUCCESS.
 *
 * @note
 *   If fallback_attempts_made is zero, the fallback_status will default to
 *   patomic_TSUCCESS, even if more fallback attempts were configured.
 */

typedef struct {

    /** @brief Status from the final attempt at committing the transaction. */
    unsigned int status;

    /** @brief Status from the final attempt at committing the fallback
     *         transaction. */
    unsigned int fallback_status;

    /** @brief Attempts made to commit the transaction. */
    unsigned long attempts_made;

    /** @brief Attempts made to commit the fallback transaction. */
    unsigned long fallback_attempts_made;

} patomic_transaction_result_wfb_t;


/**
 * @addtogroup transaction
 *
 * @brief
 *   Provides the recommended limits for transactions. The limits are generated
 *   by tests run internally with successively harder values.
 *
 * @details
 *   - "rmw" models cmpxchg's success path (load, compare, store)               \n
 *   - "load" models cmpxchg's failure path (load)                              \n
 *   - the tests used to generate these values are run at least once per program
 *     execution                                                                \n
 *   - tests may be run multiple times until they succeed internally, with the
 *     number of times being unspecified                                        \n
 *   - implementations may cache the values
 *
 * @note
 *   The tests used to generate the values for this type are likely run under
 *   sterile conditions with no memory contention. They represent the best
 *   possible outcome, which may not be achievable in real world scenarios.
 */

typedef struct {

    /** @brief Test transaction performs the equivalent of a successful
     *         fp_cmpxchg_weak with an increasing byte width. This value is
     *         the maximum byte width where the test transaction eventually
     *         succeeded. */
    unsigned long max_rmw_memory;

    /** @brief Test transaction performs the equivalent of a successful fp_load
     *         with an increasing byte width. This value is the maximum byte
     *         width where the test transaction eventually succeeded. */
    unsigned long max_load_memory;

    /** @brief The number of attempts taken for the max_rmw_memory test to
     *         succeed. */
    unsigned long min_rmw_attempts;

    /** @brief The number of attempts taken for the max_load_memory test to
     *         succeed. */
    unsigned long min_load_attempts;

} patomic_transaction_recommended_t;


/**
 * @addtogroup transaction
 *
 * @brief
 *   Transaction safe counterparts to core <string.h> functions.
 *
 * @warning
 *   These functions are **NOT** atomic (which is why they're here are not in
 *   the header with the rest of the atomic functions). They are designed to be
 *   used inside transactions.
 *
 * @warning
 *   Compilers may insert calls to <string.h> functions such as memcpy without
 *   these functions being called explicitly. If "required" is 1, then these
 *   implicit usages of these functions may cause a transaction to unexpectedly
 *   abort.
 *
 * @details
 *   - these functions are direct counterparts to their <string.h> variants,
 *     with identical semantics and constraints                                 \n
 *   - the <string.h> variants may use instructions which may cause a
 *     transaction to abort (e.g. vzeroupper on x86_64 in memcmp)               \n
 *   - these functions are guaranteed not to cause an abort due to the usage of
 *     such instructions (although they may still cause an abort for other
 *     reasons, such as accessing too much memory)                              \n
 *   - these counterparts will typically be significantly faster than a volatile
 *     char loop
 */

typedef struct {

    /** @brief Value is 1 if <string.h> functions may cause a transactional
     *         abort, and 0 if they're safe to use (in which case the function
     *         pointers here will point to the <string.h> variants). */
    int required;

    /** @brief Transaction safe version of <string.h> function memcpy. */
    void * (* fp_memcpy) (void *dst, const void *src, size_t len);

    /** @brief Transaction safe version of <string.h> function memmove. */
    void * (* fp_memmove) (void *dst, const void *src, size_t len);

    /** @brief Transaction safe version of <string.h> function memset. */
    void * (* fp_memset) (void *dst, int value, size_t len);

    /** @brief Transaction safe version of <string.h> function memcmp. */
    void * (* fp_memcmp) (const void *lhs, const void *rhs, size_t len);

} patomic_transaction_safe_string_t;


#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* PATOMIC_TRANSACTION_H */
