// Copyright (c) doodspav.
// SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception

#include <test/common/transaction.hpp>

namespace test
{


generic_cmpxchg::generic_cmpxchg(
    std::size_t width, std::size_t align, bool is_signed
)
    : object(width, align, is_signed),
      expected(width, align, is_signed),
      desired(width, align, is_signed)
{}


generic_cmpxchg::operator patomic_transaction_cmpxchg_t() noexcept
{
    patomic_transaction_cmpxchg_t ret;
    ret.obj = object;
    ret.expected = expected;
    ret.desired = desired;
    return ret;
}


}  // namespace test


namespace test
{
namespace _detail
{


#define DEFN_OVERLOAD_CALL_PATOMIC_OP(fp_type)      \
    void                                            \
    call_patomic_op(                                \
        const fp_type fp,                           \
        void *const ptr,                            \
        const patomic_transaction_config_t& config, \
        patomic_transaction_result_t *const result  \
    ) noexcept

#define DEFN_OVERLOAD_CALL_PATOMIC_OP_WFB(fp_type)      \
    void                                                \
    call_patomic_op(                                    \
        const fp_type fp,                               \
        void *const ptr,                                \
        const patomic_transaction_config_wfb_t& config, \
        patomic_transaction_result_wfb_t *const result  \
    ) noexcept


DEFN_OVERLOAD_CALL_PATOMIC_OP(patomic_opsig_transaction_store_t)
{
    fp(ptr, ptr, config, result);
}

DEFN_OVERLOAD_CALL_PATOMIC_OP(patomic_opsig_transaction_load_t)
{
    fp(ptr, ptr, config, result);
}

DEFN_OVERLOAD_CALL_PATOMIC_OP(patomic_opsig_transaction_exchange_t)
{
    fp(ptr, ptr, ptr, config, result);
}

DEFN_OVERLOAD_CALL_PATOMIC_OP_WFB(patomic_opsig_transaction_cmpxchg_t)
{
    fp(ptr, ptr, ptr, config, result);
}

DEFN_OVERLOAD_CALL_PATOMIC_OP(patomic_opsig_transaction_test_t)
{
    fp(ptr, 0, config, result);
}

DEFN_OVERLOAD_CALL_PATOMIC_OP(patomic_opsig_transaction_test_modify_t)
{
    fp(ptr, 0, config, result);
}

// fetch_t == exchange_t

DEFN_OVERLOAD_CALL_PATOMIC_OP(patomic_opsig_transaction_fetch_noarg_t)
{
    fp(ptr, ptr, config, result);
}

// void_t == store_t

DEFN_OVERLOAD_CALL_PATOMIC_OP(patomic_opsig_transaction_void_noarg_t)
{
    fp(ptr, config, result);
}

DEFN_OVERLOAD_CALL_PATOMIC_OP_WFB(patomic_opsig_transaction_double_cmpxchg_t)
{
    const patomic_transaction_cmpxchg_t cxa { ptr, ptr, ptr };
    const patomic_transaction_cmpxchg_t cxb = cxa;
    fp(cxa, cxb, config, result);
}

DEFN_OVERLOAD_CALL_PATOMIC_OP_WFB(patomic_opsig_transaction_multi_cmpxchg_t)
{
    const patomic_transaction_cmpxchg_t cx { ptr, ptr, ptr };
    fp(&cx, 1u, config, result);
}

DEFN_OVERLOAD_CALL_PATOMIC_OP(patomic_opsig_transaction_generic_t)
{
    const auto noop = [](void *) {};
    fp(noop, ptr, config, result);
}

DEFN_OVERLOAD_CALL_PATOMIC_OP_WFB(patomic_opsig_transaction_generic_wfb_t)
{
    const auto noop = [](void *) {};
    fp(noop, ptr, noop, ptr, config, result);
}


}  // namespace _detail
}   // namespace test