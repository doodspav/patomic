#ifndef PATOMIC_TRANSACTION_H
#define PATOMIC_TRANSACTION_H

#include <stddef.h>
#include <patomic/types/align.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * TRANSACTION FLAG
 * - used to trigger an abort in a live transaction if modified, by reading from
 *   it at the start of each transaction
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
 * - used in n_cmpxchg to pass multiple memory locations
 */

typedef struct {
    size_t width;
    volatile void *obj;
    void *expected;
    const void *desired;
} patomic_transaction_cmpxchg_t;


/*
 * TRANSACTION CONFIG
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
 * - dwfb: double (for double cmpxchg) with fallback
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
 * - success: the atomic operation was committed
 * - aborted: the atomic operation was not committed
 */

typedef enum {
    patomic_TSUCCESS = 0
    ,patomic_TABORTED = 1
} patomic_transaction_status_t;


/*
 * TRANSACTION RESULT
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
    patomic_transaction_status_t status;
    size_t attempts_made;
} patomic_transaction_result_t;

typedef struct {
    patomic_transaction_status_t status;
    patomic_transaction_status_t fallback_status;
    size_t attempts_made;
    size_t fallback_attempts_made;
} patomic_transaction_result_wfb_t;

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* !PATOMIC_TRANSACTION_H */
