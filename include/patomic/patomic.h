#ifndef PATOMIC_PATOMIC_H
#define PATOMIC_PATOMIC_H

#include <patomic/patomic_export.h>

#ifdef __cplusplus
extern "C" {
#endif


PATOMIC_EXPORT int
patomic_example_add(
    int a,
    int b
);


#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* PATOMIC_PATOMIC_H */
