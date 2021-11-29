#ifndef PATOMIC_OPS_H
#define PATOMIC_OPS_H

#include <patomic/types/transaction.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * IMPLICIT MEMORY ORDER OPSIGS
 * - in-params
 * - out-params
 */

typedef void (* patomic_opsig_store_t) \
    (volatile void *obj,               \
     const void *desired);

typedef void (* patomic_opsig_load_t) \
    (const volatile void *obj,        \
     void *ret);

typedef void (* patomic_opsig_exchange_t) \
    (volatile void *obj,                  \
     const void *desired,                 \
     void *ret);

typedef int (* patomic_opsig_cmpxchg_t) \
    (volatile void *obj,                \
     void *expected,                    \
     const void *desired);

typedef int (* patomic_opsig_test_t) \
    (const volatile void *obj,       \
     int offset);

typedef int (* patomic_opsig_test_modify_t) \
    (volatile void *obj,                    \
     int offset);                           \

typedef void (* patomic_opsig_fetch_t) \
    (volatile void *obj,               \
     const void *arg,                  \
     void *ret);

typedef void (* patomic_opsig_fetch_noarg_t) \
    (volatile void *obj,                     \
     void *ret);

typedef void (* patomic_opsig_void_t) \
    (volatile void *obj,              \
     const void *arg);

typedef void (* patomic_opsig_void_noarg_t) \
    (volatile void *obj);


/*
 * EXPLICIT MEMORY ORDER OPSIGS
 * - in-params
 * - memory order
 * - out-params
 */

typedef void (* patomic_opsig_explicit_store_t) \
    (volatile void *obj,                        \
     const void *desired,                       \
     int order);

typedef void (* patomic_opsig_explicit_load_t) \
    (const volatile void *obj,                 \
     int order,                                \
     void *ret);

typedef void (* patomic_opsig_explicit_exchange_t) \
    (volatile void *obj,                           \
     const void *desired,                          \
     int order,                                    \
     void *ret);

typedef int (* patomic_opsig_explicit_cmpxchg_t) \
    (volatile void *obj,                         \
     void *expected,                             \
     const void *desired,                        \
     int succ,                                   \
     int fail);

typedef int (* patomic_opsig_explicit_test_t) \
    (const volatile void *obj,                \
     int offset,                              \
     int order);

typedef int (* patomic_opsig_explicit_test_modify_t) \
    (volatile void *obj,                             \
     int offset,                                     \
     int order);

typedef void (* patomic_opsig_explicit_fetch_t) \
    (volatile void *obj,                        \
     const void *arg,                           \
     int order,                                 \
     void *ret);

typedef void (* patomic_opsig_explicit_fetch_noarg_t) \
    (volatile void *obj,                     \
     int order,                              \
     void *ret);

typedef void (* patomic_opsig_explicit_void_t) \
    (volatile void *obj,                       \
     const void *arg,                          \
     int order);

typedef void (* patomic_opsig_explicit_void_noarg_t) \
    (volatile void *obj,                             \
     int order);


/*
 * TRANSACTION OPSIGS
 * - in-params
 * - out-params
 * - result-out-param
 * - config
 *
 * - (except for flag - its only param is the flag)
 */

typedef void (* patomic_opsig_transaction_store_t) \
    (volatile void *obj,                           \
     const void *desired,                          \
     patomic_transaction_result_t *result,         \
     patomic_transaction_config_t config);

typedef void (* patomic_opsig_transaction_load_t) \
    (const volatile void *obj,                    \
     void *ret,                                   \
     patomic_transaction_result_t *result,        \
     patomic_transaction_config_t config);

typedef void (* patomic_opsig_transaction_exchange_t) \
    (volatile void *obj,                              \
     const void *desired,                             \
     void *ret,                                       \
     patomic_transaction_result_t *result,            \
     patomic_transaction_config_t config);

typedef int (* patomic_opsig_transaction_cmpxchg_t) \
    (volatile void *obj,                            \
     void *expected,                                \
     const void *desired,                           \
     patomic_transaction_result_t *result,          \
     patomic_transaction_config_t config);

