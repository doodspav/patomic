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
            ++res.attempts_made;                                       \
            res.status = tbegin();                                     \
            if (res.status == 0ul)                                     \
            {                                                          \
                int cmp;                                               \
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
        PATOMIC_WRAPPED_DO_ASSERT(expected != NULL);                   \
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
#define PATOMIC_WRAPPED_TSX_DEFINE_OP_FETCH_NOARG(                   \
    fn_name, tbegin, tcommit, do_op                                  \
)                                                                    \
    static void                                                      \
    fn_name(                                                         \
        volatile void *const obj,                                    \
        void *const ret,                                             \
        patomic_transaction_config_t config,                         \
        patomic_transaction_result *const result                     \
    )                                                                \
    {                                                                \
        /* declarations */                                           \
        unsigned char *const uc_obj = (unsigned char *) obj;         \
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
                do_op(uc_obj, config.width);                         \
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
 *   Defines a function which implements an atomic void operation using
 *   tbegin and tcommit as the underlying atomic transaction primitives.
 *
 * @details
 *   The defined function's signature will match
 *   patomic_opsig_transaction_void_t.
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
#define PATOMIC_WRAPPED_TSX_DEFINE_OP_VOID(                              \
    fn_name, tbegin, tcommit, do_op                                      \
)                                                                        \
    static void                                                          \
    fn_name(                                                             \
        volatile void *const obj,                                        \
        const void *const arg,                                           \
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
                                                                         \
        /* operation */                                                  \
        while (config.attempts-- > 0ul)                                  \
        {                                                                \
            ++res.attempts_made;                                         \
            res.status = tbegin();                                       \
            if (res.status == 0ul)                                       \
            {                                                            \
                PATOMIC_IGNORE_UNUSED(*config.flag_nullable);            \
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
 *   Defines a function which implements an atomic void_noarg operation using
 *   tbegin and tcommit as the underlying atomic transaction primitives.
 *
 * @details
 *   The defined function's signature will match
 *   patomic_opsig_transaction_void_noarg_t.
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
#define PATOMIC_WRAPPED_TSX_DEFINE_OP_VOID_NOARG(                    \
    fn_name, tbegin, tcommit, do_op                                  \
)                                                                    \
    static void                                                      \
    fn_name(                                                         \
        volatile void *const obj,                                    \
        patomic_transaction_config_t config,                         \
        patomic_transaction_result *const result                     \
    )                                                                \
    {                                                                \
        /* declarations */                                           \
        unsigned char *const uc_obj = (unsigned char *) obj;         \
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
                                                                     \
        /* operation */                                              \
        while (config.attempts-- > 0ul)                              \
        {                                                            \
            ++res.attempts_made;                                     \
            res.status = tbegin();                                   \
            if (res.status == 0ul)                                   \
            {                                                        \
                PATOMIC_IGNORE_UNUSED(*config.flag_nullable);        \
                do_op(uc_obj, config.width);                         \
                tcommit();                                           \
                goto cleanup;                                        \
            }                                                        \
        }                                                            \
                                                                     \
        /* cleanup */                                                \
    cleanup:                                                         \
        *result = res;                                               \
    }


// internal helper macros for binary FETCH/VOID(_NOARG)
#define patomic_wrapped_tsx_do_bin_or(obj, arg, width) \
    do {                                               \
        for (size_t i = 0; i < (width); ++i) {         \
            (obj)[i] |= (arg)[i];                      \
        }                                              \
    } while (0)
#define patomic_wrapped_tsx_do_bin_xor(obj, arg, width) \
    do {                                                \
        for (size_t i = 0; i < (width); ++i) {          \
            (obj)[i] ^= (arg)[i];                       \
        }                                               \
    } while (0)
#define patomic_wrapped_tsx_do_bin_and(obj, arg, width) \
    do {                                                \
        for (size_t i = 0; i < (width); ++i) {          \
            (obj)[i] &= (arg)[i];                       \
        }                                               \
    } while (0)
#define patomic_wrapped_tsx_do_bin_not(obj, width)  \
    do {                                            \
        for (size_t i = 0; i < (width); ++i) {      \
            (obj)[i] = (unsigned char) ~((obj)[i]); \
        }                                           \
    } while (0)


/**
 * @addtogroup wrapped.tsx
 *
 * @brief
 *   Defines a function which implements an atomic fetch_or operation using
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
 */
#define PATOMIC_WRAPPED_TSX_DEFINE_OP_FETCH_OR( \
    fn_name, tbegin, tcommit                    \
)                                               \
    PATOMIC_WRAPPED_TSX_DEFINE_OP_FETCH(        \
        fn_name, tbegin, tcommit,               \
        patomic_wrapped_tsx_do_bin_or           \
    )


/**
 * @addtogroup wrapped.tsx
 *
 * @brief
 *   Defines a function which implements an atomic fetch_xor operation using
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
 */
#define PATOMIC_WRAPPED_TSX_DEFINE_OP_FETCH_XOR( \
    fn_name, tbegin, tcommit                     \
)                                                \
    PATOMIC_WRAPPED_TSX_DEFINE_OP_FETCH(         \
        fn_name, tbegin, tcommit,                \
        patomic_wrapped_tsx_do_bin_xor           \
    )


