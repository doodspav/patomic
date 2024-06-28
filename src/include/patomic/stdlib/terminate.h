#ifndef PATOMIC_STDLIB_TERMINATE_H
#define PATOMIC_STDLIB_TERMINATE_H

#include <patomic/macros/noreturn.h>


/**
 * @addtogroup stdlib
 *
 * @brief
 *   Performs any cleanup deemed necessary by this library and then calls
 *   abort().
 */
PATOMIC_NORETURN void
patomic_terminate(void);


#endif  /* PATOMIC_STDLIB_TERMINATE_H */
