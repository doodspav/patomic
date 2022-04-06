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
 * - (except for flag and raw)
 */

typedef patomic_transaction_result_t      \
    (* patomic_opsig_transaction_store_t) \
    (volatile void *obj,                  \
     const void *desired,                 \
     patomic_transaction_config_t config);

typedef patomic_transaction_result_t     \
    (* patomic_opsig_transaction_load_t) \
    (const volatile void *obj,           \
     void *ret,                          \
     patomic_transaction_config_t config);

typedef patomic_transaction_result_t         \
    (* patomic_opsig_transaction_exchange_t) \
    (volatile void *obj,                     \
     const void *desired,                    \
     void *ret,                              \
     patomic_transaction_config_t config);

typedef patomic_transaction_result_wfb_t    \
    (* patomic_opsig_transaction_cmpxchg_t) \
    (volatile void *obj,                    \
     void *expected,                        \
     const void *desired,                   \
     int *ret,                              \
     patomic_transaction_config_wfb_t config);

typedef patomic_transaction_result_t     \
    (* patomic_opsig_transaction_test_t) \
    (const volatile void *obj,           \
     size_t offset,                      \
     int *ret,                           \
     patomic_transaction_config_t config);

typedef patomic_transaction_result_t            \
    (* patomic_opsig_transaction_test_modify_t) \
    (volatile void *obj,                        \
     size_t offset,                             \
     int *ret,                                  \
     patomic_transaction_config_t config);

typedef patomic_transaction_result_t      \
    (* patomic_opsig_transaction_fetch_t) \
    (volatile void *obj,                  \
     const void *arg,                     \
     void *ret,                           \
     patomic_transaction_config_t config);

typedef patomic_transaction_result_t            \
    (* patomic_opsig_transaction_fetch_noarg_t) \
    (volatile void *obj,                        \
     void *ret,                                 \
     patomic_transaction_config_t config);

typedef patomic_transaction_result_t     \
    (* patomic_opsig_transaction_void_t) \
    (volatile void *obj,                 \
     const void *arg,                    \
     patomic_transaction_config_t config);

typedef patomic_transaction_result_t           \
    (* patomic_opsig_transaction_void_noarg_t) \
    (volatile void *obj,                       \
     patomic_transaction_config_t config);

/* NOTE: expected and desired may be NULL - check transaction cmpxchg docs */
/* NOTE: config.width applies to both sets of params */
typedef patomic_transaction_result_wfb_t           \
    (* patomic_opsig_transaction_double_cmpxchg_t) \
    (volatile void *obj_a,                         \
     volatile void *obj_b,                         \
     void *expected_nullable_a,                    \
     void *expected_nullable_b,                    \
     const void *desired_nullable_a,               \
     const void *desired_nullable_b,               \
     patomic_transaction_config_wfb_t);

/* NOTE: size of cxs array is obtained from config.width */
typedef patomic_transaction_result_wfb_t      \
    (* patomic_opsig_transaction_n_cmpxchg_t) \
    (const patomic_transaction_cmpxchg_t *cxs,
     patomic_transaction_config_wfb_t config);

/* NOTE: config.width is ignored */
typedef patomic_transaction_result_t        \
    (* patomic_opsig_transaction_generic_t) \
    (void (* fn) (void *),                  \
     void *ctx,                             \
     patomic_transaction_config_t config);

/* NOTE: config.width is ignored */
typedef patomic_transaction_result_wfb_t        \
    (* patomic_opsig_transaction_generic_wfb_t) \
    (void (* fn) (void *),                      \
     void *ctx,                                 \
     void (* fallback_fn) (void *),             \
     void *fallback_ctx,                        \
     patomic_transaction_config_wfb_t config);

typedef int (* patomic_opsig_transaction_flag_test_t) \
    (const patomic_transaction_flag_t *);

typedef int (* patomic_opsig_transaction_flag_test_set_t) \
    (patomic_transaction_flag_t *);

typedef void (* patomic_opsig_transaction_flag_clear_t) \
    (patomic_transaction_flag_t *);

typedef unsigned int (* patomic_opsig_transaction_tbegin_t) (void);

/* Note: only first 8bits of reason will be used */
typedef void (* patomic_opsig_transaction_tabort_t) (unsigned char reason);

typedef void (* patomic_opsig_transaction_tcommit_t) (void);

typedef int (* patomic_opsig_transaction_ttest_t) (void);


/*
 * OP STRUCTS
 * - implicit: no extra parameters or special return type
 * - explicit: memory order parameter
 * - transaction: tconfig parameter, tresult return type (except flag and raw)
 */

