#ifndef PATOMIC_TRANSACTION_H
#define PATOMIC_TRANSACTION_H

#include <stddef.h>
#include <patomic/types/align.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * TRANSACTION FLAG
 * - used to cause an abort in a live transaction if modified
 * - both flag ops (set, reset) are lock-free atomic (like all ops)
 *
 * - padded_flag_holder is available for C90/99 since no alignment utilities
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

typedef void (* patomic_transaction_flag_set_t) \
    (patomic_transaction_flag_t *);

typedef void (* patomic_transaction_flag_reset_t) \
    (patomic_transaction_flag_t *);

typedef struct {
    patomic_transaction_flag_set_t fp_set;
    patomic_transaction_flag_reset_t fp_reset;
} patomic_transaction_flag_ops_t;


/*
 * TRANSACTION CONFIG#
 * - width: size in bytes of objects to operate on
 * - attempts: number of attempts to make committing atomic transaction
 * - fallback_attempts: number of attempts to make commit fallback atomic
 *   transaction
 * - flag: read from at the start of atomic transaction
 * - fallback_flag: read from at the start of fallback atomic transaction
 *
 * - flag and fallback_flag may point to the same object, or be NULL (in which
 *   case a locally allocated flag is used)
 */

typedef struct {
    size_t width;
    size_t attempts;
    size_t fallback_attempts;
    const patomic_transaction_flag_t *flag;
    const patomic_transaction_flag_t *fallback_flag;
};

/*
 * TRANSACTION STATUS
 * - success: the atomic operation was committed
 * - failure: the fallback atomic operation was committed
 * - aborted: no atomic operation was committed
 */

typedef enum {
    patomic_transaction_SUCCESS,
    patomic_transaction_FAILURE,
    patomic_transaction_ABORTED
} patomic_transaction_status_t;


/*
 * TRANSACTION RESULT
 * - status: see above
 * - attempts_made: how many attempts were made to commit the atomic operation
 * - fallback_attempts_made: how many attempts were made to commit the fallback
 *   atomic operation
 *
 * - the (fallback_)attempts members can be used to determine if the memory
 *   region being operated on is under a lot of contention (this can cause
 *   repeated transaction aborts)
 *
 * - if status is success, fallback_attempts_made will be 0
 * - if status is failure, attempts_made will equal the attempts value from the
 *   config
 * - if status is aborted, attempts_made and fallback_attempts_made will be
 *   equal to their respective config values
 */

typedef struct {
    patomic_transaction_status_t status;
    size_t attempts_made;
    size_t fallback_attempts_made;
} patomic_transaction_result_t;

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* !PATOMIC_TRANSACTION_H */
