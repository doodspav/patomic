#ifndef PATOMIC_STDLIB_STDALIGN_H
#define PATOMIC_STDLIB_STDALIGN_H

#include <stddef.h>
#include <patomic/patomic_config.h>
#include <patomic/patomic_export.h>

PATOMIC_NO_EXPORT
extern size_t
__patomic_alignment_from_size(
    size_t size
);
#define patomic_alignment_from_size __patomic_alignment_from_size

PATOMIC_NO_EXPORT
extern int
__patomic_is_aligned(
    const volatile void *ptr,
    size_t align
);
#define patomic_is_aligned __patomic_is_aligned

#if PATOMIC_HAVE_ALIGNOF
    #define patomic_alignof_type(t) _Alignof(t)
#elif PATOMIC_HAVE_MS_ALIGNOF_ALIGN_DSPC
    #define patomic_alignof_type(t) __alignof(t)
#elif PATOMIC_HAVE_GNU_ALIGNOF_ALIGNED_ATTR
    #define patomic_alignof_type(t) __alignof__(t)
#else
    #define patomic_alignof_type(t) __patomic_align_from_size(sizeof(t))
#endif

#endif  /* !PATOMIC_STDLIB_STDALIGN_H */
