/* Copyright (c) doodspav. */
/* SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception */

#include <patomic/api/memory_order.h>


int
patomic_is_valid_order(const int order)
{
    return PATOMIC_IS_VALID_ORDER(order);
}


int
patomic_is_valid_store_order(const int order)
{
    return PATOMIC_IS_VALID_STORE_ORDER(order);
}


int patomic_is_valid_load_order(const int order)
{
    return PATOMIC_IS_VALID_LOAD_ORDER(order);
}


int
patomic_is_valid_fail_order(const int succ, const int fail)
{
    return PATOMIC_IS_VALID_FAIL_ORDER(succ, fail);
}


int
patomic_cmpxchg_fail_order(const int succ)
{
    return PATOMIC_CMPXCHG_FAIL_ORDER(succ);
}