/* ARITHMETIC OP STRUCTS */

typedef struct {
    /* in place */
    patomic_opsig_void_t fp_add;
    patomic_opsig_void_t fp_sub;
    patomic_opsig_void_noarg_t fp_inc;
    patomic_opsig_void_noarg_t fp_dec;
    patomic_opsig_void_noarg_t fp_neg;
    /* fetch */
    patomic_opsig_fetch_t fp_fetch_add;
    patomic_opsig_fetch_t fp_fetch_sub;
    patomic_opsig_fetch_noarg_t fp_fetch_inc;
    patomic_opsig_fetch_noarg_t fp_fetch_dec;
    patomic_opsig_fetch_noarg_t fp_fetch_neg;
} patomic_ops_arithmetic_t;

typedef struct {
    /* in place */
    patomic_opsig_explicit_void_t fp_add;
    patomic_opsig_explicit_void_t fp_sub;
    patomic_opsig_explicit_void_noarg_t fp_inc;
    patomic_opsig_explicit_void_noarg_t fp_dec;
    patomic_opsig_explicit_void_noarg_t fp_neg;
    /* fetch */
    patomic_opsig_explicit_fetch_t fp_fetch_add;
    patomic_opsig_explicit_fetch_t fp_fetch_sub;
    patomic_opsig_explicit_fetch_noarg_t fp_fetch_inc;
    patomic_opsig_explicit_fetch_noarg_t fp_fetch_dec;
    patomic_opsig_explicit_fetch_noarg_t fp_fetch_neg;
} patomic_ops_explicit_arithmetic_t;

typedef struct {
    /* in place */
    patomic_opsig_transaction_void_t fp_add;
    patomic_opsig_transaction_void_t fp_sub;
    patomic_opsig_transaction_void_noarg_t fp_inc;
    patomic_opsig_transaction_void_noarg_t fp_dec;
    patomic_opsig_transaction_void_noarg_t fp_neg;
    /* fetch */
    patomic_opsig_transaction_fetch_t fp_fetch_add;
    patomic_opsig_transaction_fetch_t fp_fetch_sub;
    patomic_opsig_transaction_fetch_noarg_t fp_fetch_inc;
    patomic_opsig_transaction_fetch_noarg_t fp_fetch_dec;
    patomic_opsig_transaction_fetch_noarg_t fp_fetch_neg;
} patomic_ops_transaction_arithmetic_t;

/* BINARY OP STRUCTS */

typedef struct {
    /* in place */
    patomic_opsig_void_t fp_or;
    patomic_opsig_void_t fp_xor;
    patomic_opsig_void_t fp_and;
    patomic_opsig_void_noarg_t fp_not;
    /* fetch */
    patomic_opsig_fetch_t fp_fetch_or;
    patomic_opsig_fetch_t fp_fetch_xor;
    patomic_opsig_fetch_t fp_fetch_and;
    patomic_opsig_fetch_noarg_t fp_fetch_not;
} patomic_ops_binary_t;

typedef struct {
    /* in place */
    patomic_opsig_explicit_void_t fp_or;
    patomic_opsig_explicit_void_t fp_xor;
    patomic_opsig_explicit_void_t fp_and;
    patomic_opsig_explicit_void_noarg_t fp_not;
    /* fetch */
    patomic_opsig_explicit_fetch_t fp_fetch_or;
    patomic_opsig_explicit_fetch_t fp_fetch_xor;
    patomic_opsig_explicit_fetch_t fp_fetch_and;
    patomic_opsig_explicit_fetch_noarg_t fp_fetch_not;
} patomic_ops_explicit_binary_t;

typedef struct {
    /* in place */
    patomic_opsig_transaction_void_t fp_or;
    patomic_opsig_transaction_void_t fp_xor;
    patomic_opsig_transaction_void_t fp_and;
    patomic_opsig_transaction_void_noarg_t fp_not;
    /* fetch */
    patomic_opsig_transaction_fetch_t fp_fetch_or;
    patomic_opsig_transaction_fetch_t fp_fetch_xor;
    patomic_opsig_transaction_fetch_t fp_fetch_and;
    patomic_opsig_transaction_fetch_noarg_t fp_fetch_not;
} patomic_ops_transaction_binary_t;

/* BITWISE OP STRUCTS */

typedef struct {
    patomic_opsig_test_t fp_test;
    patomic_opsig_test_modify_t fp_test_compl;
    patomic_opsig_test_modify_t fp_test_set;
    patomic_opsig_test_modify_t fp_test_reset;
} patomic_ops_bitwise_t;