typedef int (* patomic_opsig_transaction_test_t) \
    (const volatile void *obj,                   \
     size_t offset,                              \
     patomic_transaction_result_t *result,       \
     patomic_transaction_config_t config);

typedef int (* patomic_opsig_transaction_test_modify_t) \
    (volatile void *obj,                                \
     size_t offset,                                     \
     patomic_transaction_result_t *result,              \
     patomic_transaction_config_t config);

typedef void (* patomic_opsig_transaction_fetch_t) \
    (volatile void *obj,                           \
     const void *arg,                              \
     void *ret,                                    \
     patomic_transaction_result_t *result,         \
     patomic_transaction_config_t config);

typedef void (* patomic_opsig_transaction_fetch_noarg_t) \
    (volatile void *obj,                                 \
     void *ret,                                          \
     patomic_transaction_result_t *result,               \
     patomic_transaction_config_t config);

typedef void (* patomic_opsig_transaction_void_t) \
    (volatile void *obj,                          \
     const void *arg,                             \
     patomic_transaction_result_t *result,        \
     patomic_transaction_config_t config);

typedef void (* patomic_opsig_transaction_void_noarg_t) \
    (volatile void *obj,                                \
     patomic_transaction_result_t *result,              \
     patomic_transaction_config_t config);

typedef void (* patomic_opsig_transaction_flag_t) \
    (patomic_transaction_flag_t *);


/*
 * OP STRUCTS
 * - implicit: each operation is a function pointer (fp)
 *             without a memory order parameter (implicit)
 * - explicit: each operation is a function pointer (fp)
 *             with an explicit memory order parameter
 * - transaction: each operation is a function pointer (fp)
 *                with either a transaction result and config,
 *                or a flag
 */

#undef PATOMIC_DEFINE_ARITHMETIC_OPS_STRUCT
#define PATOMIC_DEFINE_ARITHMETIC_OPS_STRUCT(opsig_kind, ops_kind) \
    typedef struct {                                               \
        /* in place */                                             \
        patomic_##opsig_kind##_void_t fp_add;                      \
        patomic_##opsig_kind##_void_t fp_sub;                      \
        patomic_##opsig_kind##_void_noarg_t fp_inc;                \
        patomic_##opsig_kind##_void_noarg_t fp_dec;                \
        patomic_##opsig_kind##_void_noarg_t fp_neg;                \
        /* fetch */                                                \
        patomic_##opsig_kind##_fetch_t fp_fetch_add;               \
        patomic_##opsig_kind##_fetch_t fp_fetch_sub;               \
        patomic_##opsig_kind##_fetch_noarg_t fp_fetch_inc;         \
        patomic_##opsig_kind##_fetch_noarg_t fp_fetch_dec;         \
        patomic_##opsig_kind##_fetch_noarg_t fp_fetch_neg;         \
    } patomic_##ops_kind##_arithmetic_t

#undef PATOMIC_DEFINE_BINARY_OPS_STRUCT
#define PATOMIC_DEFINE_BINARY_OPS_STRUCT(opsig_kind, ops_kind) \
    typedef struct {                                           \
        /* in place */                                         \
        patomic_##opsig_kind##_void_t fp_or;                   \
        patomic_##opsig_kind##_void_t fp_xor;                  \
        patomic_##opsig_kind##_void_t fp_and;                  \
        patomic_##opsig_kind##_void_noarg_t fp_not;            \
        /* fetch */                                            \
        patomic_##opsig_kind##_fetch_t fp_fetch_or;            \
        patomic_##opsig_kind##_fetch_t fp_fetch_xor;           \
        patomic_##opsig_kind##_fetch_t fp_fetch_and;           \
        patomic_##opsig_kind##_fetch_noarg_t fp_fetch_not;     \
    } patomic_##ops_kind##_binary_t

