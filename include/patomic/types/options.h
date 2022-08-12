#ifndef PATOMIC_OPTIONS_H
#define PATOMIC_OPTIONS_H

#ifdef __cplusplus
extern "C" {
#endif


/*
 * OPTIONS
 *
 * - options are passed directly to each implementation, and it is up to each
 *   implementation to decide how to handle them
 * - options do not affect the correctness of any implementation or op; any op
 *   that is obtained both when passing and not passing an option is equally
 *   correct in terms of thread-safety and memory ordering
 * - options MAY affect constraints that do not affect thread-safety or memory
 *   ordering, namely alignment requirements and efficiency of implementation
 *
 * WARNING:
 * - you should be cautious about using an op obtained without an option in the
 *   same place as an op obtained with the option (and vice versa)
 * - if the option relaxes alignment requirements, then (without further checks)
 *   you could end up with misaligned memory access when using an op obtained
 *   without the option
 */

typedef enum {
    patomic_option_NONE = 0x0
} patomic_option_t;


#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* !PATOMIC_OPTIONS_H */
