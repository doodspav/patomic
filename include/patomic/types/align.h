#ifndef PATOMIC_ALIGN_H
#define PATOMIC_ALIGN_H

#include <stddef.h>
#include <patomic/patomic_export.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @addtogroup align
 *
 * @brief
 *   The alignments required for atomic operations on the specified type to be
 *   valid.
 *
 * @warning
 *   Not meeting the alignment requirements will result in undefined behaviour.
 *
 * @details
 *   - recommended: the alignment required by the C language (always valid)     \n
 *   - minimum: the alignment required by the architecture (conditionally valid)\n
 *   - size_within: the size and alignment of a hypothetical buffer within which
 *       the object is to be considered valid, unless it is 0 in which case
 *       "minimum" is always valid                                              \n
 *
 * @note
 *   - "recommended" and "minimum" are always a positive power of 2             \n
 *   - "minimum" is never larger than "recommended"                             \n
 *   - "size_within" has no restrictions on its value                           \n
 *   - the "minimum" alignment is only valid if the object resides entirely
 *     within the buffer specified with "size_within", unless "size_within" is
 *     0, in which case "minimum" is always valid                               \n
 *   - the intention of this is to communicate on x86 that operations on a type
 *     are atomic if the object doesn't cross a cache line                      \n
 *
 * @example
 *   - type: int (size=4, align=4)                                              \n
 *   - operation: exchange                                                      \n
 *   - potential assembly (x86): LOCK XCHG, no alignment required               \n
 *   - atomic if: always                                                        \n
 *   - example value: {.recommended=4, .minimum=1, .size_within=0}              \n
 *
 * @example
 *   - type: avx_128 (size=16, align=16)                                        \n
 *   - operation: load                                                          \n
 *   - potentially assembly (x86) VMOVDQU, no alignment required                \n
 *   - atomic if: whole object sits in a single cache line (e.g. 64 bytes)      \n
 *   - example value: {.recommended=16, .minimum=1, .sie_within=4}              \n
 */

typedef struct {
    size_t recommended;
    size_t minimum;
    size_t size_within;
} patomic_align_t;


/**
 * @addtogroup align
 *
 * @brief
 *   Represents the compile-time upper bound of the cache line size for the
 *   target platform.
 *
 * @warning
 *   Changing this value is an ABI break, requiring a major version bump.
 *
 * @note
 *   The value is always a power of 2.
 */

#undef PATOMIC_MAX_CACHE_LINE_SIZE
#define PATOMIC_MAX_CACHE_LINE_SIZE ((size_t) 128)


/**
 * @addtogroup align
 *
 * @brief
 *   Represents the compile-time upper bound of the cache line size for the
 *   target platform.
 *
 * @warning
 *   Changing this value is NOT an ABI break, with no major version bump needed.
 *   The benefit is that the value can be updated frequently to more closely
 *   match the true value on a wider range of platforms as they become known.
 *
 * @note
 *   The value is always a power of 2.
 */

#undef PATOMIC_MAX_CACHE_LINE_SIZE_ABI_UNSTABLE
#define PATOMIC_MAX_CACHE_LINE_SIZE_ABI_UNSTABLE ((size_t) 128)


/**
 * @addtogroup align
 *
 * @brief
 *   Returns the runtime upper bound of the cache line size for the target
 *   platform; may provide a more accurate value than the corresponding macros.
 *
 * @note
 *   The value will not return a value larger than either macro equivalent but
 *   may return a smaller value.
 *
 * @note
 *   The value returned is always a power of 2.
 */

PATOMIC_EXPORT size_t
patomic_cache_line_size(void);


/**
 * @addtogroup align
 *
 * @brief
 *   Checks that the pointer meets the recommended alignment according to the
 *   semantics of patomic_align_t.
 *
 * @note
 *   Internal casts may rely on implementation defined behaviour.
 *
 * @warning
 *   A successful check does not imply that the recommended alignment is a valid
 *   alignment for an object according to the C standard.
 */

PATOMIC_EXPORT int
patomic_align_meets_recommended(
    const volatile void *ptr,
    patomic_align_t align
);


/**
 * @addtogroup align
 *
 * @brief
 *   Checks that the buffer represented by the pointer and its width meets the
 *   minimum alignment according to the semantics of patomic_align_t.
 *
 * @note
 *   Internal casts may rely on implementation defined behaviour.
 *
 * @warning
 *   A successful check does not imply that the minimum alignment is a valid
 *   alignment for an object according to the C standard.
 */

PATOMIC_EXPORT int
patomic_align_meets_minimum(
    const volatile void *ptr,
    patomic_align_t align,
    size_t width
);


#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* PATOMIC_ALIGN_H */