/**
 * @addtogroup wrapped.tsx
 *
 * @brief
 *   Defines a function which implements an atomic fetch_and operation using
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
 */
#define PATOMIC_WRAPPED_TSX_DEFINE_OP_FETCH_AND( \
    fn_name, tbegin, tcommit                     \
)                                                \
    PATOMIC_WRAPPED_TSX_DEFINE_OP_FETCH(         \
        fn_name, tbegin, tcommit,                \
        patomic_wrapped_tsx_do_bin_and           \
    )


/**
 * @addtogroup wrapped.tsx
 *
 * @brief
 *   Defines a function which implements an atomic fetch_not operation using
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
 */
#define PATOMIC_WRAPPED_TSX_DEFINE_OP_FETCH_NOT( \
    fn_name, tbegin, tcommit                     \
)                                                \
    PATOMIC_WRAPPED_TSX_DEFINE_OP_FETCH_NOARG(   \
        fn_name, tbegin, tcommit,                \
        patomic_wrapped_tsx_do_bin_not           \
    )


/**
 * @addtogroup wrapped.tsx
 *
 * @brief
 *   Defines a function which implements an atomic or operation using
 *   tbegin and tcommit as the underlying atomic transaction primitives.
 *
 * @details
 *   The defined function's signature will match
 *   patomic_opsig_transaction_void_t.
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
#define PATOMIC_WRAPPED_TSX_DEFINE_OP_OR( \
    fn_name, tbegin, tcommit              \
)                                         \
    PATOMIC_WRAPPED_TSX_DEFINE_OP_VOID(   \
        fn_name, tbegin, tcommit,         \
        patomic_wrapped_tsx_do_bin_or     \
    )


/**
 * @addtogroup wrapped.tsx
 *
 * @brief
 *   Defines a function which implements an atomic xor operation using
 *   tbegin and tcommit as the underlying atomic transaction primitives.
 *
 * @details
 *   The defined function's signature will match
 *   patomic_opsig_transaction_void_t.
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
#define PATOMIC_WRAPPED_TSX_DEFINE_OP_XOR( \
    fn_name, tbegin, tcommit               \
)                                          \
    PATOMIC_WRAPPED_TSX_DEFINE_OP_VOID(    \
        fn_name, tbegin, tcommit,          \
        patomic_wrapped_tsx_do_bin_xor     \
    )


/**
 * @addtogroup wrapped.tsx
 *
 * @brief
 *   Defines a function which implements an atomic and operation using
 *   tbegin and tcommit as the underlying atomic transaction primitives.
 *
 * @details
 *   The defined function's signature will match
 *   patomic_opsig_transaction_void_t.
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
#define PATOMIC_WRAPPED_TSX_DEFINE_OP_AND( \
    fn_name, tbegin, tcommit               \
)                                          \
    PATOMIC_WRAPPED_TSX_DEFINE_OP_VOID(    \
        fn_name, tbegin, tcommit,          \
        patomic_wrapped_tsx_do_bin_and     \
    )


/**
 * @addtogroup wrapped.tsx
 *
 * @brief
 *   Defines a function which implements an atomic not operation using
 *   tbegin and tcommit as the underlying atomic transaction primitives.
 *
 * @details
 *   The defined function's signature will match
 *   patomic_opsig_transaction_void_noarg_t.
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
#define PATOMIC_WRAPPED_TSX_DEFINE_OP_NOT(    \
    fn_name, tbegin, tcommit                  \
)                                             \
    PATOMIC_WRAPPED_TSX_DEFINE_OP_VOID_NOARG( \
        fn_name, tbegin, tcommit,             \
        patomic_wrapped_tsx_do_bin_not        \
    )


// internal helper macros for arithmetic FETCH/VOID(_NOARG)
#define patomic_wrapped_tsx_do_add(obj, arg, width)                          \
    do {                                                                     \
        /* static assert */                                                  \
        PATOMIC_STATIC_ASSERT(                                               \
            ulong_big_enough,                                                \
            (ULONG_MAX / 2ul) > ((unsigned long) UCHAR_MAX)                  \
        );                                                                   \
        /* setup */                                                          \
        unsigned long carry = 0ul;                                           \
        const int is_le = IMPL_DEFINED_IS_LE();                              \
        for (size_t i = 0; i < (width); ++i) {                               \
            const size_t idx = (is_le) ? i : ((size_t) ((width) - 1ul - i)); \
            /* sum: obj[i] + arg[i] + carry */                               \
            unsigned long sum = (unsigned long) ((obj)[idx]);                \
            sum += (unsigned long) ((arg)[idx]);                             \
            sum += carry;                                                    \
            /* do add */                                                     \
            (obj)[idx] = (unsigned char) (sum & 0xFFu);                      \
            carry = (sum >> 8ul);                                            \
        }                                                                    \
    } while (0)
