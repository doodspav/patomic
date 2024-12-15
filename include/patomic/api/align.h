#ifndef PATOMIC_API_ALIGN_H
#define PATOMIC_API_ALIGN_H

#include <patomic/api/export.h>

#include <stddef.h>

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
 * @note
 *   All patomic APIs returning this type guarantee that "recommended" and
 *   "minimum" are a positive power of 2, and that "minimum" is never larger
 *   than "recommended". The "size_within" value will either be zero, or a
 *   positive power of 2.
 *
 * @details
 *   Within the semantics of this type, buffers are considered aligned to the
 *   "recommended" alignment if they meet its alignment requirement.           \n
 *   If "size_within" is zero, buffers are considered aligned to the "minimum"
 *   alignment if they meet its alignment requirement.                         \n
 *   However, if "size_within" is non-zero, then in addition to meeting the
 *   "minimum" alignment requirement, the buffer would also need to entirely
 *   reside within a buffer with size and alignment of "size_within".          \n
 *   The intention of this is to communicate that an object is suitably aligned
 *   if it does not cross a cache-line boundary.
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
 *   - potential assembly (x86): VMOVDQU, no alignment required                 \n
 *   - atomic if: whole object sits in a single cache line (e.g. 64 bytes)      \n
 *   - example value: {.recommended=16, .minimum=1, .size_within=64}            \n
 */
typedef struct {

    /** @brief Alignment required by the C language (always valid). */
    size_t recommended;

    /** @brief Alignment required by the architecture (conditionally valid). */
    size_t minimum;

    /** @brief
     *   Size and alignment of a hypothetical buffer within which the object
     *   must reside for "minimum" to be considered valid, unless it is 0 in
     *   which case "minimum" is always valid.
     */
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
 *   platform; may provide a more accurate value than the corresponding macro.
 *
 * @note
 *   The value will not return a value larger than its unstable macro
 *   equivalent but may return a smaller value.
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
 *   The check will always fail if the recommended alignment is not a positive
 *   power of 2.
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
 *   The check will always fail if the minimum alignment is not a positive
 *   power of 2.
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

#endif  /* PATOMIC_API_ALIGN_H */
