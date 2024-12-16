#ifndef PATOMIC_WRAPPED_TSX_H
#define PATOMIC_WRAPPED_TSX_H

#include "base.h"


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
#define PATOMIC_WRAPPED_TSX_DEFINE_OP_STORE(                         \
    fn_name, tbegin, tcommit                                         \
)                                                                    \
    static void                                                      \
    fn_name(                                                         \
        volatile void *const obj,                                    \
        const void *const desired,                                   \
        patomic_transaction_config_t config,                         \
        patomic_transaction_result_t *const result                   \
    )                                                                \
    {                                                                \
        /* declarations */                                           \
        patomic_transaction_result_t res = {0};                      \
        const patomic_transaction_flag_t flag = 0;                   \
        if (config.flag_nullable == NULL)                            \
        {                                                            \
            config.flag_nullable = &flag;                            \
        }                                                            \
                                                                     \
        /* assert early */                                           \
        PATOMIC_WRAPPED_DO_ASSERT(result != NULL);                   \
                                                                     \
        /* check zero */                                             \
        PATOMIC_WRAPPED_TSX_CHECK_CONFIG_ZERO(config, res, cleanup); \
                                                                     \
        /* assertions */                                             \
        PATOMIC_WRAPPED_DO_ASSERT(obj != NULL);                      \
        PATOMIC_WRAPPED_DO_ASSERT(desired != NULL);                  \
                                                                     \
        /* operation */                                              \
        while (config.attempts-- > 0ul)                              \
        {                                                            \
            ++res.attempts_made;                                     \
            res.status = tbegin();                                   \
            if (res.status == 0ul)                                   \
            {                                                        \
                PATOMIC_IGNORE_UNUSED(*config.flag_nullable);        \
                PATOMIC_WRAPPED_DO_MEMCPY(                           \
                    (void *) obj, desired, config.width              \
                );                                                   \
                tcommit();                                           \
                goto cleanup;                                        \
            }                                                        \
        }                                                            \
                                                                     \
        /* cleanup */                                                \
    cleanup:                                                         \
        *result = res;                                               \
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
#define PATOMIC_WRAPPED_TSX_DEFINE_OP_LOAD(                          \
    fn_name, tbegin, tcommit                                         \
)                                                                    \
    static void                                                      \
    fn_name(                                                         \
        const volatile void *const obj,                              \
        void *const ret,                                             \
        patomic_transaction_config_t config,                         \
        patomic_transaction_result_t *const result                   \
    )                                                                \
    {                                                                \
        /* declarations */                                           \
        patomic_transaction_result_t res = {0};                      \
        const patomic_transaction_flag_t flag = 0;                   \
        if (config.flag_nullable == NULL)                            \
        {                                                            \
            config.flag_nullable = &flag;                            \
        }                                                            \
                                                                     \
        /* assert early */                                           \
        PATOMIC_WRAPPED_DO_ASSERT(result != NULL);                   \
                                                                     \
        /* check zero */                                             \
        PATOMIC_WRAPPED_TSX_CHECK_CONFIG_ZERO(config, res, cleanup); \
                                                                     \
        /* assertions */                                             \
        PATOMIC_WRAPPED_DO_ASSERT(obj != NULL);                      \
        PATOMIC_WRAPPED_DO_ASSERT(ret != NULL);                      \
                                                                     \
        /* operation */                                              \
        while (config.attempts-- > 0ul)                              \
        {                                                            \
            ++res.attempts_made;                                     \
            res.status = tbegin();                                   \
            if (res.status == 0ul)                                   \
            {                                                        \
                PATOMIC_IGNORE_UNUSED(*config.flag_nullable);        \
                PATOMIC_WRAPPED_DO_MEMCPY(                           \
                    ret, (const void *) obj, config.width            \
                );                                                   \
                tcommit();                                           \
                goto cleanup;                                        \
            }                                                        \
        }                                                            \
                                                                     \
        /* cleanup */                                                \
    cleanup:                                                         \
        *result = res;                                               \
    }


