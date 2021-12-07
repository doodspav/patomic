#include <patomic/types/transaction.h>
#include <patomic/macros/force_inline.h>


PATOMIC_FORCE_INLINE unsigned char
patomic_transaction_abort_reason(
    unsigned int status
)
{
    if (!(status & patomic_TABORT_EXPLICIT)) { return 0; }
    /* uint will be at least 16bits */
    /* first 8 bits are status, second 8bits are reason */
    status >>= 8u;
    status &= 0xFFu;
    return (unsigned char) status;
}
