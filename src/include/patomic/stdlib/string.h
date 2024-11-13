#ifndef PATOMIC_STDLIB_STRING_H
#define PATOMIC_STDLIB_STRING_H

#include <patomic/macros/restrict.h>

#include <stddef.h>


/**
 * @addtogroup stdlib
 *
 * @brief
 *   Copies 'count' characters from the buffer pointed to by 'src' into the
 *   buffer pointed to by 'dest'.
 *
 * @note
 *   Functions identically to C's memcpy.
 *
 * @param count
 *   Number of characters to copy from one buffer to the other.
 *
 * @param src
 *   Source buffer from which to copy the characters, interpreted as an array
 *   of 'unsigned char'.
 *
 * @param dest
 *   Destination buffer into which to copy the characters, interpreted as an
 *   array of 'unsigned char'.
 *
 * @return
 *   A copy of 'dest'.
 */
void *
patomic_memcpy(
    void *PATOMIC_RESTRICT dest,
    const void *PATOMIC_RESTRICT src,
    size_t count
);


#endif  /* PATOMIC_STDLIB_STRING_H */
