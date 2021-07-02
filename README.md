# patomic
This library provides portable access to lock-free atomic operations through a unified interface.  
It should compile on any standards compliant C90 compiler with no errors.


### Build
TODO

### Usage
TODO

### Guarantees
TODO

### Example
The only file you need to include is `patomic/patomic.h`, which works both in C and C++ (although the library still needs to be built as C++).
```c
#include <stdio.h>
#include <stdint.h>

#include <patomic/patomic.h>

int main()
{
    /* the "atomic" variable */
    volatile int obj = 5;

    /* create explicit struct */
    patomic_explicit_t p;
    p = patomic_create_explicit(
        sizeof obj,               /* byte width */
        patomic_options_DEFAULT,  /* options */
        0                         /* variadic parameter count */
    );

    /* check the alignment of our atomic object */
    uintptr_t addr = (uintptr_t) &obj;
    if (addr % p.align.recommended != 0) {
        printf("Failure: alignment does not meet requirement\n");
        return 1;
    }

    /* check the operation is supported */
    patomic_opsig_explicit_fetch_t fp_fadd_int;
    fp_fadd_int = p.ops.signed_ops.fp_fetch_add;
    if (fp_fadd_int == NULL) {
        printf("Unsupported operation: fetch_add on int\n");
        return 1;
    }

    /* perform addition */
    int arg = 10;
    int old;
    fp_fadd_int(
        &obj,             /* atomic object */
        &arg,             /* input: argument */
        patomic_SEQ_CST,  /* memory order */
        &old              /* output: return value */
    );

    /* final output */
    printf("New value: %d\n", obj);
    printf("Old value: %d\n", old);
    return 0;
}
```