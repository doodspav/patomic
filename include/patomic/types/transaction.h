#ifndef PATOMIC_TRANSACTION_H
#define PATOMIC_TRANSACTION_H

#include <stddef.h>
#include <patomic/types/align.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * TRANSACTION FLAG
 *
 * - used to trigger an abort in a live transaction if modified, by reading from
 *   it at the start of each transaction
 * - any modification to any memory in the same cache line should cause an abort
 *
 * - padded_flag_holder is intended for C90/99 since no alignment utilities
 *   are provided in these standard revisions
 * - it ensures the flag variable has its own cache line to avoid false
 *   sharing (which may cause a live transaction to unexpectedly abort)
 *
 * - you are not required to align/pad your flag
 * - if you do align/pad your flag, you are not required to use the flag holder
 * - e.g. in C11 you may use _Alignas
 */

typedef volatile unsigned char patomic_transaction_flag_t;

typedef struct {
    unsigned char _padding_pre[PATOMIC_MAX_CACHE_LINE_SIZE - 1];
    patomic_transaction_flag_t flag;
    unsigned char _padding_post[PATOMIC_MAX_CACHE_LINE_SIZE];
} patomic_transaction_padded_flag_holder_t;


/*
 * TRANSACTION CMPXCHG
 *
 * - used in n_cmpxchg to pass multiple memory locations
 * - expected and desired may be NULL
 *
 * - if expected is NULL, no comparison (and write-back) is performed
 * - if desired is NULL, no value is stored to obj
 * - setting both to NULL would be a little pointless
 */

typedef struct {
    size_t width;
    volatile void *obj;
    void *expected_nullable;
    const void *desired_nullable;
} patomic_transaction_cmpxchg_t;


/*
 * TRANSACTION CONFIG
 *
 * - width: size in bytes of objects to operate on
 * - attempts: number of attempts to make committing atomic transaction
 * - fallback_attempts: number of attempts to make commit fallback atomic
 *   transaction
 * - flag: read from at the start of atomic transaction
 * - fallback_flag: read from at the start of fallback atomic transaction
 *
 * - flag and fallback_flag may point to the same object, or be NULL (in which
 *   case a locally allocated flag is used)
 *
 * - wfb: with fallback
 */

typedef struct {
    size_t width;
    size_t attempts;
    const patomic_transaction_flag_t *flag;
} patomic_transaction_config_t;

typedef struct {
    size_t width;
    size_t attempts;
    size_t fallback_attempts;
    const patomic_transaction_flag_t *flag;
    const patomic_transaction_flag_t *fallback_flag;
} patomic_transaction_config_wfb_t;


/*
 * TRANSACTION STATUS
 *
 * - success: the atomic operation was committed
 * - aborted: the atomic operation was not committed
 *
 * - explicit: tabort was explicitly called by the user with a reason
 * - conflict: memory conflict with another thread
 * - capacity: transaction used too much memory
 * - nested: abort occurred in inner nested transaction
 * - debug: abort caused by debug trap
 * - int: abort caused by interrupt
 */

/* Note: status can take up to 8 bits (of minimum 16bit uint) */
typedef enum {
     patomic_TSUCCESS = 0
    ,patomic_TABORTED = 1
    ,patomic_TABORT_EXPLICIT = 0x2u  | 1u
    ,patomic_TABORT_CONFLICT = 0x4u  | 1u
    ,patomic_TABORT_CAPACITY = 0x8u  | 1u
    ,patomic_TABORT_NESTED   = 0x10u | 1u
    ,patomic_TABORT_DEBUG    = 0x20u | 1u
    ,patomic_TABORT_INT      = 0x40u | 1u
} patomic_transaction_status_t;

/* Note: reason can take up to 8 bits (of minimum 16bit uint) */
/* Note: value will be 0 if not (x & patomic_TABORT_EXPLICIT) */
PATOMIC_EXPORT unsigned char
patomic_transaction_abort_reason(
    unsigned int status
);


/*
 * TRANSACTION RESULT
 *
 * - status: status obtained in final attempt at atomic operation
 * - fallback_status: status obtained in final attempt at fallback atomic
 *   operation
 * - attempts_made: how many attempts were made to commit the atomic operation
 * - fallback_attempts_made: how many attempts were made to commit the fallback
 *   atomic operation
 *
 * - fallback_status will default to TSUCCESS if fallback_attempts_made is 0
 *
 * - wfb: with fallback
 */

typedef struct {
    unsigned int status;
    size_t attempts_made;
} patomic_transaction_result_t;

typedef struct {
    unsigned int status;
    unsigned int fallback_status;
    size_t attempts_made;
    size_t fallback_attempts_made;
} patomic_transaction_result_wfb_t;


/*
 * TRANSACTION RECOMMENDED
 *
 * - recommended limits for transactions
 *
 * - max_rmw_memory (3N): test transaction reads N bytes from contiguous block A,
 *   compares them N bytes from contiguous block B, and writes them to
 *   contiguous block C. N is the maximum value tested where the transaction
 *   did not fail. max_rmw_memory is the TOTAL memory capacity in all three
 *   contiguous blocks (i.e. 3N).
 * - max_load_memory (2N): test transaction reads N bytes from a contiguous
 *   block A into a contiguous block B. N is the maximum value tested where the
 *   transaction did not fail. max_load_memory is the TOTAL memory capacity in
 *   both contiguous blocks (i.e. 2N).
 *
 * - min_rmw_attempts: the number of attempts taken for the max_rmw_memory test
 *   transaction to succeed
 * - min_load_attempts: the number of attempts taken for the max_load_memory
 *   test transaction to succeed
 *
 * - rmw models cmpxchg's success path (load, cmp, store)
 * - load models cmpxchg's failure path (load)
 * - the tests are run at least once per program execution
 * - implementations may cache the result of each test
 *
 * WARNING:
 * - the tests are run under sterile conditions with no memory contention
 * - they represent the best possible outcome, which may not be achievable in
 *   real world scenarios
 */

typedef struct {
    size_t max_rmw_memory;
    size_t max_load_memory;
    size_t min_rmw_attempts;
    size_t min_load_attempts;
} patomic_transaction_recommended_t;

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* !PATOMIC_TRANSACTION_H */