#define patomic_wrapped_tsx_do_sub(obj, arg, width)                          \
    do {                                                                     \
        /* static assert */                                                  \
        PATOMIC_STATIC_ASSERT(                                               \
            long_big_enough,                                                 \
            (LONG_MAX / 2l) > ((long) UCHAR_MAX)                             \
        );                                                                   \
        /* setup */                                                          \
        long borrow = 0l;                                                    \
        const int is_le = IMPL_DEFINED_IS_LE();                              \
        for (size_t i = 0; i < (width); ++i) {                               \
            const size_t idx = (is_le) ? i : ((size_t) ((width) - 1ul - i)); \
            /* diff: obj[i] - arg[i] - borrow */                             \
            long diff = (long) ((obj)[idx]);                                 \
            diff -= (long) ((arg)[idx]);                                     \
            diff -= borrow;                                                  \
            /* check for negative */                                         \
            if (diff < 0l) {                                                 \
                diff += 256l;                                                \
                borrow = 1l;                                                 \
            } else {                                                         \
                borrow = 0l;                                                 \
            }                                                                \
            /* do sub */                                                     \
            (obj)[idx] = (unsigned char) (diff & 0xFFl);                     \
        }                                                                    \
    } while (0)
#define patomic_wrapped_tsx_do_inc(obj, width)                               \
    do {                                                                     \
        /* static assert */                                                  \
        PATOMIC_STATIC_ASSERT(                                               \
            ulong_big_enough,                                                \
            (ULONG_MAX / 2ul) > ((unsigned long) UCHAR_MAX)                  \
        );                                                                   \
        /* setup */                                                          \
        unsigned long carry = 1ul;                                           \
        const int is_le = IMPL_DEFINED_IS_LE();                              \
        for (size_t i = 0; i < (width); ++i) {                               \
            const size_t idx = (is_le) ? i : ((size_t) ((width) - 1ul - i)); \
            /* sum: obj[i] + carry */                                        \
            unsigned long sum = (unsigned long) ((obj)[idx]);                \
            sum += carry;                                                    \
            /* do inc */                                                     \
            (obj)[idx] = (unsigned char) (sum & 0xFFu);                      \
            carry = (sum >> 8ul);                                            \
        }                                                                    \
    } while (0)
#define patomic_wrapped_tsx_do_dec(obj, width)                               \
    do {                                                                     \
        /* static assert */                                                  \
        PATOMIC_STATIC_ASSERT(                                               \
            long_big_enough,                                                 \
            (LONG_MAX / 2l) > ((long) UCHAR_MAX)                             \
        );                                                                   \
        /* setup */                                                          \
        long borrow = 1l;                                                    \
        const int is_le = IMPL_DEFINED_IS_LE();                              \
        for (size_t i = 0; i < (width); ++i) {                               \
            const size_t idx = (is_le) ? i : ((size_t) ((width) - 1ul - i)); \
            /* diff: obj[i] - borrow */                                      \
            long diff = (long) ((obj)[idx]);                                 \
            diff -= borrow;                                                  \
            /* check for negative */                                         \
            if (diff < 0l) {                                                 \
                diff += 256l;                                                \
                borrow = 1l;                                                 \
            } else {                                                         \
                borrow = 0l;                                                 \
            }                                                                \
            /* do sub */                                                     \
            (obj)[idx] = (unsigned char) (diff & 0xFFl);                     \
            if (borrow == 0l)                                                \
            {                                                                \
                break;                                                       \
            }                                                                \
        }                                                                    \
    } while (0)
#define patomic_wrapped_tsx_do_neg(obj, width)                               \
    do {                                                                     \
        /* static assert */                                                  \
        PATOMIC_STATIC_ASSERT(                                               \
            ulong_big_enough,                                                \
            (ULONG_MAX / 2ul) > ((unsigned long) UCHAR_MAX)                  \
        );                                                                   \
        /* setup */                                                          \
        unsigned long carry = 1ul;                                           \
        const int is_le = IMPL_DEFINED_IS_LE();                              \
        for (size_t i = 0; i < (width); ++i) {                               \
            const size_t idx = (is_le) ? i : ((size_t) ((width) - 1ul - i)); \
            /* inv -> sum: ~obj[i] + carry */                                \
            unsigned long sum = (unsigned long) ((obj)[idx]);                \
            sum = (unsigned long) ~sum;                                      \
            sum += carry;                                                    \
            /* do neg */                                                     \
            (obj)[idx] = (unsigned char) (sum & 0xFFu);                      \
            carry = (sum >> 8ul);                                            \
        }                                                                    \
    } while (0)


/**
 * @addtogroup wrapped.tsx
 *
 * @brief
 *   Defines a function which implements an atomic fetch_add operation using
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
 */
#define PATOMIC_WRAPPED_TSX_DEFINE_OP_FETCH_ADD( \
    fn_name, tbegin, tcommit                     \
)                                                \
    PATOMIC_WRAPPED_TSX_DEFINE_OP_FETCH(         \
        fn_name, tbegin, tcommit,                \
        patomic_wrapped_tsx_do_add               \
    )


/**
 * @addtogroup wrapped.tsx
 *
 * @brief
 *   Defines a function which implements an atomic fetch_sub operation using
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
 */
#define PATOMIC_WRAPPED_TSX_DEFINE_OP_FETCH_SUB( \
    fn_name, tbegin, tcommit                     \
)                                                \
    PATOMIC_WRAPPED_TSX_DEFINE_OP_FETCH(         \
        fn_name, tbegin, tcommit,                \
        patomic_wrapped_tsx_do_sub               \
    )


