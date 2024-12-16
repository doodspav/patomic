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
#define PATOMIC_WRAPPED_TSX_DEFINE_OP_BIT_TEST(                          \
    fn_name, tbegin, tcommit                                             \
)                                                                        \
    static int                                                           \
    fn_name(                                                             \
        const volatile void *const obj,                                  \
        const int offset,                                                \
        patomic_transaction_config_t config,                             \
        patomic_transaction_result_t *const result                       \
    )                                                                    \
    {                                                                    \
        /* declarations */                                               \
        int bit = 0;                                                     \
        const unsigned char *const uc_obj = (const unsigned char *) obj; \
        const size_t byte_offset = ((size_t) offset) / CHAR_BIT;         \
        const int bit_offset = offset % ((int) CHAR_BIT);                \
        patomic_transaction_result_t res = {0};                          \
        const patomic_transaction_flag_t flag = 0;                       \
        if (config.flag_nullable == NULL)                                \
        {                                                                \
            config.flag_nullable = &flag;                                \
        }                                                                \
                                                                         \
        /* assert early */                                               \
        PATOMIC_WRAPPED_DO_ASSERT(result != NULL);                       \
                                                                         \
        /* check zero */                                                 \
        PATOMIC_WRAPPED_TSX_CHECK_CONFIG_ZERO(config, res, cleanup);     \
                                                                         \
        /* assertions */                                                 \
        PATOMIC_WRAPPED_DO_ASSERT(obj != NULL);                          \
        PATOMIC_WRAPPED_DO_ASSERT(byte_offset < config.widths);          \
                                                                         \
        /* operation */                                                  \
        while (config.attempts-- > 0ul)                                  \
        {                                                                \
            ++res.attempts_made;                                         \
            res.status = tbegin();                                       \
            if (res.status == 0ul)                                       \
            {                                                            \
                PATOMIC_IGNORE_UNUSED(*config.flag_nullable);            \
                bit = (int) ((uc_obj[byte_offset] >> bit_offset) & 1u);  \
                tcommit();                                               \
                goto cleanup;                                            \
            }                                                            \
        }                                                                \
                                                                         \
        /* cleanup */                                                    \
    cleanup:                                                             \
        *result = res;                                                   \
        return bit;                                                      \
    }


/**
 * @addtogroup wrapped.tsx
 *
 * @brief
 *   Defines a function which implements an atomic bit test-modify operation
 *   using tbegin and tcommit as the underlying atomic transaction primitives.
 *
 * @details
 *   The defined function's signature will match
 *   patomic_opsig_transaction_test_modify_t.
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
 *
 * @param do_bit_modify
 *   A callable with the signature
 *   'unsigned char(unsigned char byte, int offset)' which will evaluate to the
 *   byte with the bit at offset modified in the appropriate manner. The offset
 *   is guaranteed to be less than CHAR_BIT.
 *
 */
#define PATOMIC_WRAPPED_TSX_DEFINE_OP_BIT_TEST_MODIFY(                  \
    fn_name, tbegin, tcommit, do_bit_modify                             \
)                                                                       \
    static int                                                          \
    fn_name(                                                            \
        volatile void *const obj,                                       \
        const int offset,                                               \
        patomic_transaction_config_t config,                            \
        patomic_transaction_result_t *const result                      \
    )                                                                   \
    {                                                                   \
        /* declarations */                                              \
        int bit = 0;                                                    \
        unsigned char *const uc_obj = (unsigned char *) obj;            \
        const size_t byte_offset = ((size_t) offset) / CHAR_BIT;        \
        const int bit_offset = offset % ((int) CHAR_BIT);               \
        patomic_transaction_result_t res = {0};                         \
        const patomic_transaction_flag_t flag = 0;                      \
        if (config.flag_nullable == NULL)                               \
        {                                                               \
            config.flag_nullable = &flag;                               \
        }                                                               \
                                                                        \
        /* assert early */                                              \
        PATOMIC_WRAPPED_DO_ASSERT(result != NULL);                      \
                                                                        \
        /* check zero */                                                \
        PATOMIC_WRAPPED_TSX_CHECK_CONFIG_ZERO(config, res, cleanup);    \
                                                                        \
        /* assertions */                                                \
        PATOMIC_WRAPPED_DO_ASSERT(obj != NULL);                         \
        PATOMIC_WRAPPED_DO_ASSERT(byte_offset < config.widths);         \
                                                                        \
        /* operation */                                                 \
        while (config.attempts-- > 0ul)                                 \
        {                                                               \
            ++res.attempts_made;                                        \
            res.status = tbegin();                                      \
            if (res.status == 0ul)                                      \
            {                                                           \
                PATOMIC_IGNORE_UNUSED(*config.flag_nullable);           \
                bit = (int) ((uc_obj[byte_offset] >> bit_offset) & 1u); \
                uc_obj[byte_offset] = do_bit_modify(                    \
                    uc_obj[byte_offset], bit_offset                     \
                );                                                      \
                tcommit();                                              \
                goto cleanup;                                           \
            }                                                           \
        }                                                               \
                                                                        \
        /* cleanup */                                                   \
    cleanup:                                                            \
        *result = res;                                                  \
        return bit;                                                     \
    }


