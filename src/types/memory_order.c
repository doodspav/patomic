#include <patomic/types/memory_order.h>


int
patomic_is_valid_order(int order)
{
    return PATOMIC_IS_VALID_ORDER(order);
}


int
patomic_is_valid_store_order(int order)
{
    return PATOMIC_IS_VALID_STORE_ORDER(order);
}


int patomic_is_valid_load_order(int order)
{
    return PATOMIC_IS_VALID_LOAD_ORDER(order);
}


int
patomic_is_valid_fail_order(int succ, int fail)
{
    return PATOMIC_IS_VALID_FAIL_ORDER(succ, fail);
}


int
patomic_cmpxchg_fail_order(int succ)
{
    return PATOMIC_CMPXCHG_FAIL_ORDER(succ);
}
