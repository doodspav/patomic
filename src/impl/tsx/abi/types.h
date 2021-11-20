#ifndef PATOMIC_IMPL_TSX_ABI_TYPES_H
#define PATOMIC_IMPL_TSX_ABI_TYPES_H

#include <stddef.h>

#include <patomic/patomic_config.h>

typedef struct {
    const char *data;
    size_t size;
} patomic_func_view_t;

typedef struct {
    patomic_func_view_t test_rtm;
    patomic_func_view_t cas_vptr;
    patomic_func_view_t cmpxchg_weak;
    patomic_func_view_t explicit_cmpxchg_weak;
    size_t magic_size;
} patomic_tsx_funcs_t;

typedef int (* patomic_tsx_test_rtm_t) \
    (void);

typedef void (* patomic_tsx_cas_vptr_t) \
    (void *volatile *obj,               \
     void **expected,                   \
     const void *desired);

typedef int (* patomic_tsx_cmpxchg_t) \
    (volatile void *obj,              \
     void *expected,                  \
     const void *desired);

typedef int (* patomic_tsx_explicit_cmpxchg_t) \
    (volatile void *obj,                       \
     void *expected,                           \
     const void *desired,                      \
     int,                                      \
     int);

static patomic_tsx_funcs_t
patomic_tsx_get_funcs(void);

#endif  /* !PATOMIC_IMPL_TSX_ABI_TYPES_H */