#undef PATOMIC_DEFINE_BITWISE_OPS_STRUCT
#define PATOMIC_DEFINE_BITWISE_OPS_STRUCT(opsig_kind, ops_kind) \
    typedef struct {                                            \
        patomic_##opsig_kind##_test_t fp_test;                  \
        patomic_##opsig_kind##_test_modify_t fp_test_compl;     \
        patomic_##opsig_kind##_test_modify_t fp_test_set;       \
        patomic_##opsig_kind##_test_modify_t fp_test_reset;     \
    } patomic_##ops_kind##_bitwise_t

#undef PATOMIC_DEFINE_XCHG_OPS_STRUCT
#define PATOMIC_DEFINE_XCHG_OPS_STRUCT(opsig_kind, ops_kind) \
    typedef struct {                                         \
        patomic_##opsig_kind##_exchange_t fp_exchange;       \
        patomic_##opsig_kind##_cmpxchg_t fp_cmpxchg_weak;    \
        patomic_##opsig_kind##_cmpxchg_t fp_cmpxchg_strong;  \
    } patomic_##ops_kind##_xchg_t

typedef struct {
    patomic_opsig_transaction_flag_t fp_set;
    patomic_opsig_transaction_flag_t fp_reset;
} patomic_ops_transaction_flag_t;

typedef struct {
    char _todo;
} patomic_ops_transaction_special_t;

#undef PATOMIC_DEFINE_OPS_STRUCTS
#define PATOMIC_DEFINE_OPS_STRUCTS(opsig_kind, ops_kind)        \
    PATOMIC_DEFINE_ARITHMETIC_OPS_STRUCT(opsig_kind, ops_kind); \
    PATOMIC_DEFINE_BINARY_OPS_STRUCT(opsig_kind, ops_kind);     \
    PATOMIC_DEFINE_BITWISE_OPS_STRUCT(opsig_kind, ops_kind);    \
    PATOMIC_DEFINE_XCHG_OPS_STRUCT(opsig_kind, ops_kind)

/* implicit */
PATOMIC_DEFINE_OPS_STRUCTS(opsig, ops);
typedef struct {
    patomic_opsig_store_t fp_store;
    patomic_opsig_load_t fp_load;
    patomic_ops_xchg_t xchg_ops;
    patomic_ops_bitwise_t bitwise_ops;
    patomic_ops_binary_t binary_ops;
    patomic_ops_arithmetic_t signed_ops;
    patomic_ops_arithmetic_t unsigned_ops;
} patomic_ops_t;

/* explicit */
PATOMIC_DEFINE_OPS_STRUCTS(opsig_explicit, ops_explicit);
typedef struct {
    patomic_opsig_explicit_store_t fp_store;
    patomic_opsig_explicit_load_t fp_load;
    patomic_ops_explicit_xchg_t xchg_ops;
    patomic_ops_explicit_bitwise_t bitwise_ops;
    patomic_ops_explicit_binary_t binary_ops;
    patomic_ops_explicit_arithmetic_t signed_ops;
    patomic_ops_explicit_arithmetic_t unsigned_ops;
} patomic_ops_explicit_t;

/* transaction */
PATOMIC_DEFINE_OPS_STRUCTS(opsig_transaction, ops_transaction);
typedef struct {
    patomic_opsig_transaction_store_t fp_store;
    patomic_opsig_transaction_load_t fp_load;
    patomic_ops_transaction_xchg_t xchg_ops;
    patomic_ops_transaction_bitwise_t bitwise_ops;
    patomic_ops_transaction_binary_t binary_ops;
    patomic_ops_transaction_arithmetic_t signed_ops;
    patomic_ops_transaction_arithmetic_t unsigned_ops;
    /* extra ops not available in implicit/explicit */
    patomic_ops_transaction_flag_t flag_ops;
    patomic_ops_transaction_special_t special_ops;
} patomic_ops_transaction_t;


#undef PATOMIC_DEFINE_OPS_STRUCTS
#undef PATOMIC_DEFINE_XCHG_OPS_STRUCT
#undef PATOMIC_DEFINE_BITWISE_OPS_STRUCT
#undef PATOMIC_DEFINE_BINARY_OPS_STRUCT
#undef PATOMIC_DEFINE_ARITHMETIC_OPS_STRUCT

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* !PATOMIC_OPS_H */