/**
 * @addtogroup wrapped.tsx
 *
 * @brief
 *   Defines a function which implements an atomic fetch_inc operation using
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
 */
#define PATOMIC_WRAPPED_TSX_DEFINE_OP_FETCH_INC( \
    fn_name, tbegin, tcommit                     \
)                                                \
    PATOMIC_WRAPPED_TSX_DEFINE_OP_FETCH_NOARG(   \
        fn_name, tbegin, tcommit,                \
        patomic_wrapped_tsx_do_inc               \
    )


/**
 * @addtogroup wrapped.tsx
 *
 * @brief
 *   Defines a function which implements an atomic fetch_dec operation using
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
 */
#define PATOMIC_WRAPPED_TSX_DEFINE_OP_FETCH_DEC( \
    fn_name, tbegin, tcommit                     \
)                                                \
    PATOMIC_WRAPPED_TSX_DEFINE_OP_FETCH_NOARG(   \
        fn_name, tbegin, tcommit,                \
        patomic_wrapped_tsx_do_dec               \
    )


/**
 * @addtogroup wrapped.tsx
 *
 * @brief
 *   Defines a function which implements an atomic fetch_neg operation using
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
 */
#define PATOMIC_WRAPPED_TSX_DEFINE_OP_FETCH_NEG( \
    fn_name, tbegin, tcommit                     \
)                                                \
    PATOMIC_WRAPPED_TSX_DEFINE_OP_FETCH_NOARG(   \
        fn_name, tbegin, tcommit,                \
        patomic_wrapped_tsx_do_neg               \
    )