// internal helper macros for BIT_MODIFY
#define patomic_wrapped_tsx_do_bit_compl(byte, offset) \
    ((unsigned char) ((byte) ^ ((unsigned char) (1u << (offset)))))
#define patomic_wrapped_tsx_do_bit_set(byte, offset) \
    ((unsigned char) ((byte) | ((unsigned char) (1u << (offset)))))
#define patomic_wrapped_tsx_do_bit_reset(byte, offset) \
    ((unsigned char) ((byte) & ((unsigned char) (~((unsigned char) (1u << (offset)))))))


/**
 * @addtogroup wrapped.tsx
 *
 * @brief
 *   Defines a function which implements an atomic bit test_compl operation
 *   using tbegin and tcommit as the underlying atomic transaction primitives.
 *
 * @details
 *   The defined function's signature will match
 *   patomic_opsig_transaction_test_modify_t.
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
#define PATOMIC_WRAPPED_TSX_DEFINE_OP_BIT_TEST_COMPL( \
    fn_name, tbegin, tcommit                          \
)                                                     \
    PATOMIC_WRAPPED_TSX_DEFINE_OP_BIT_TEST_MODIFY(    \
        fn_name, tbegin, tcommit,                     \
        patomic_wrapped_tsx_do_bit_compl              \
    )


/**
 * @addtogroup wrapped.tsx
 *
 * @brief
 *   Defines a function which implements an atomic bit test_set operation
 *   using tbegin and tcommit as the underlying atomic transaction primitives.
 *
 * @details
 *   The defined function's signature will match
 *   patomic_opsig_transaction_test_modify_t.
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
#define PATOMIC_WRAPPED_TSX_DEFINE_OP_BIT_TEST_SET( \
    fn_name, tbegin, tcommit                        \
)                                                   \
    PATOMIC_WRAPPED_TSX_DEFINE_OP_BIT_TEST_MODIFY(  \
        fn_name, tbegin, tcommit,                   \
        patomic_wrapped_tsx_do_bit_set              \
    )


/**
 * @addtogroup wrapped.tsx
 *
 * @brief
 *   Defines a function which implements an atomic bit test_reset operation
 *   using tbegin and tcommit as the underlying atomic transaction primitives.
 *
 * @details
 *   The defined function's signature will match
 *   patomic_opsig_transaction_test_modify_t.
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
#define PATOMIC_WRAPPED_TSX_DEFINE_OP_BIT_TEST_RESET( \
    fn_name, tbegin, tcommit                          \
)                                                     \
    PATOMIC_WRAPPED_TSX_DEFINE_OP_BIT_TEST_MODIFY(    \
        fn_name, tbegin, tcommit,                     \
        patomic_wrapped_tsx_do_bit_reset              \
    )


/**
 * @addtogroup wrapped.tsx
 *
 * @brief
 *   Defines a function which implements an atomic fetch operation using
 *   tbegin and tcommit as the underlying atomic transaction primitives.
 *
 * @details
 *   The defined function's signature will match
 *   patomic_opsig_transaction_fetch_t.
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
 *
 * @param do_op
 *   A callable with the signature
 *   'void(unsigned char *obj, const unsigned char *arg, size_t width)' which
 *   will perform the desired operation.
 */
