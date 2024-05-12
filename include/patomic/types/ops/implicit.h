#ifndef PATOMIC_OPS_IMPLICIT_H
#define PATOMIC_OPS_IMPLICIT_H

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @addtogroup ops.implicit
 *
 * @brief
 *   Function signature for an atomic store operation with implicit memory
 *   order.
 *
 * @details
 *   Atomically stores a desired value into an object.
 *
 * @param obj
 *   Pointer to object into which to atomically store a value.
 *
 * @param desired
 *   Pointer to object holding value to be atomically stored.
 *
 * @note
 *   Width of all objects is the same, and is known implicitly.
 */
typedef void (* patomic_opsig_store_t) (
    volatile void *obj,
    const void *desired
);


/**
 * @addtogroup ops.implicit
 *
 * @brief
 *   Function signature for an atomic load operation with implicit memory order.
 *
 * @details
 *   Atomically loads a value from an object.
 *
 * @param obj
 *   Pointer to object from which a value will be atomically loaded.
 *
 * @param ret
 *   Pointer to object into which to write the atomically loaded value.
 *
 * @note
 *   Width of all objects is the same, and is known implicitly.
 */
typedef void (* patomic_opsig_load_t) (
    const volatile void *obj,
    void *ret
);


/**
 * @addtogroup ops.implicit
 *
 * @brief
 *   Function signature for an atomic exchange operation with implicit memory
 *   order.
 *
 * @details
 *   Atomically replaces the value of an object with a desired value and returns
 *   the value the object held previously, in a single read-modify-write atomic
 *   operation.
 *
 * @param obj
 *   Pointer to object whose value to atomically exchange with desired value.
 *
 * @param desired
 *   Pointer to object whose value will replace the existing value of "obj".
 *
 * @param ret
 *   Pointer to object into which to write the existing value held by "obj".
 *
 * @note
 *   Width of all objects is the same, and is known implicitly.
 */
typedef void (* patomic_opsig_exchange_t) (
    volatile void *obj,
    const void *desired,
    void *ret
);


/**
 * @addtogroup ops.implicit
 *
 * @brief
 *   Function signature for an atomic compare-exchange (cmpxchg) operation with
 *   implicit memory order.
 *
 * @details
 *   Atomically compares the value of the object with with the value of an
 *   expected object; if these compare equal, replaces the value of the object
 *   with a desired value and returns 1, otherwise stores the value of the
 *   object into the expected object, and returns 0. This is done in a single
 *   read-modify-write atomic operation if 1 is returned, otherwise this is a
 *   single atomic read (load) operation.
 *
 * @param obj
 *   Pointer to object whose value to atomically cmpxchg with expected object
 *   and desired value.
 *
 * @param expected
 *   Pointer to object whose value is compared against the existing value of
 *   "obj", and into which "obj"'s existing value will be stored if the
 *   comparison fails.
 *
 * @param desired
 *   Pointer to object whose value will replace the existing value of "obj".
 *
 * @returns
 *   The value 1 if the existing value of "obj" compares equal to the value of
 *   "expected", otherwise the value 0.
 *
 * @note
 *   Width of all objects is the same, and is known implicitly.
 */
typedef int (* patomic_opsig_cmpxchg_t) (
    volatile void *obj,
    void *expected,
    const void *desired
);


/**
 * @addtogroup ops.implicit
 *
 * @brief
 *   Function signature for an atomic bit test operation with implicit memory
 *   order.
 *
 * @details
 *   Atomically tests a single bit from an object, returning 1 if it is set and
 *   0 if it is not set. The whole object is atomically loaded in order to test
 *   the bit.
 *
 * @param obj
 *   Pointer to object from which bit will be atomically loaded.
 *
 * @param offset
 *   Zero-based bit index.
 *
 * @returns
 *   The value of the tested bit.
 *
 * @note
 *   Width of all objects is the same, and is known implicitly.
 */
typedef int (* patomic_opsig_test_t) (
    const volatile void *obj,
    int offset
);


/**
 * @addtogroup ops.implicit
 *
 * @brief
 *   Function signature for an atomic bit test-and-set operation with implicit
 *   memory order.
 *
 * @details
 *   Atomically modifies a single bit from an object with an implicitly known
 *   unary bitwise operation, returning the original value of the bit (0 or 1).
 *   The whole object is loaded and stored in a single atomic read-modify-write
 *   operation.
 *
 * @param obj
 *   Pointer to object from which bit will be atomically modified.
 *
 * @param offset
 *   Zero-based bit index.
 *
 * @returns
 *   The original value of the tested bit before modification.
 *
 * @note
 *   Width of all objects is the same, and is known implicitly.
 */