/**
 * @addtogroup wrapped.tsx
 *
 * @brief
 *   Defines a function which implements an atomic add operation using
 *   tbegin and tcommit as the underlying atomic transaction primitives.
 *
 * @details
 *   The defined function's signature will match
 *   patomic_opsig_transaction_void_t.
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
#define PATOMIC_WRAPPED_TSX_DEFINE_OP_ADD( \
    fn_name, tbegin, tcommit               \
)                                          \
    PATOMIC_WRAPPED_TSX_DEFINE_OP_VOID(    \
        fn_name, tbegin, tcommit,          \
        patomic_wrapped_tsx_do_add         \
    )


/**
 * @addtogroup wrapped.tsx
 *
 * @brief
 *   Defines a function which implements an atomic sub operation using
 *   tbegin and tcommit as the underlying atomic transaction primitives.
 *
 * @details
 *   The defined function's signature will match
 *   patomic_opsig_transaction_void_t.
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
#define PATOMIC_WRAPPED_TSX_DEFINE_OP_SUB( \
    fn_name, tbegin, tcommit               \
)                                          \
    PATOMIC_WRAPPED_TSX_DEFINE_OP_VOID(    \
        fn_name, tbegin, tcommit,          \
        patomic_wrapped_tsx_do_sub         \
    )


/**
 * @addtogroup wrapped.tsx
 *
 * @brief
 *   Defines a function which implements an atomic inc operation using
 *   tbegin and tcommit as the underlying atomic transaction primitives.
 *
 * @details
 *   The defined function's signature will match
 *   patomic_opsig_transaction_void_noarg_t.
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
#define PATOMIC_WRAPPED_TSX_DEFINE_OP_INC(    \
    fn_name, tbegin, tcommit                  \
)                                             \
    PATOMIC_WRAPPED_TSX_DEFINE_OP_VOID_NOARG( \
        fn_name, tbegin, tcommit,             \
        patomic_wrapped_tsx_do_inc            \
    )


/**
 * @addtogroup wrapped.tsx
 *
 * @brief
 *   Defines a function which implements an atomic dec operation using
 *   tbegin and tcommit as the underlying atomic transaction primitives.
 *
 * @details
 *   The defined function's signature will match
 *   patomic_opsig_transaction_void_noarg_t.
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
#define PATOMIC_WRAPPED_TSX_DEFINE_OP_DEC(    \
    fn_name, tbegin, tcommit                  \
)                                             \
    PATOMIC_WRAPPED_TSX_DEFINE_OP_VOID_NOARG( \
        fn_name, tbegin, tcommit,             \
        patomic_wrapped_tsx_do_dec            \
    )


/**
 * @addtogroup wrapped.tsx
 *
 * @brief
 *   Defines a function which implements an atomic neg operation using
 *   tbegin and tcommit as the underlying atomic transaction primitives.
 *
 * @details
 *   The defined function's signature will match
 *   patomic_opsig_transaction_void_noarg_t.
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
#define PATOMIC_WRAPPED_TSX_DEFINE_OP_NEG(    \
    fn_name, tbegin, tcommit                  \
)                                             \
    PATOMIC_WRAPPED_TSX_DEFINE_OP_VOID_NOARG( \
        fn_name, tbegin, tcommit,             \
        patomic_wrapped_tsx_do_neg            \
    )


/**
 * @addtogroup wrapped.tsx
 *
 * @brief
 *   Defines a function which implements an atomic double_cmpxchg operation
 *   using tbegin and tcommit as the underlying atomic transaction primitives.
 *
 * @details
 *   The defined function's signature will match
 *   patomic_opsig_transaction_double_cmpxchg_t.
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
#define PATOMIC_WRAPPED_TSX_DEFINE_OP_DOUBLE_CMPXCHG(                  \
    fn_name, tbegin, tcommit                                           \
)                                                                      \
    static int                                                         \
    fn_name(                                                           \
        const patomic_transaction_cmpxchg_t cxa,                       \
        const patomic_transaction_cmpxchg_t cxb,                       \
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
        PATOMIC_WRAPPED_DO_ASSERT(cxa.obj != NULL);                    \
        PATOMIC_WRAPPED_DO_ASSERT(cxa.expected != NULL);               \
        PATOMIC_WRAPPED_DO_ASSERT(cxa.desired != NULL);                \
        PATOMIC_WRAPPED_DO_ASSERT(cxb.obj != NULL);                    \
        PATOMIC_WRAPPED_DO_ASSERT(cxb.expected != NULL);               \
        PATOMIC_WRAPPED_DO_ASSERT(cxb.desired != NULL);                \
                                                                       \
        /* operation */                                                \
        while (config.attempts-- > 0ul)                                \
        {                                                              \
            ++res.attempts_made;                                       \
            res.status = tbegin();                                     \
            if (res.status == 0ul)                                     \
            {                                                          \
                int cmp_a, cmp_b;                                      \
                PATOMIC_IGNORE_UNUSED(*config.flag_nullable);          \
                cmp_a = memcmp(                                        \
                    (const void *) cxa.obj, cxa.expected, config.width \
                );                                                     \
                cmp_b = memcmp(                                        \
                    (const void *) cxb.obj, cxb.expected, config.width \
                )                                                      \
                if (cmp_a == 0 && cmp_b == 0)                          \
                {                                                      \
                    PATOMIC_WRAPPED_DO_MEMCPY(                         \
                        (void *) cxa.obj, cxa.desired, config.width    \
                    );                                                 \
                    PATOMIC_WRAPPED_DO_MEMCPY(                         \
                        (void *) cxb.obj, cxb.desired, config.width    \
                    );                                                 \
                }                                                      \
                tcommit();                                             \
                if (!(cmp_a == 0 && cmp_b == 0))                       \
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
        PATOMIC_WRAPPED_DO_ASSERT(cxa.obj != NULL);                    \
        PATOMIC_WRAPPED_DO_ASSERT(cxa.expected != NULL);               \
        PATOMIC_WRAPPED_DO_ASSERT(cxb.obj != NULL);                    \
        PATOMIC_WRAPPED_DO_ASSERT(cxb.expected != NULL);               \
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
                    cxa.expected, (const void *) cxa.obj, config.width \
                );                                                     \
                PATOMIC_WRAPPED_DO_MEMCPY(                             \
                    cxb.expected, (const void *) cxb.obj, config.width \
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
 *   Defines a function which implements an atomic multi_cmpxchg operation
 *   using tbegin and tcommit as the underlying atomic transaction primitives.
 *
 * @details
 *   The defined function's signature will match
 *   patomic_opsig_transaction_multi_cmpxchg_t.
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
#define PATOMIC_WRAPPED_TSX_DEFINE_OP_MULTI_CMPXCHG(                   \
    fn_name, tbegin, tcommit                                           \
)                                                                      \
    static int                                                         \
    fn_name(                                                           \
        const patomic_transaction_cmpxchg_t *const cxs_buf,            \
        const size_t cxs_len,                                          \
        patomic_transaction_config_wfb_t config,                       \
        patomic_transaction_result_wfb_t *const result                 \
    )                                                                  \
    {                                                                  \
        /* declarations */                                             \
        size_t i;                                                      \
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
        for (i = 0; i < cxs_len; ++i)                                  \
        {                                                              \
            PATOMIC_WRAPPED_DO_ASSERT(cxs_buf[i].obj != NULL);         \
            PATOMIC_WRAPPED_DO_ASSERT(cxs_buf[i].expected != NULL);    \
            PATOMIC_WRAPPED_DO_ASSERT(cxs_buf[i].desired != NULL);     \
        }                                                              \
                                                                       \
        /* operation */                                                \
        while (config.attempts-- > 0ul)                                \
        {                                                              \
            ++res.attempts_made;                                       \
            res.status = tbegin();                                     \
            if (res.status == 0ul)                                     \
            {                                                          \
                size_t cmp = 0;                                        \
                PATOMIC_IGNORE_UNUSED(*config.flag_nullable);          \
                for (i = 0; i < cxs_len && cmp_all == 0; ++i)          \
                {                                                      \
                    cmp = memcmp(                                      \
                        (const void *) cxs_buf[i].obj,                 \
                        cxs_buf[i].expected,                           \
                        config.width                                   \
                    );                                                 \
                }                                                      \
                if (cmp == 0)                                          \
                {                                                      \
                    for (i = 0; i < cxs_len; ++i)                      \
                    {                                                  \
                        PATOMIC_WRAPPED_DO_MEMCPY(                     \
                            (void *) cxs_buf[i].obj,                   \
                            cxs_buf[i].desired,                        \
                            config.width                               \
                        );                                             \
                    }                                                  \
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
        for (i = 0; i < cxs_len; ++i)                                  \
        {                                                              \
            PATOMIC_WRAPPED_DO_ASSERT(cxs_buf[i].obj != NULL);         \
            PATOMIC_WRAPPED_DO_ASSERT(cxs_buf[i].expected != NULL);    \
        }                                                              \
                                                                       \
        /* operation */                                                \
        while (config.fallback_attempts-- > 0ul)                       \
        {                                                              \
            ++res.fallback_attempts_made;                              \
            res.fallback_status = tbegin();                            \
            if (res.fallback_status == 0ul)                            \
            {                                                          \
                PATOMIC_IGNORE_UNUSED(*config.fallback_flag_nullable); \
                for (i = 0; i < cxs_len; ++i)                          \
                {                                                      \
                    PATOMIC_WRAPPED_DO_MEMCPY(                         \
                        cxs_buf[i].expected,                           \
                        (const void *) cxs_buf[i].obj,                 \
                        config.width                                   \
                    );                                                 \
                }                                                      \
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
 *   Defines a function which implements an atomic generic operation using
 *   tbegin and tcommit as the underlying atomic transaction primitives.
 *
 * @details
 *   The defined function's signature will match
 *   patomic_opsig_transaction_generic_t.
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
#define PATOMIC_WRAPPED_TSX_DEFINE_OP_GENERIC(                            \
    fn_name, tbegin, tcommit                                              \
)                                                                         \
    static void                                                           \
    fn_name(                                                              \
        void (* const fn) (void *),                                       \
        void *const ctx,                                                  \
        patomic_transaction_config_t config,                              \
        patomic_transaction_result_t *const result                        \
    )                                                                     \
    {                                                                     \
        /* declarations */                                                \
        patomic_transaction_result_t res = {0};                           \
        const patomic_transaction_flag_t flag = 0;                        \
        if (config.flag_nullable == NULL)                                 \
        {                                                                 \
            config.flag_nullable = &flag;                                 \
        }                                                                 \
                                                                          \
        /* assert early */                                                \
        PATOMIC_WRAPPED_DO_ASSERT(result != NULL);                        \
                                                                          \
        /* check zero */                                                  \
        PATOMIC_WRAPPED_TSX_CHECK_CONFIG_ZERO(config, res, cleanup);      \
                                                                          \
        /* assertions */                                                  \
        PATOMIC_WRAPPED_DO_ASSERT(fn != NULL);                            \
                                                                          \
        /* operation */                                                   \
        while (config.attempts-- > 0ul)                                   \
        {                                                                 \
            ++res.attempts_made;                                          \
            res.status = tbegin();                                        \
            if (res.status == 0ul)                                        \
            {                                                             \
                PATOMIC_IGNORE_UNUSED(*config.flag_nullable);             \
                fn(ctx);                                                  \
                tcommit();                                                \
                goto cleanup;                                             \
            }                                                             \
        }                                                                 \
                                                                          \
        /* cleanup */                                                     \
    cleanup:                                                              \
        *result = res;                                                    \
    }


/**
 * @addtogroup wrapped.tsx
 *
 * @brief
 *   Defines all xchg operations as well as a function that returns an instance
 *   of patomic_ops_transaction_xchg_t that points to these functions.
 *
 * @param name
 *   The name suffixed to all defined functions.
 *
 * @param tbegin
 *   A callable with the signature and semantics of
 *   patomic_opsig_transaction_tbegin_t.
 *
 * @param tcommit
 *   A callable with the signature and semantics of
 *   patomic_opsig_transaction_tcommit_t.
 */
#define PATOMIC_WRAPPED_TSX_DEFINE_XCHG_OPS_CREATE(               \
    name, tbegin, tcommit                                         \
)                                                                 \
    PATOMIC_WRAPPED_TSX_DEFINE_OP_EXCHANGE(                       \
        patomic_opimpl_exchange_##name, tbegin, tcommit           \
    )                                                             \
    PATOMIC_WRAPPED_TSX_DEFINE_OP_CMPXCHG_WEAK(                   \
        patomic_opimpl_cmpxchg_weak_##name, tbegin, tcommit       \
    )                                                             \
    static patomic_ops_transaction_xchg_t                         \
    patomic_ops_xchg_create_##name(void)                          \
    {                                                             \
        patomic_ops_transaction_xchg_t pao;                       \
        pao.fp_exchange = patomic_opimpl_exchange_##name;         \
        pao.fp_cmpxchg_weak = patomic_opimpl_cmpxchg_weak_##name; \
        pao.fp_cmpxchg_strong = NULL;                             \
        return pao;                                               \
    }


/**
 * @addtogroup wrapped.tsx
 *
 * @brief
 *   Defines all bitwise operations as well as a function that returns an
 *   instance of patomic_ops_transaction_bitwise_t that points to these
 *   functions.
 *
 * @param name
 *   The name suffixed to all defined functions.
 *
 * @param tbegin
 *   A callable with the signature and semantics of
 *   patomic_opsig_transaction_tbegin_t.
 *
 * @param tcommit
 *   A callable with the signature and semantics of
 *   patomic_opsig_transaction_tcommit_t.
 */
#define PATOMIC_WRAPPED_TSX_DEFINE_BITWISE_OPS_CREATE(            \
    name, tbegin, tcommit                                         \
)                                                                 \
    PATOMIC_WRAPPED_TSX_DEFINE_OP_BIT_TEST(                       \
        patomic_opimpl_bit_test_##name, tbegin, tcommit           \
    )                                                             \
    PATOMIC_WRAPPED_TSX_DEFINE_OP_BIT_TEST_COMPL(                 \
        patomic_opimpl_bit_test_compl_##name, tbegin, tcommit     \
    )                                                             \
    PATOMIC_WRAPPED_TSX_DEFINE_OP_BIT_TEST_SET(                   \
        patomic_opimpl_bit_test_set_##name, tbegin, tcommit       \
    )                                                             \
    PATOMIC_WRAPPED_TSX_DEFINE_OP_BIT_TEST_RESET(                 \
        patomic_opimpl_bit_test_reset_##name, tbegin, tcommit     \
    )                                                             \
    static patomic_ops_transaction_bitwise_t                      \
    patomic_ops_bitwise_create_##name(void)                       \
    {                                                             \
        patomic_ops_transaction_bitwise_t pao;                    \
        pao.fp_test = patomic_opimpl_bit_test_##name;             \
        pao.fp_test_compl = patomic_opimpl_bit_test_compl_##name; \
        pao.fp_test_set = patomic_opimpl_bit_test_set_##name;     \
        pao.fp_test_reset = patomic_opimpl_bit_test_reset_##name; \
        return pao;                                               \
    }


/**
 * @addtogroup wrapped.tsx
 *
 * @brief
 *   Defines all binary operations as well as a function that returns an
 *   instance of patomic_ops_transaction_binary_t that points to these
 *   functions.
 *
 * @param name
 *   The name suffixed to all defined functions.
 *
 * @param tbegin
 *   A callable with the signature and semantics of
 *   patomic_opsig_transaction_tbegin_t.
 *
 * @param tcommit
 *   A callable with the signature and semantics of
 *   patomic_opsig_transaction_tcommit_t.
 */
#define PATOMIC_WRAPPED_TSX_DEFINE_BINARY_OPS_CREATE(       \
    name, tbegin, tcommit                                   \
)                                                           \
    PATOMIC_WRAPPED_TSX_DEFINE_OP_OR(                       \
        patomic_opimpl_void_or_##name, tbegin, tcommit      \
    )                                                       \
    PATOMIC_WRAPPED_TSX_DEFINE_OP_XOR(                      \
        patomic_opimpl_void_xor_##name, tbegin, tcommit     \
    )                                                       \
    PATOMIC_WRAPPED_TSX_DEFINE_OP_AND(                      \
        patomic_opimpl_void_and_##name, tbegin, tcommit     \
    )                                                       \
    PATOMIC_WRAPPED_TSX_DEFINE_OP_NOT(                      \
        patomic_opimpl_void_not_##name, tbegin, tcommit     \
    )                                                       \
    PATOMIC_WRAPPED_TSX_DEFINE_OP_FETCH_OR(                 \
        patomic_opimpl_fetch_or_##name, tbegin, tcommit     \
    )                                                       \
    PATOMIC_WRAPPED_TSX_DEFINE_OP_FETCH_XOR(                \
        patomic_opimpl_fetch_xor_##name, tbegin, tcommit    \
    )                                                       \
    PATOMIC_WRAPPED_TSX_DEFINE_OP_FETCH_AND(                \
        patomic_opimpl_fetch_and_##name, tbegin, tcommit    \
    )                                                       \
    PATOMIC_WRAPPED_TSX_DEFINE_OP_FETCH_NOT(                \
        patomic_opimpl_fetch_not_##name, tbegin, tcommit    \
    )                                                       \
    static patomic_ops_transaction_binary_t                 \
    patomic_ops_binary_create_##name(void)                  \
    {                                                       \
        patomic_ops_transaction_binary_t pao;               \
        pao.fp_or  = patomic_opimpl_void_or_##name;         \
        pao.fp_xor = patomic_opimpl_void_xor_##name;        \
        pao.fp_and = patomic_opimpl_void_and_##name;        \
        pao.fp_not = patomic_opimpl_void_not_##name;        \
        pao.fp_fetch_or  = patomic_opimpl_fetch_or_##name;  \
        pao.fp_fetch_xor = patomic_opimpl_fetch_xor_##name; \
        pao.fp_fetch_and = patomic_opimpl_fetch_and_##name; \
        pao.fp_fetch_not = patomic_opimpl_fetch_not_##name; \
        return pao;                                         \
    }


/**
 * @addtogroup wrapped.tsx
 *
 * @brief
 *   Defines all arithmetic operations as well as a function that returns an
 *   instance of patomic_ops_transaction_arithmetic_t that points to these
 *   functions.
 *
 * @param name
 *   The name suffixed to all defined functions.
 *
 * @param tbegin
 *   A callable with the signature and semantics of
 *   patomic_opsig_transaction_tbegin_t.
 *
 * @param tcommit
 *   A callable with the signature and semantics of
 *   patomic_opsig_transaction_tcommit_t.
 */
#define PATOMIC_WRAPPED_TSX_DEFINE_ARITHMETIC_OPS_CREATE(   \
    name, tbegin, tcommit                                   \
)                                                           \
    PATOMIC_WRAPPED_TSX_DEFINE_OP_ADD(                      \
        patomic_opimpl_void_add_##name, tbegin, tcommit     \
    )                                                       \
    PATOMIC_WRAPPED_TSX_DEFINE_OP_SUB(                      \
        patomic_opimpl_void_sub_##name, tbegin, tcommit     \
    )                                                       \
    PATOMIC_WRAPPED_TSX_DEFINE_OP_INC(                      \
        patomic_opimpl_void_inc_##name, tbegin, tcommit     \
    )                                                       \
    PATOMIC_WRAPPED_TSX_DEFINE_OP_DEC(                      \
        patomic_opimpl_void_dec_##name, tbegin, tcommit     \
    )                                                       \
    PATOMIC_WRAPPED_TSX_DEFINE_OP_NEG(                      \
        patomic_opimpl_void_neg_##name, tbegin, tcommit     \
    )                                                       \
    PATOMIC_WRAPPED_TSX_DEFINE_OP_FETCH_ADD(                \
        patomic_opimpl_fetch_add_##name, tbegin, tcommit    \
    )                                                       \
    PATOMIC_WRAPPED_TSX_DEFINE_OP_FETCH_SUB(                \
        patomic_opimpl_fetch_sub_##name, tbegin, tcommit    \
    )                                                       \
    PATOMIC_WRAPPED_TSX_DEFINE_OP_FETCH_INC(                \
        patomic_opimpl_fetch_inc_##name, tbegin, tcommit    \
    )                                                       \
    PATOMIC_WRAPPED_TSX_DEFINE_OP_FETCH_DEC(                \
        patomic_opimpl_fetch_dec_##name, tbegin, tcommit    \
    )                                                       \
    PATOMIC_WRAPPED_TSX_DEFINE_OP_FETCH_NEG(                \
        patomic_opimpl_fetch_neg_##name, tbegin, tcommit    \
    )                                                       \
    static patomic_ops_transaction_arithmetic_t             \
    patomic_ops_arithmetic_create_##name(void)              \
    {                                                       \
        patomic_ops_transaction_arithmetic_t pao;           \
        pao.fp_add = patomic_opimpl_void_add_##name;        \
        pao.fp_sub = patomic_opimpl_void_sub_##name;        \
        pao.fp_inc = patomic_opimpl_void_inc_##name;        \
        pao.fp_dec = patomic_opimpl_void_dec_##name;        \
        pao.fp_neg = patomic_opimpl_void_neg_##name;        \
        pao.fp_fetch_add = patomic_opimpl_fetch_add_##name; \
        pao.fp_fetch_sub = patomic_opimpl_fetch_sub_##name; \
        pao.fp_fetch_inc = patomic_opimpl_fetch_inc_##name; \
        pao.fp_fetch_dec = patomic_opimpl_fetch_dec_##name; \
        pao.fp_fetch_neg = patomic_opimpl_fetch_neg_##name; \
        return pao;                                         \
    }


/**
 * @addtogroup wrapped.tsx
 *
 * @brief
 *   Defines all operations which are not unique to transactions as well as a
 *   function that returns an instance of patomic_ops_transaction_t that points
 *   to these functions.
 *
 * @param name
 *   The name suffixed to all defined functions.
 *
 * @param tbegin
 *   A callable with the signature and semantics of
 *   patomic_opsig_transaction_tbegin_t.
 *
 * @param tcommit
 *   A callable with the signature and semantics of
 *   patomic_opsig_transaction_tcommit_t.
 */
#define PATOMIC_WRAPPED_TSX_DEFINE_OPS_CREATE(                       \
    name, tbegin, tcommit                                            \
)                                                                    \
    PATOMIC_WRAPPED_TSX_DEFINE_OP_STORE(                             \
        patomic_opimpl_store_##name, tbegin, tcommit                 \
    )                                                                \
    PATOMIC_WRAPPED_TSX_DEFINE_OP_LOAD(                              \
        patomic_opimpl_load_##name, tbegin, tcommit                  \
    )                                                                \
    PATOMIC_WRAPPED_TSX_DEFINE_XCHG_OPS_CREATE(                      \
        name, tbegin, tcommit                                        \
    )                                                                \
    PATOMIC_WRAPPED_TSX_DEFINE_BITWISE_OPS_CREATE(                   \
        name, tbegin, tcommit                                        \
    )                                                                \
    PATOMIC_WRAPPED_TSX_DEFINE_BINARY_OPS_CREATE(                    \
        name, tbegin, tcommit                                        \
    )                                                                \
    PATOMIC_WRAPPED_TSX_DEFINE_ARITHMETIC_OPS_CREATE(                \
        name, tbegin, tcommit                                        \
    )                                                                \
    static patomic_ops_transaction_t                                 \
    patomic_ops_create_##name(void)                                  \
    {                                                                \
        patomic_ops_transaction_t pao;                               \
        pao.fp_store = patomic_opimpl_store_##name;                  \
        pao.fp_load = patomic_opimpl_load_##name;                    \
        pao.xchg_ops = patomic_ops_xchg_create_##name();             \
        pao.bitwise_ops = patomic_ops_bitwise_create_##name();       \
        pao.binary_ops = patomic_ops_binary_create_##name();         \
        pao.arithmetic_ops = patomic_ops_arithmetic_create_##name(); \
        return pao;                                                  \
    }


#endif  /* PATOMIC_WRAPPED_TSX_H */
