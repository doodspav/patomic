#ifndef PATOMIC_STDLIB_CPUID_H
#define PATOMIC_STDLIB_CPUID_H

#include <patomic/patomic_export.h>

/* returns 1 if cpuid supported or 0 if not supported */
/* writes 12 byte vendor ID string to sig_out if cpuid supported */
PATOMIC_NO_EXPORT
extern int
__patomic_cpuid_sig(
    char *sig_out
);
#define patomic_cpuid_sig __patomic_cpuid_sig

/* leaf can be 0 for standard functions or 0x80000000 for extended functions */
/* returns max leaf value supported or 0 if cpuid not supported */
PATOMIC_NO_EXPORT
extern unsigned int
__patomic_cpuid_max(
    unsigned int leaf
);
#define patomic_cpuid_max __patomic_cpuid_max

/* returns 1 if cpuid supported or 0 if not supported */
PATOMIC_NO_EXPORT
extern int
__patomic_cpuid(
    unsigned int leaf,
    unsigned int *eax, unsigned int *ebx,
    unsigned int *ecx, unsigned int *edx
);
#define patomic_cpuid __patomic_cpuid

/* returns 1 if cpuid supported or 0 if not supported */
PATOMIC_NO_EXPORT
extern int
__patomic_cpuidex(
    unsigned int leaf, unsigned int subleaf,
    unsigned int *eax, unsigned int *ebx,
    unsigned int *exc, unsigned int *edx
);
#define patomic_cpuidex __patomic_cpuidex

#endif  /* !PATOMIC_STDLIB_CPUID_H */
