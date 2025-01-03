/* Copyright (c) doodspav. */
/* SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception */

#ifndef PATOMIC_API_OPTIONS_H
#define PATOMIC_API_OPTIONS_H

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @addtogroup options
 *
 * @brief
 *   Enum constants providing hints to implementations that allow them to
 *   optimize how they implement certain atomic operations.
 *
 * @details
 *   Options do NOT affect the correctness of any implementation. Any atomic
 *   operation that is obtained both when passing and not passing an option is
 *   equally correct in terms of thread-safety and memory ordering.             \n
 *   However, options MAY affect constraints that are unrelated to thread-safety
 *   and memory ordering, namely alignment requirements and the quality of the
 *   implementation.
 *
 * @warning
 *   You should be cautious about using an atomic operation obtained without
 *   an option in the same place as one obtained with the option (and vice
 *   versa). Options are allowed to relax alignment requirements, and you could
 *   end up with misaligned memory accesses by mistake.
 *
 * @note
 *   Options are merely hints to an implementation; they may be completely
 *   ignored.
 *
 * @note
 *   Each "option" label has a single bit set (except for NONE which has no
 *   bits set), and each "options" label has more than one bit set.
 */
typedef enum {

    /** @brief The empty option hinting nothing */
    patomic_option_NONE = 0x0

} patomic_option_t;


#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* PATOMIC_API_OPTIONS_H */
