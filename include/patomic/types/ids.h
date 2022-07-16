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
 * - ids are guaranteed to be consecutive powers of 2, starting at 1, with
 *   id_NULL having the value of 0
 *
 * - unsigned long macros are used here rather than an enum to extend the number
 *   of implementations we can support (up to 32)
 * - ids_ALL is all bits set rather than other values combined to avoid an ABI
 *   break if a new id is added
 */

typedef unsigned long patomic_id_t;

#define patomic_id_NULL (0ul)
#define patomic_id_STD  (1ul << 0ul)
#define patomic_id_MSVC (1ul << 1ul)
#define patomic_id_GNU  (1ul << 2ul)
#define patomic_id_TSX  (1ul << 3ul)

#define patomic_ids_ALL (~0ul)


/*
 * IMPLEMENTATION KINDS
 *
 * - UNKN: unknown kind (no implementation will have this kind)
 * - DYN:  dynamic function call that cannot be inlined
 * - OS:   implementation provided by host OS, e.g. Interlocked, atomic_ops(3C)
 * - LIB:  statically linked library implementation which may be inlined
 * - BLTN: usually ASM but may fall back to LIB/OS/DYN implementation under
 *         certain circumstances, e.g. __atomic -> libatomic for 16B operations
 *         most compiler builtins and stdatomic.h macros fall in this category
 * - ASM: precise instructions with no call overhead, e.g. _Interlocked
 *
 * - values are ordered in ascending order based on call overhead (under the
 *   assumption that an inline ASM implementation will have the least overhead)
 * - values are NOT ordered on quality of implementation (i.e. on x86 an ASM
 *   implementation of add is free to use a LOCK CMPXCHG loop over LOCK XADD)
 *
 * Note:
 * - implementations may have different kinds on different platforms
 * - BLTN is only used where the implementation makes a best-faith effort to
 *   provide an ASM implementation, otherwise, LIB, OS, or DYN should be used
 */

typedef enum {
    patomic_kind_UNKN  = 0x0
    ,patomic_kind_DYN  = 0x1
    ,patomic_kind_OS   = 0x2
    ,patomic_kind_LIB  = 0x4
    ,patomic_kind_BLTN = 0x8
    ,patomic_kind_ASM  = 0x10
    ,patomic_kinds_ALL = patomic_kind_DYN  |
                         patomic_kind_OS   |
                         patomic_kind_LIB  |
                         patomic_kind_BLTN |
                         patomic_kind_ASM
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
PATOMIC_EXPORT unsigned long
patomic_get_ids(
    unsigned int kinds
);

/* returns kind for given id, or kind_UNKN for invalid id or id_NULL */
PATOMIC_EXPORT unsigned int
patomic_get_kind(
    unsigned long id
);


#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* !PATOMIC_IDS_H */