#define PATOMIC_WRAPPED_TSX_DEFINE_OP_FETCH(                             \
    fn_name, tbegin, tcommit, do_op                                      \
)                                                                        \
    static void                                                          \
    fn_name(                                                             \
        volatile void *const obj,                                        \
        const void *const arg,                                           \
        void *const ret,                                                 \
        patomic_transaction_config_t config,                             \
        patomic_transaction_result *const result                         \
    )                                                                    \
    {                                                                    \
        /* declarations */                                               \
        unsigned char *const uc_obj = (unsigned char *) obj;             \
        const unsigned char *const uc_arg = (const unsigned char *) arg; \
        patomic_transaction_result_t res = {0};                          \
        const patomic_transaction_flag_t flag = 0;                       \
        if (config.flag_nullable == NULL)                                \
        {                                                                \
            config.flag_nullable = &flag;                                \
        }                                                                \
                                                                         \
        /* assert early */                                               \
        PATOMIC_WRAPPED_DO_ASSERT(result != NULL);                       \
                                                                         \
        /* check zero */                                                 \
        PATOMIC_WRAPPED_TSX_CHECK_CONFIG_ZERO(config, res, cleanup);     \
                                                                         \
        /* assertions */                                                 \
        PATOMIC_WRAPPED_DO_ASSERT(obj != NULL);                          \
        PATOMIC_WRAPPED_DO_ASSERT(arg != NULL);                          \
        PATOMIC_WRAPPED_DO_ASSERT(ret != NULL);                          \
                                                                         \
        /* operation */                                                  \
        while (config.attempts-- > 0ul)                                  \
        {                                                                \
            ++res.attempts_made;                                         \
            res.status = tbegin();                                       \
            if (res.status == 0ul)                                       \
            {                                                            \
                PATOMIC_IGNORE_UNUSED(*config.flag_nullable);            \
                PATOMIC_WRAPPED_DO_MEMCPY(                               \
                    ret, (const void *) obj, config.width                \
                );                                                       \
                do_op(uc_obj, uc_arg, config.width);                     \
                tcommit();                                               \
                goto cleanup;                                            \
            }                                                            \
        }                                                                \
                                                                         \
        /* cleanup */                                                    \
    cleanup:                                                             \
        *result = res;                                                   \
    }


/**
 * @addtogroup wrapped.tsx
 *
 * @brief
 *   Defines a function which implements an atomic fetch_noarg operation using
 *   tbegin and tcommit as the underlying atomic transaction primitives.
 *
 * @details
 *   The defined function's signature will match
 *   patomic_opsig_transaction_fetch_noarg_t.
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
 *
 * @param do_op
 *   A callable with the signature
 *   'void(unsigned char *obj, size_t width)' which will perform the desired
 *   operation.
 */
#define PATOMIC_WRAPPED_TSX_DEFINE_OP_FETCH_NOARG(                       \
    fn_name, tbegin, tcommit, do_op                                      \
)                                                                        \
    static void                                                          \
    fn_name(                                                             \
        volatile void *const obj,                                        \
        void *const ret,                                                 \
        patomic_transaction_config_t config,                             \
        patomic_transaction_result *const result                         \
    )                                                                    \
    {                                                                    \
        /* declarations */                                               \
        unsigned char *const uc_obj = (unsigned char *) obj;             \
        patomic_transaction_result_t res = {0};                          \
        const patomic_transaction_flag_t flag = 0;                       \
        if (config.flag_nullable == NULL)                                \
        {                                                                \
            config.flag_nullable = &flag;                                \
        }                                                                \
                                                                         \
        /* assert early */                                               \
        PATOMIC_WRAPPED_DO_ASSERT(result != NULL);                       \
                                                                         \
        /* check zero */                                                 \
        PATOMIC_WRAPPED_TSX_CHECK_CONFIG_ZERO(config, res, cleanup);     \
                                                                         \
        /* assertions */                                                 \
        PATOMIC_WRAPPED_DO_ASSERT(obj != NULL);                          \
        PATOMIC_WRAPPED_DO_ASSERT(arg != NULL);                          \
        PATOMIC_WRAPPED_DO_ASSERT(ret != NULL);                          \
                                                                         \
        /* operation */                                                  \
        while (config.attempts-- > 0ul)                                  \
        {                                                                \
            ++res.attempts_made;                                         \
            res.status = tbegin();                                       \
            if (res.status == 0ul)                                       \
            {                                                            \
                PATOMIC_IGNORE_UNUSED(*config.flag_nullable);            \
                PATOMIC_WRAPPED_DO_MEMCPY(                               \
                    ret, (const void *) obj, config.width                \
                );                                                       \
                do_op(uc_obj, config.width);                             \
                tcommit();                                               \
                goto cleanup;                                            \
            }                                                            \
        }                                                                \
                                                                         \
        /* cleanup */                                                    \
    cleanup:                                                             \
        *result = res;                                                   \
    }


#endif  /* PATOMIC_WRAPPED_TSX_H */