/**
 * @addtogroup wrapped.tsx
 *
 * @brief
 *   Defines a function which implements an atomic exchange operation using
 *   tbegin and tcommit as the underlying atomic transaction primitives.
 *
 * @details
 *   The defined function's signature will match
 *   patomic_opsig_transaction_exchange_t.
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
#define PATOMIC_WRAPPED_TSX_DEFINE_OP_EXCHANGE(                      \
    fn_name, tbegin, tcommit                                         \
)                                                                    \
    static void                                                      \
    fn_name(                                                         \
        volatile void *const obj,                                    \
        const void *const desired,                                   \
        void *const ret,                                             \
        patomic_transaction_config_t config,                         \
        patomic_transaction_result_t *const result                   \
    )                                                                \
    {                                                                \
        /* declarations */                                           \
        patomic_transaction_result_t res = {0};                      \
        const patomic_transaction_flag_t flag = 0;                   \
        if (config.flag_nullable == NULL)                            \
        {                                                            \
            config.flag_nullable = &flag;                            \
        }                                                            \
                                                                     \
        /* assert early */                                           \
        PATOMIC_WRAPPED_DO_ASSERT(result != NULL);                   \
                                                                     \
        /* check zero */                                             \
        PATOMIC_WRAPPED_TSX_CHECK_CONFIG_ZERO(config, res, cleanup); \
                                                                     \
        /* assertions */                                             \
        PATOMIC_WRAPPED_DO_ASSERT(obj != NULL);                      \
        PATOMIC_WRAPPED_DO_ASSERT(desired != NULL);                  \
        PATOMIC_WRAPPED_DO_ASSERT(ret != NULL);                      \
                                                                     \
        /* operation */                                              \
        while (config.attempts-- > 0ul)                              \
        {                                                            \
            ++res.attempts_made;                                     \
            res.status = tbegin();                                   \
            if (res.status == 0ul)                                   \
            {                                                        \
                PATOMIC_IGNORE_UNUSED(*config.flag_nullable);        \
                PATOMIC_WRAPPED_DO_MEMCPY(                           \
                    ret, (const void *) obj, config.width            \
                );                                                   \
                PATOMIC_WRAPPED_DO_MEMCPY(                           \
                    (void *) obj, desired, config.width              \
                );                                                   \
                tcommit();                                           \
                goto cleanup;                                        \
            }                                                        \
        }                                                            \
                                                                     \
        /* cleanup */                                                \
    cleanup:                                                         \
        *result = res;                                               \
    }


/**
 * @addtogroup wrapped.tsx
 *
 * @brief
 *   Defines a function which implements an atomic cmpxchg_weak operation using
 *   tbegin and tcommit as the underlying atomic transaction primitives.
 *
 * @details
 *   The defined function's signature will match
 *   patomic_opsig_transaction_cmpxchg_t.
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
#define PATOMIC_WRAPPED_TSX_DEFINE_OP_CMPXCHG_WEAK(                    \
    fn_name, tbegin, tcommit                                           \
)                                                                      \
    static int                                                         \
    fn_name(                                                           \
        volatile void *const obj,                                      \
        void *const expected,                                          \
        const void *const desired,                                     \
        patomic_transaction_config_wfb_t config,                       \
        patomic_transaction_result_wfb_t *const result                 \
    )                                                                  \
    {                                                                  \
        /* declarations */                                             \
        patomic_transaction_result_wfb_t res = {0};                    \
        const patomic_transaction_flag_t flag = 0;                     \
        if (config.flag_nullable == NULL)                              \
        {                                                              \
            config.flag_nullable = &flag;                              \
        }                                                              \
        if (config.fallback_flag_nullable == NULL)                     \
        {                                                              \
            config.fallback_flag_nullable = &flag;                     \
        }                                                              \
                                                                       \
        /* assert early */                                             \
        PATOMIC_WRAPPED_DO_ASSERT(result != NULL);                     \
                                                                       \
        /* check zero */                                               \
        PATOMIC_WRAPPED_TSX_CHECK_CONFIG_ZERO_WFB(                     \
            config, res, fallback, cleanup                             \
        );                                                             \
                                                                       \
        /* assertions */                                               \
        PATOMIC_WRAPPED_DO_ASSERT(obj != NULL);                        \
        PATOMIC_WRAPPED_DO_ASSERT(expected != NULL);                   \
        PATOMIC_WRAPPED_DO_ASSERT(desired != NULL);                    \
                                                                       \
        /* operation */                                                \
        while (config.attempts-- > 0ul)                                \
        {                                                              \
            int cmp = 0;                                               \
            ++res.attempts_made;                                       \
            res.status = tbegin();                                     \
            if (res.status == 0ul)                                     \
            {                                                          \
                PATOMIC_IGNORE_UNUSED(*config.flag_nullable);          \
                cmp = memcmp(                                          \
                    (const void *) obj, expected, config.width         \
                );                                                     \
                if (cmp == 0)                                          \
                {                                                      \
                    PATOMIC_WRAPPED_DO_MEMCPY(                         \
                        (void *) obj, desired, config.width            \
                    );                                                 \
                }                                                      \
                tcommit();                                             \
                if (cmp != 0)                                          \
                {                                                      \
                    goto fallback;                                     \
                }                                                      \
                goto cleanup;                                          \
            }                                                          \
        }                                                              \
                                                                       \
        /* fallback */                                                 \
    fallback:                                                          \
                                                                       \
        /* check zero */                                               \
        PATOMIC_WRAPPED_TSX_CHECK_CONFIG_ZERO_FALLBACK(                \
            config, res, cleanup                                       \
        );                                                             \
                                                                       \
        /* assertions */                                               \
        PATOMIC_WRAPPED_DO_ASSERT(obj != NULL);                        \
        PATOMIC_WRAPPED_DO_ASSERT(ret != NULL);                        \
                                                                       \
        /* operation */                                                \
        while (config.fallback_attempts-- > 0ul)                       \
        {                                                              \
            ++res.fallback_attempts_made;                              \
            res.fallback_status = tbegin();                            \
            if (res.fallback_status == 0ul)                            \
            {                                                          \
                PATOMIC_IGNORE_UNUSED(*config.fallback_flag_nullable); \
                PATOMIC_WRAPPED_DO_MEMCPY(                             \
                    expected, (const void *) obj, config.width         \
                );                                                     \
                tcommit();                                             \
                goto cleanup;                                          \
            }                                                          \
        }                                                              \
                                                                       \
        /* cleanup */                                                  \
    cleanup:                                                           \
        *result = res;                                                 \
        return res.status == 0ul;                                      \
    }


