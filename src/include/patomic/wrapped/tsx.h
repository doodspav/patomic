#ifndef PATOMIC_WRAPPED_TSX_H
#define PATOMIC_WRAPPED_TSX_H

#include "base.h"

#include <patomic/api/transaction.h>
#include <patomic/internal/transaction.h>

#include <stddef.h>


/**
 * @addtogroup wrapped.tsx
 *
 * @brief
 *   Defines a function which implements an atomic store operation using
 *   tbegin and tcommit as the underlying atomic transaction primitives.
 *
 * @details
 *   The defined function's signature will match
 *   patomic_opsig_transaction_store_t.
 *
 * @param fn_name
 *   The name of the function to be defined.
 *
 * @param tbegin
 *   A callable with the signature and semantics of
 *   patomic_opsig_transaction_tbegin_t.
 *
 * @param tcommit
 *   A callable with the signature and semantics of
 *   patomic_opsig_transaction_tcommit_t.
 */
#define PATOMIC_WRAPPED_TSX_DEFINE_OP_STORE(                                \
    fn_name, tbegin, tcommit                                                \
)                                                                           \
    static void                                                             \
    fn_name(                                                                \
        volatile void *const obj,                                           \
        const void *const desired,                                          \
        patomic_transaction_config_t config,                                \
        patomic_transaction_result_t *const result                          \
    )                                                                       \
    {                                                                       \
        /* declarations */                                                  \
        patomic_transaction_result_t res = {0};                             \
        patomic_transaction_padded_flag_holder_abi_unstable_t holder = {0}; \
        if (config.flag_nullable == NULL)                                   \
        {                                                                   \
            config.flag_nullable = &holder.flag;                            \
        }                                                                   \
                                                                            \
        /* assert result early */                                           \
        PATOMIC_WRAPPED_DO_ASSERT(result != NULL);                          \
                                                                            \
        /* check zero */                                                    \
        PATOMIC_WRAPPED_TSX_CHECK_CONFIG_ZERO(config, res, cleanup);        \
                                                                            \
        /* assertions */                                                    \
        PATOMIC_WRAPPED_DO_ASSERT(obj != NULL);                             \
        PATOMIC_WRAPPED_DO_ASSERT(desired != NULL);                         \
                                                                            \
        /* operation */                                                     \
        while (config.attempts-- > 0ul)                                     \
        {                                                                   \
            ++res.attempts_made;                                            \
            res.status = tbegin();                                          \
            if (res.status == 0ul)                                          \
            {                                                               \
                PATOMIC_IGNORE_UNUSED(*config.flag_nullable);               \
                PATOMIC_WRAPPED_DO_MEMCPY(                                  \
                    (void *) obj, desired, config.width                     \
                );                                                          \
                tcommit();                                                  \
                break;                                                      \
            }                                                               \
        }                                                                   \
                                                                            \
        /* cleanup */                                                       \
    cleanup:                                                                \
        *result = res;                                                      \
    }


/**
 * @addtogroup wrapped.tsx
 *
 * @brief
 *   Defines a function which implements an atomic load operation using
 *   tbegin and tcommit as the underlying atomic transaction primitives.
 *
 * @details
 *   The defined function's signature will match
 *   patomic_opsig_transaction_load_t.
 *
 * @param fn_name
 *   The name of the function to be defined.
 *
 * @param tbegin
 *   A callable with the signature and semantics of
 *   patomic_opsig_transaction_tbegin_t.
 *
 * @param tcommit
 *   A callable with the signature and semantics of
 *   patomic_opsig_transaction_tcommit_t.
 */
#define PATOMIC_WRAPPED_TSX_DEFINE_OP_LOAD(                                 \
    fn_name, tbegin, tcommit                                                \
)                                                                           \
    static void                                                             \
    fn_name(                                                                \
        const volatile void *const obj,                                     \
        void *const ret,                                                    \
        patomic_transaction_config_t config,                                \
        patomic_transaction_result_t *const result                          \
    )                                                                       \
    {                                                                       \
        /* declarations */                                                  \
        patomic_transaction_result_t res = {0};                             \
        patomic_transaction_padded_flag_holder_abi_unstable_t holder = {0}; \
        if (config.flag_nullable == NULL)                                   \
        {                                                                   \
            config.flag_nullable = &holder.flag;                            \
        }                                                                   \
                                                                            \
        /* assert result early */                                           \
        PATOMIC_WRAPPED_DO_ASSERT(result != NULL);                          \
                                                                            \
        /* check zero */                                                    \
        PATOMIC_WRAPPED_TSX_CHECK_CONFIG_ZERO(config, res, cleanup);        \
                                                                            \
        /* assertions */                                                    \
        PATOMIC_WRAPPED_DO_ASSERT(obj != NULL);                             \
        PATOMIC_WRAPPED_DO_ASSERT(ret != NULL);                             \
                                                                            \
        /* operation */                                                     \
        while (config.attempts-- > 0ul)                                     \
        {                                                                   \
            ++res.attempts_made;                                            \
            res.status = tbegin();                                          \
            if (res.status == 0ul)                                          \
            {                                                               \
                PATOMIC_IGNORE_UNUSED(*config.flag_nullable);               \
                PATOMIC_WRAPPED_DO_MEMCPY(                                  \
                    ret, (const void *) obj, config.width                   \
                );                                                          \
                tcommit();                                                  \
                break;                                                      \
            }                                                               \
        }                                                                   \
                                                                            \
        /* cleanup */                                                       \
    cleanup:                                                                \
        *result = res;                                                      \
    }


#endif  /* PATOMIC_WRAPPED_TSX_H */
