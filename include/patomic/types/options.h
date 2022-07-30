#ifndef PATOMIC_OPTIONS_H
#define PATOMIC_OPTIONS_H

#ifdef __cplusplus
extern "C" {
#endif


/*
 * OPTIONS
 *
 * - options are passed directly to each implementation
 * - options do not affect the correctness of any implementation or op; you can
 *   use an op obtained when passing an option in every place you could use it
 *   when obtained without passing that option
 * - implementations are not required to do anything with the options passed to
 *   them, they are purely suggestions
 */

typedef enum {
    patomic_option_NONE = 0x0
} patomic_option_t;


#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* !PATOMIC_OPTIONS_H */