/**
 * @addtogroup wrapped.tsx
 *
 * @brief
 *   Defines a function which implements an atomic bit test operation using
 *   tbegin and tcommit as the underlying atomic transaction primitives.
 *
 * @details
 *   The defined function's signature will match
 *   patomic_opsig_transaction_test_t.
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
#define PATOMIC_WRAPPED_TSX_DEFINE_OP_BIT_TEST(                             \
    fn_name, tbegin, tcommit                                                \
)                                                                           \
    static int                                                              \
    fn_name(                                                                \
        const volatile void *const obj,                                     \
        int offset,                                                         \
        patomic_transaction_config_t config,                                \
        patomic_transaction_result_t *const result                          \
    )                                                                       \
    {                                                                       \
        /* declarations */                                                  \
        int bit = 0;                                                        \
        const unsigned char *const uc_obj = (const unsigned char *) obj;    \
        const size_t byte_offset = (size_t) (offset / 8);                   \
        const int bit_offset = offset % 8;                                  \
        patomic_transaction_result_t res = {0};                             \
        const patomic_transaction_flag_t flag = 0;                          \
        if (config.flag_nullable == NULL)                                   \
        {                                                                   \
            config.flag_nullable = &flag;                                   \
        }                                                                   \
                                                                            \
        /* assert early */                                                  \
        PATOMIC_WRAPPED_DO_ASSERT(result != NULL);                          \
        PATOMIC_WRAPPED_DO_ASSERT(byte_offset < config.widths);             \
                                                                            \
        /* check zero */                                                    \
        PATOMIC_WRAPPED_TSX_CHECK_CONFIG_ZERO(config, res, cleanup);        \
                                                                            \
        /* assertions */                                                    \
        PATOMIC_WRAPPED_DO_ASSERT(obj != NULL);                             \
                                                                            \
        /* operation */                                                     \
        while (config.attempts-- > 0ul)                                     \
        {                                                                   \
            ++res.attempts_made;                                            \
            res.status = tbegin();                                          \
            if (res.status == 0ul)                                          \
            {                                                               \
                PATOMIC_IGNORE_UNUSED(*config.flag_nullable);               \
                bit = (int) ((uc_obj[byte_offset] >> bit_offset) & 1u);     \
                tcommit();                                                  \
                goto cleanup;                                               \
            }                                                               \
        }                                                                   \
                                                                            \
        /* cleanup */                                                       \
    cleanup:                                                                \
        *result = res;                                                      \
        return bit;                                                         \
    }


#endif  /* PATOMIC_WRAPPED_TSX_H */
