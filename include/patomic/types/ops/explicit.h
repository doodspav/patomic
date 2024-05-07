#ifndef PATOMIC_OPS_EXPLICIT_H
#define PATOMIC_OPS_EXPLICIT_H

// TODO: extern "C"


/**
 * @addtogroup ops.explicit
 *
 * @brief
 *   Function signature for an atomic store operation with explicit memory
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
 * @param order
 *   Memory order used for atomic operation. Must be a valid store order.
 *
 * @note
 *   Width of all objects is the same, and is known implicitly.
 */
typedef void (* patomic_opsig_store_t) (
    volatile void *obj,
    const void *desired,
    int order
);


/**
 * @addtogroup ops.explicit
 *
 * @brief
 *   Function signature for an atomic load operation with explicit memory order.
 *
 * @details
 *   Atomically loads a value from an object.
 *
 * @param obj
 *   Pointer to object from which a value will be atomically loaded.
 *
 * @param order
 *   Memory order used for atomic operation. Must be a valid load order.
 *
 * @param ret
 *   Pointer to object into which to write the atomically loaded value.
 *
 * @note
 *   Width of all objects is the same, and is known implicitly.
 */
typedef void (* patomic_opsig_load_t) (
    const volatile void *obj,
    int order,
    void *ret
);


/**
 * @addtogroup ops.explicit
 *
 * @brief
 *   Function signature for an atomic exchange operation with explicit memory
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
 * @param order
 *   Memory order used for atomic operation.
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
    int order,
    void *ret
);


/**
 * @addtogroup ops.explicit
 *
 * @brief
 *   Function signature for an atomic compare-exchange (cmpxchg) operation with
 *   explicit memory order.
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
 * @param succ
 *   Memory order used for read-modify-write atomic operation on success path.
 *
 * @param fail
 *   Memory order used for read (load) atomic operation on failure path. Must be
 *   a valid fail order.
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
    const void *desired,
    int succ,
    int fail
);


/**
 * @addtogroup ops.explicit
 *
 * @brief
 *   Function signature for an atomic bit test operation wth explicit memory
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
 * @param order
 *   Memory order used for atomic operation. Must be a valid load order.
 *
 * @returns
 *   The value of the tested bit.
 *
 * @note
 *   Width of all objects is the same, and is known implicitly.
 */
typedef int (* patomic_opsig_test_t) (
    const volatile void *obj,
    int offset,
    int order
);


/**
 * @addtogroup ops.explicit
 *
 * @brief
 *   Function signature for an atomic bit test-and-set operation with explicit
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
 * @param order
 *   Memory order used for atomic operation.
 *
 * @returns
 *   The original value of the tested bit before modification.
 *
 * @note
 *   Width of all objects is the same, and is known implicitly.
 */
typedef int (* patomic_opsig_test_modify_t) (
    volatile void *obj,
    int offset,
    int order
);


/**
 * @addtogroup ops.explicit
 *
 * @brief
 *   Function signature for an atomic fetch binary operation with explicit
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
 * @param order
 *   Memory order used for atomic operation.
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
    int order,
    void *ret
);


/**
 * @addtogroup ops.explicit
 *
 * @brief
 *   Function signature for an atomic fetch unary operation with explicit
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
 * @param order
 *   Memory order used for atomic operation.
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
    int order,
    void *ret
);


/**
 * @addtogroup ops.explicit
 *
 * @brief
 *   Function signature for an atomic binary operation with explicit memory
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
 * @param order
 *   Memory order used for atomic operation.
 *
 * @note
 *   Width of all objects is the same, and is known implicitly.
 */
typedef void (* patomic_opsig_void_t) (
    volatile void *obj,
    const void *arg,
    int order
);


/**
 * @addtogroup ops.explicit
 *
 * @brief
 *   Function signature for an atomic unary operation with explicit memory
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
 * @param order
 *   Memory order used for atomic operation.
 *
 * @note
 *   Width of all objects is the same, and is known implicitly.
 */
typedef void (* patomic_opsig_void_noarg_t) (
    volatile void *obj,
    int order
);


#endif  /* PATOMIC_OPS_EXPLICIT_H */