typedef int (* patomic_opsig_test_modify_t) (
    volatile void *obj,
    int offset
);


/**
 * @addtogroup ops.implicit
 *
 * @brief
 *   Function signature for an atomic fetch binary operation with implicit
 *   memory order.
 *
 * @details
 *   Atomically loads an object, performs an implicitly known binary operation
 *   on the value, and stores the result in a single atomic read-modify-write
 *   operation.
 *
 * @param obj
 *   Pointer to object whose value will be atomically modified, and passed as
 *   first parameter in the binary operation.
 *
 * @param arg
 *   Pointer to object whose value is passed as the second parameter in the
 *   binary operation.
 *
 * @param ret
 *   Pointer to object into which to write the original value of "obj" before
 *   modification.
 *
 * @note
 *   Width of all objects is the same, and is known implicitly.
 */
typedef void (* patomic_opsig_fetch_t) (
    volatile void *obj,
    const void *arg,
    void *ret
);


/**
 * @addtogroup ops.implicit
 *
 * @brief
 *   Function signature for an atomic fetch unary operation with implicit
 *   memory order.
 *
 * @details
 *   Atomically loads an object, performs an implicitly known unary operation on
 *   the value, and stores the result in a single atomic read-modify-write
 *   operation.
 *
 * @param obj
 *   Pointer to object whose value will be atomically modified, and passed as
 *   first parameter in the unary operation.
 *
 * @param ret
 *   Pointer to object into which to write the original value of "obj" before
 *   modification.
 *
 * @note
 *   Width of all objects is the same, and is known implicitly.
 */
typedef void (* patomic_opsig_fetch_noarg_t) (
    volatile void *obj,
    void *ret
);


/**
 * @addtogroup ops.implicit
 *
 * @brief
 *   Function signature for an atomic binary operation with implicit memory
 *   order.
 *
 * @details
 *   Atomically loads an object, performs an implicitly known binary operation
 *   on the value, and stores the result in a single atomic read-modify-write
 *   operation.
 *
 * @param obj
 *   Pointer to object whose value will be atomically modified, and passed as
 *   first parameter in the binary operation.
 *
 * @param arg
 *   Pointer to object whose value is passed as the second parameter in the
 *   binary operation.
 *
 * @note
 *   Width of all objects is the same, and is known implicitly.
 */
typedef void (* patomic_opsig_void_t) (
    volatile void *obj,
    const void *arg
);


/**
 * @addtogroup ops.implicit
 *
 * @brief
 *   Function signature for an atomic unary operation with implicit memory
 *   order.
 *
 * @details
 *   Atomically loads an object, performs an implicitly known unary operation on
 *   the value, and stores the result in a single atomic read-modify-write
 *   operation.
 *
 * @param obj
 *   Pointer to object whose value will be atomically modified, and passed as
 *   first parameter in the unary operation.
 *
 * @note
 *   Width of all objects is the same, and is known implicitly.
 */
typedef void (* patomic_opsig_void_noarg_t) (
    volatile void *obj
);


/**
 * @addtogroup ops.implicit
 *
 * @brief
 *   Set of function pointers for atomic arithmetic operations with implicit
 *   memory order. Pointers are NULL if operation is not supported.
 */
typedef struct {

    /** @brief Atomic addition using two's complement representation with
     *         implicit memory order. There is no undefined behaviour. */
    patomic_opsig_void_t fp_add;

    /** @brief Atomic subtraction using two's complement representation with
     *         implicit memory order. There is no undefined behaviour. */
    patomic_opsig_void_t fp_sub;

    /** @brief Atomic increment using two's complement representation with
     *         implicit memory order. There is no undefined behaviour. */
    patomic_opsig_void_noarg_t fp_inc;

    /** @brief Atomic decrement using two's complement representation with
     *         implicit memory order. There is no undefined behaviour. */
    patomic_opsig_void_noarg_t fp_dec;

    /** @brief Atomic negation using two's complement representation with
     *         implicit memory order. There is no undefined behaviour. */
    patomic_opsig_void_noarg_t fp_neg;

    /** @brief Atomic addition using two's complement representation with
     *         implicit memory order, returning original value from before
     *         operation. There is no undefined behaviour. */
    patomic_opsig_fetch_t fp_fetch_add;

    /** @brief Atomic subtraction using two's complement representation with
     *         implicit memory order, returning original value from before
     *         operation. There is no undefined behaviour. */
    patomic_opsig_fetch_t fp_fetch_sub;

    /** @brief Atomic increment using two's complement representation with
     *         implicit memory order, returning original value from before
     *         operation. There is no undefined behaviour. */
    patomic_opsig_fetch_noarg_t fp_fetch_inc;

    /** @brief Atomic decrement using two's complement representation with
     *         implicit memory order, returning original value from before
     *         operation. There is no undefined behaviour. */
    patomic_opsig_fetch_noarg_t fp_fetch_dec;

    /** @brief Atomic negation using two's complement representation with
     *         implicit memory order, returning original value from before
     *         operation. There is no undefined behaviour. */
    patomic_opsig_fetch_noarg_t fp_fetch_neg;

} patomic_ops_arithmetic_t;


