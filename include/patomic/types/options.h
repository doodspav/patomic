#ifndef PATOMIC_OPTIONS_H
#define PATOMIC_OPTIONS_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    patomic_options_DEFAULT = 0
    ,patomic_options_PRIORITISE_ARG_IDS = 1
    ,patomic_options_IGNORE_NON_ARG_IDS = 2
} patomic_options_t;

#ifdef __cplusplus
}
#endif

#endif  /* !PATOMIC_OPTIONS_H */
