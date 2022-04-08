#ifndef PATOMIC_IDS_H
#define PATOMIC_IDS_H

#include <patomic/patomic_export.h>

#ifdef __cplusplus
extern "C" {
#endif


/*
 * IMPLEMENTATION IDS
 *
 * - each id must have a unique name globally
 * - each id must have a unique value which is a positive power of 2, except for
 *   NULL which has the value 0
 * - the value of each implementation's id does not correspond to its kind
 * - the order and value of ids does not confer any ranking or priority
 *
 * - ALL is all bits set rather than other values combined to avoid an ABI break
 *   if a new id is added
 */

typedef enum {
    patomic_ids_ALL  = ~0
    ,patomic_id_NULL = 0x0
    ,patomic_id_STD  = 0x1
    ,patomic_id_MSVC = 0x2
    ,patomic_id_GNU  = 0x4
    ,patomic_id_TSX  = 0x8
} patomic_id_t;


/*
 * IMPLEMENTATION KINDS
 *
 * - DYN: dynamic function call that cannot be inlined, e.g. OS apis
 * - LIB: unknown library implementation which may be inlined
 * - ASM: precise instructions with no call overhead, e.g. _Interlocked
 * - BLTN: usually ASM but may fall back to LIB/DYN implementation under certain
 *         circumstances, e.g. __atomic -> libatomic for 16B operations
 *         most compiler builtins and stdatomic.h fall in this category
 * - UNKN: unknown kind
 *
 * - ALL is all bits set rather than other values combined to avoid an ABI break
 *   if a new id is added
 *
 * - values are ordered in ascending order based on efficiency (under the
 *   assumption that an inline ASM implementation will be the most efficient)
 *
 * Note:
 * - BLTN is only used where the implementation makes a best-faith effort to
 *   provide an ASM implementation; otherwise, LIB or DYN should be used
 * - LIB is still guaranteed to be lock-free and address-free (along with the
 *   other three groups)
 */

typedef enum {
    patomic_kinds_ALL  = ~0
    ,patomic_kind_UNKN = 0x0
    ,patomic_kind_DYN  = 0x1
    ,patomic_kind_LIB  = 0x2
    ,patomic_kind_BLTN = 0x4
    ,patomic_kind_ASM  = 0x8
} patomic_kind_t;


/*
 * IMPL GET
 *
 * - kinds: one or more kind_t flags combined
 * - id: any single id_t flag that has a single bit set
 *
 * WARNING:
 * - id MUST NOT have more than 1 bit set
 * - this means you cannot pass id_ALL
 * - this will always be asserted (even if NDEBUG is defined)
 */

/* returns all combined ids for implementations whose kind is in kinds */
PATOMIC_EXPORT unsigned int
patomic_get_ids(
    unsigned int kinds
);

/* returns kind for given id, or kind_UNKN for invalid id or id_NULL */
PATOMIC_EXPORT patomic_kind_t
patomic_get_kind(
    patomic_id_t id
);


#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* !PATOMIC_IDS_H */
