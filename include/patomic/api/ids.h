#ifndef PATOMIC_API_IDS_H
#define PATOMIC_API_IDS_H

#include <patomic/api/export.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @addtogroup impl
 *
 * @brief
 *   A set of globally unique constants used to identify which implementation(s)
 *   to use to provide atomic operations.
 *
 * @details
 *   - each id must have a unique name globally                                 \n
 *   - each id must have a unique value which is 0 or a positive power of 2     \n
 *   - the value of each implementation's id does not correspond to its kind    \n
 *   - the order and value of ids does not confer any ranking or priority       \n
 *
 * @note
 *   Unsigned long macros are used here rather than an enum in order to extend
 *   the number of implementations we can support (up to 32).
 *
 * @note
 *   The patomic_ids_ALL is all bits set rather than other values combined in
 *   order to avoid an ABI break if a new id is added.
 */
typedef unsigned long patomic_id_t;

/** @brief Value matching any and all implementation ids.
 *  @note  To get all currently available implementation ids at runtime, you
 *         can call patomic_get_ids(patomic_kinds_ALL). */
#define patomic_ids_ALL (~0ul)

/** @brief The id corresponding to the NULL implementation (i.e. none). */
#define patomic_id_NULL (0ul)

/** @brief The id corresponding to the C standard implementation. */
#define patomic_id_STDC (1ul << 0ul)


/**
 * @addtogroup impl
 *
 * @brief
 *   A set of globally unique constants used to provide a rough estimate as to
 *   the runtime overhead of each implementation.
 *
 * @details
 *   Values are ordered in ascending order based on call overhead, under the
 *   assumption that an inline ASM implementation will have the least overhead
 *   and an opaque call to a shared library will have the most.
 *
 * @warning
 *   Values are NOT ordered on quality of implementation. E.g. on x86 an ASM
 *   implementation of atomic_add is free to use a LOCK CMPXCHG loop over
 *   LOCK ADD.
 *
 * @note
 *   Implementations may have different kinds on different platforms.
 *
 * @note
 *   If an implementation fits into multiple kinds, it should be given the kind
 *   with the highest value.
 *
 * @note
 *   The BLTN (builtin) kind should only be used where the implementation makes
 *   a best-faith effort to provide an ASM implementation (e.g. with __atomic),
 *   otherwise LIB, OS, or DYN should be used.
 */
typedef enum {

    /** @brief The implementation kind is unknown. */
    patomic_kind_UNKN  = 0x0

    /** @brief The implementation calls a dynamically linked opaque function. */
    ,patomic_kind_DYN  = 0x1

    /** @brief The implementation calls an OS provided function. */
    ,patomic_kind_OS   = 0x2

    /** @brief The implementation calls a statically linked opaque function. */
    ,patomic_kind_LIB  = 0x4

    /** @brief The implementation calls a compiler builtin. */
    ,patomic_kind_BLTN = 0x8

    /** @brief The implementation uses inline assembly. */
    ,patomic_kind_ASM  = 0x10

    /** @brief Value matching any and all implementation kinds. */
    ,patomic_kinds_ALL = patomic_kind_DYN  |
                         patomic_kind_OS   |
                         patomic_kind_LIB  |
                         patomic_kind_BLTN |
                         patomic_kind_ASM

} patomic_kind_t;


/**
 * @addtogroup impl
 *
 * @param kinds
 *   One or more patomic_kind_t flags combined (ORd) together.
 *
 * @brief
 *   Returns all combined ids for all implementations whose kind matches at
 *   least one bit in kinds.
 */
PATOMIC_EXPORT unsigned long
patomic_get_ids(
    unsigned int kinds
);


/**
 * @addtogroup impl
 *
 * @param id
 *   Any single patomic_id_t flag that has a single bit set.
 *
 * @brief
 *   Returns the kind corresponding to the given id, or patomic_kind_UNKN for an
 *   invalid id or patomic_id_NULL.
 *
 * @warning
 *   The id MUST NOT have more than a single bit set. This means that you cannot
 *   pass patomic_id_ALL. This will always be asserted, even if NDEBUG is
 *   defined.
 */
PATOMIC_EXPORT unsigned int
patomic_get_kind(
    unsigned long id
);


#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* PATOMIC_API_IDS_H */
