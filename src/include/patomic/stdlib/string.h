#ifndef PATOMIC_STDLIB_STRING_H
#define PATOMIC_STDLIB_STRING_H

#include <stddef.h>

#include <patomic/patomic_export.h>

#include <patomic/macros/force_inline.h>
#include <patomic/macros/restrict.h>


PATOMIC_NO_EXPORT
PATOMIC_FORCE_INLINE
extern void *
__patomic_string_memcpy(
    void *PATOMIC_RESTRICT dst,
    const void *PATOMIC_RESTRICT src,
    size_t size
);
#define patomic_memcpy __patomic_string_memcpy

PATOMIC_NO_EXPORT
PATOMIC_FORCE_INLINE
extern void *
__patomic_string_memmove(
    void *PATOMIC_RESTRICT dst,
    const void *PATOMIC_RESTRICT src,
    size_t size
);
#define patomic_memmove __patomic_string_memmove

PATOMIC_NO_EXPORT
PATOMIC_FORCE_INLINE
extern void *
__patomic_string_memset(
    void *dst,
    int ch,
    size_t size
);
#define patomic_memset __patomic_string_memset

PATOMIC_NO_EXPORT
PATOMIC_FORCE_INLINE
extern int
__patomic_string_memcmp(
    const void *lhs,
    const void *rhs,
    size_t size
);
#define patomic_memcmp __patomic_string_memcmp


#endif  /* !PATOMIC_STDLIB_STRING_H */