typedef struct {
    patomic_opsig_explicit_test_t fp_test;
    patomic_opsig_explicit_test_modify_t fp_test_compl;
    patomic_opsig_explicit_test_modify_t fp_test_set;
    patomic_opsig_explicit_test_modify_t fp_test_reset;
} patomic_ops_explicit_bitwise_t;

typedef struct {
    patomic_opsig_transaction_test_t fp_test;
    patomic_opsig_transaction_test_modify_t fp_test_compl;
    patomic_opsig_transaction_test_modify_t fp_test_set;
    patomic_opsig_transaction_test_modify_t fp_test_reset;
} patomic_ops_transaction_bitwise_t;

/* XCHG OP STRUCTS */

typedef struct {
    patomic_opsig_exchange_t fp_exchange;
    patomic_opsig_cmpxchg_t fp_cmpxchg_weak;
    patomic_opsig_cmpxchg_t fp_cmpxchg_strong;
} patomic_ops_xchg_t;

typedef struct {
    patomic_opsig_explicit_exchange_t fp_exchange;
    patomic_opsig_explicit_cmpxchg_t fp_cmpxchg_weak;
    patomic_opsig_explicit_cmpxchg_t fp_cmpxchg_strong;
} patomic_ops_explicit_xchg_t;

typedef struct {
    patomic_opsig_transaction_exchange_t fp_exchange;
    patomic_opsig_transaction_cmpxchg_t fp_cmpxchg_weak;
    patomic_opsig_transaction_cmpxchg_t fp_cmpxchg_strong;
} patomic_ops_transaction_xchg_t;

/* TRANSACTION SPECIFIC OP STRUCTS */

typedef struct {
    patomic_opsig_transaction_flag_test_t fp_test;
    patomic_opsig_transaction_flag_test_set_t fp_test_set;
    patomic_opsig_transaction_flag_clear_t fp_clear;
} patomic_ops_transaction_flag_t;

typedef struct {
    patomic_opsig_transaction_double_cmpxchg_t fp_double_cmpxchg_weak;
    patomic_opsig_transaction_double_cmpxchg_t fp_double_cmpxchg_strong;
    patomic_opsig_transaction_n_cmpxchg_t fp_n_cmpxchg_weak;
    patomic_opsig_transaction_n_cmpxchg_t fp_n_cmpxchg_strong;
    patomic_opsig_transaction_generic_t fp_generic;
    patomic_opsig_transaction_generic_wfb_t fp_generic_wfb;
} patomic_ops_transaction_special_t;

typedef struct {
    patomic_opsig_transaction_tbegin_t fp_tbegin;
    patomic_opsig_transaction_tabort_t fp_tabort;
    patomic_opsig_transaction_tcommit_t fp_tcommit;
    patomic_opsig_transaction_ttest_t fp_ttest;
} patomic_ops_transaction_raw_t;

/* COMBINED OP STRUCTS */

typedef struct {
    patomic_opsig_store_t fp_store;
    patomic_opsig_load_t fp_load;
    patomic_ops_xchg_t xchg_ops;
    patomic_ops_bitwise_t bitwise_ops;
    patomic_ops_binary_t binary_ops;
    patomic_ops_arithmetic_t signed_ops;
    patomic_ops_arithmetic_t unsigned_ops;
} patomic_ops_t;

typedef struct {
    patomic_opsig_explicit_store_t fp_store;
    patomic_opsig_explicit_load_t fp_load;
    patomic_ops_explicit_xchg_t xchg_ops;
    patomic_ops_explicit_bitwise_t bitwise_ops;
    patomic_ops_explicit_binary_t binary_ops;
    patomic_ops_explicit_arithmetic_t signed_ops;
    patomic_ops_explicit_arithmetic_t unsigned_ops;
} patomic_ops_explicit_t;

typedef struct {
    patomic_opsig_transaction_store_t fp_store;
    patomic_opsig_transaction_load_t fp_load;
    patomic_ops_transaction_xchg_t xchg_ops;
    patomic_ops_transaction_bitwise_t bitwise_ops;
    patomic_ops_transaction_binary_t binary_ops;
    patomic_ops_transaction_arithmetic_t signed_ops;
    patomic_ops_transaction_arithmetic_t unsigned_ops;
    /* extra ops not available in implicit/explicit */
    patomic_ops_transaction_special_t special_ops;
    patomic_ops_transaction_flag_t flag_ops;
    patomic_ops_transaction_raw_t raw_ops;
} patomic_ops_transaction_t;


#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* !PATOMIC_OPS_H */