/**
 * @addtogroup ops.implicit
 *
 * @brief
 *   Set of function pointers for atomic binary operations with implicit
 *   memory order. Pointers are NULL if operation is not supported.
 */
typedef struct {

    /** @brief Atomic OR with implicit memory order. */
    patomic_opsig_void_t fp_or;

    /** @brief Atomic XOR with implicit memory order. */
    patomic_opsig_void_t fp_xor;

    /** @brief Atomic AND with implicit memory order. */
    patomic_opsig_void_t fp_and;

    /** @brief Atomic NOT with implicit memory order. */
    patomic_opsig_void_noarg_t fp_not;

    /** @brief Atomic OR with implicit memory order, returning original
     *         value from before operation. */
    patomic_opsig_fetch_t fp_fetch_or;

    /** @brief Atomic XOR with implicit memory order, returning original
     *         value from before operation. */
    patomic_opsig_fetch_t fp_fetch_xor;

    /** @brief Atomic AND with implicit memory order, returning original
     *         value from before operation. */
    patomic_opsig_fetch_t fp_fetch_and;

    /** @brief Atomic NOT with implicit memory order, returning original
     *         value from before operation. */
    patomic_opsig_fetch_noarg_t fp_fetch_not;

} patomic_ops_binary_t;


/**
 * @addtogroup ops.implicit
 *
 * @brief
 *   Set of function pointers for atomic bitwise operations with implicit
 *   memory order. Pointers are NULL if operation is not supported.
 */
typedef struct {

    /** @brief Atomic bit test with implicit memory order, returning original
     *         bit value. */
    patomic_opsig_test_t fp_test;

    /** @brief Atomic bit test-and-complement with implicit memory order,
     *         returning original bit value from before operation. */
    patomic_opsig_test_modify_t fp_test_compl;

    /** @brief Atomic bit test-and-set with implicit memory order, returning
     *         original bit value from before operation. */
    patomic_opsig_test_modify_t fp_test_set;

    /** @brief Atomic bit test-and-reset with implicit memory order, returning
     *         original bit value from before operation. */
    patomic_opsig_test_modify_t fp_test_reset;

} patomic_ops_bitwise_t;


/**
 * @addtogroup ops.implicit
 *
 * @brief
 *   Set of function pointers for atomic exchange operations with implicit
 *   memory order. Pointers are NULL if operation is not supported.
 */
typedef struct {

    /** @brief Atomic exchange with implicit memory order. */
    patomic_opsig_exchange_t fp_exchange;

    /** @brief Atomic compare-exchange with implicit memory order. Operation may
     *         spuriously fail and act as if the object's value does not compare
     *         equal to the expected value. */
    patomic_opsig_cmpxchg_t fp_cmpxchg_weak;

    /** @brief Atomic compare-exchange with implicit memory order. Operation
     *         will never spuriously fail. */
    patomic_opsig_cmpxchg_t fp_cmpxchg_strong;

} patomic_ops_xchg_t;


/**
 * @addtogroup ops.implicit
 *
 * @brief
 *   Set of function pointers for atomic load and store operations and other
 *   sets of atomic operations, with implicit memory order. Pointers are NULL
 *   if operation is not supported.
 */
typedef struct {

    /** @brief Atomic store with implicit memory order. */
    patomic_opsig_store_t fp_store;

    /** @brief Atomic load with implicit memory order. */
    patomic_opsig_load_t fp_load;

    /** @brief Set of atomic xchg operations with implicit memory order. */
    patomic_ops_xchg_t xchg_ops;

    /** @brief Set of atomic bitwise operations with implicit memory order. */
    patomic_ops_bitwise_t bitwise_ops;

    /** @brief Set of atomic binary operations with implicit memory order. */
    patomic_ops_binary_t binary_ops;

    /** @brief Set of atomic arithmetic operations using two's complement
     *         representation with implicit memory order. */
    patomic_ops_arithmetic_t arithmetic_ops;

} patomic_ops_t;


#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* PATOMIC_OPS_IMPLICIT_H */
