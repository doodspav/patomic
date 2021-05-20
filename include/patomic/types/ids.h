#ifndef PATOMIC_IDS_H
#define PATOMIC_IDS_H

#include <limits.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * CHOOSING AN IMPLEMENTATION ID
 *
 * - each id must have a unique name, and a unique valid int value
 * - implementations are ranked and prioritised based on their id
 * - lower value id ranks better than higher value id
 * - NULL will always have the highest value, and is used to make
 *   sure the register array is never empty (zero-sized arrays aren't
 *   standard), and if you want an empty implementation
 */

typedef enum {
    patomic_impl_id_MSVC
    ,patomic_impl_id_STD
    ,patomic_impl_id_NULL = INT_MAX
} patomic_impl_id_t;

#ifdef __cplusplus
}
#endif

#endif  /* !PATOMIC_IDS_H */
