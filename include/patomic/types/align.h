#ifndef PATOMIC_ALIGN_H
#define PATOMIC_ALIGN_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * ALIGN
 *
 * - these alignments are required for atomic operations on
 *   the specified type to be valid
 *
 * - recommended: the alignment required by the C language
 *                (always valid)
 * - minimum: the alignment required by the architecture
 *            (conditionally valid)
 * - size_within: the size and alignment of a hypothetical buffer
 *                within which the object must reside if "minimum"
 *                is to be considered valid
 *
 * NOTE:
 * - "recommended" and "minimum" are always a positive power of 2
 * - "size_within" has no restrictions on its value
 * - the "minimum" alignment is only valid if the object resides
 *   entirely within the buffer specified with "size_within"
 * - the purpose of this is to communicate on x86 that operations
 *   on a type are atomic if the object doesn't cross a cache line
 *
 * EXAMPLES:
 * - type: int, op: exchange
 *   - taken to have a size and alignment of 4
 *   - LOCK XCHG on x86, which has no alignment requirement
 *   - {.recommended=4, .minimum=1, .size_within=0}
 * - type: avx_128, op: load
 *   - taken to have a size and alignment of 16
 *   - atomic on x86 if it doesn't cross a cache-line boundary
 *   - cache-line size is taken to be 64 bytes
 *   - {.recommended=16, .minimum=1, .size_within=64}
 */

typedef struct {
    size_t recommended;
    size_t minimum;
    size_t size_within;
} patomic_align_t;

#ifdef __cplusplus
}
#endif

#endif /* !PATOMIC_ALIGN_H */
