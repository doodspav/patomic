// Copyright (c) doodspav.
// SPDX-License-Identifier: LGPL-3.0-only WITH LGPL-3.0-linking-exception

#include <patomic/patomic.h>

#include <test/common/death.hpp>
#include <test/common/generic_int.hpp>
#include <test/common/params.hpp>

#include <gtest/gtest.h>


/// @brief
///   Improvement over non-vec variant where vec is a vector of pointers and n
///   is the number of pointers used by fp.
#define ASSERT_DEATH_IF_NON_NULL_VEC(vec, n, fp, ...)      \
    {                                                      \
        if (fp != nullptr)                                 \
        {                                                  \
            void *const old = vec[0];                      \
            vec[0] = nullptr;                              \
            auto begin = vec.begin();                      \
            auto end = std::next(begin, n);                \
            for (std::size_t v = 0; v < vec.size(); ++v,   \
                 std::rotate(begin, begin + 1, end))       \
            {                                              \
                ASSERT_DEATH_IF_NON_NULL(fp, __VA_ARGS__); \
            }                                              \
            vec[0] = old;                                  \
        }                                                  \
    }                                                      \
    static_assert(true, "require semi-colon")


/// @brief Test fixture.
class BtDeathNullParams : public testing::Test
{};


/// @brief Check that calling an implicit atomic operation with any pointer
///        param as null is asserted.
TEST_F(BtDeathNullParams, implicit)
{
    // go through all params
    const auto params = test::ParamsImplicit::combinations();
    for (const auto& param : params)
    {
        // add trace
        SCOPED_TRACE(param.as_test_suffix());
        
        // get operations
        const auto pao = patomic_create(
            param.width,
            param.order,
            param.options,
            patomic_kinds_ALL,
            param.id
        );
        const auto& ops = pao.ops;
        
        // create vector of max number of pointers we'll need
        test::generic_integer gi { param.width, pao.align.recommended, false };
        std::vector<void *> ptrs;
        ptrs.resize(3, gi);
        const auto& a = ptrs[0];
        const auto& b = ptrs[1];
        const auto& c = ptrs[2];
        
        // attempt to call operations if non-null with pointers

        // load/store
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 2, ops.fp_store, a, b);
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 2, ops.fp_load, a, b);

        // xchg
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 3, ops.xchg_ops.fp_exchange, a, b, c);
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 3, ops.xchg_ops.fp_cmpxchg_weak, a, b, c);
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 3, ops.xchg_ops.fp_cmpxchg_strong, a, b, c);

        // bitwise
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 1, ops.bitwise_ops.fp_test, a, 0);
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 1, ops.bitwise_ops.fp_test_compl, a, 0);
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 1, ops.bitwise_ops.fp_test_set, a, 0);
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 1, ops.bitwise_ops.fp_test_reset, a, 0);

        // binary
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 2, ops.binary_ops.fp_or, a, b);
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 2, ops.binary_ops.fp_xor, a, b);
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 2, ops.binary_ops.fp_and, a, b);
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 1, ops.binary_ops.fp_not, a);
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 3, ops.binary_ops.fp_fetch_or, a, b, c);
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 3, ops.binary_ops.fp_fetch_xor, a, b, c);
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 3, ops.binary_ops.fp_fetch_and, a, b, c);
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 2, ops.binary_ops.fp_fetch_not, a, b);

        // arithmetic
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 2, ops.arithmetic_ops.fp_add, a, b);
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 2, ops.arithmetic_ops.fp_sub, a, b);
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 1, ops.arithmetic_ops.fp_inc, a);
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 1, ops.arithmetic_ops.fp_dec, a);
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 1, ops.arithmetic_ops.fp_neg, a);
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 3, ops.arithmetic_ops.fp_fetch_add, a, b, c);
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 3, ops.arithmetic_ops.fp_fetch_sub, a, b, c);
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 2, ops.arithmetic_ops.fp_fetch_inc, a, b);
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 2, ops.arithmetic_ops.fp_fetch_dec, a, b);
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 2, ops.arithmetic_ops.fp_fetch_neg, a, b);
    }
}


/// @brief Check that calling an explicit atomic operation with any pointer
///        param as null is asserted.
TEST_F(BtDeathNullParams, explicit)
{
    // go through all params
    const auto params = test::ParamsExplicit::combinations();
    for (const auto& param : params)
    {
        // add trace
        SCOPED_TRACE(param.as_test_suffix());

        // get operations
        const auto pao = patomic_create_explicit(
            param.width,
            param.options,
            patomic_kinds_ALL,
            param.id
        );
        const auto& ops = pao.ops;

        // create vector of max number of pointers we'll need
        test::generic_integer gi { param.width, pao.align.recommended, false };
        std::vector<void *> ptrs;
        ptrs.resize(3, gi);
        const auto& a = ptrs[0];
        const auto& b = ptrs[1];
        const auto& c = ptrs[2];

        // attempt to call operations if non-null with pointers

        // load/store
        if (patomic_is_valid_store_order(param.order))
        {
            ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 2, ops.fp_store, a, b, param.order);
        }
        if (patomic_is_valid_load_order(param.order))
        {
            ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 2, ops.fp_load, a, param.order, b);
        }

        // xchg
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 3, ops.xchg_ops.fp_exchange, a, b, param.order, c);
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 3, ops.xchg_ops.fp_cmpxchg_weak, a, b, c, param.order, patomic_RELAXED);
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 3, ops.xchg_ops.fp_cmpxchg_strong, a, b, c, param.order, patomic_RELAXED);

        // bitwise
        if (patomic_is_valid_load_order(param.order))
        {
            ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 1, ops.bitwise_ops.fp_test, a, 0, param.order);
        }
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 1, ops.bitwise_ops.fp_test_compl, a, 0, param.order);
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 1, ops.bitwise_ops.fp_test_set, a, 0, param.order);
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 1, ops.bitwise_ops.fp_test_reset, a, 0, param.order);

        // binary
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 2, ops.binary_ops.fp_or, a, b, param.order);
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 2, ops.binary_ops.fp_xor, a, b, param.order);
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 2, ops.binary_ops.fp_and, a, b, param.order);
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 1, ops.binary_ops.fp_not, a, param.order);
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 3, ops.binary_ops.fp_fetch_or, a, b, param.order, c);
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 3, ops.binary_ops.fp_fetch_xor, a, b, param.order, c);
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 3, ops.binary_ops.fp_fetch_and, a, b, param.order, c);
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 2, ops.binary_ops.fp_fetch_not, a, param.order, b);

        // arithmetic
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 2, ops.arithmetic_ops.fp_add, a, b, param.order);
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 2, ops.arithmetic_ops.fp_sub, a, b, param.order);
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 1, ops.arithmetic_ops.fp_inc, a, param.order);
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 1, ops.arithmetic_ops.fp_dec, a, param.order);
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 1, ops.arithmetic_ops.fp_neg, a, param.order);
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 3, ops.arithmetic_ops.fp_fetch_add, a, b, param.order, c);
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 3, ops.arithmetic_ops.fp_fetch_sub, a, b, param.order, c);
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 2, ops.arithmetic_ops.fp_fetch_inc, a, param.order, b);
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 2, ops.arithmetic_ops.fp_fetch_dec, a, param.order, b);
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 2, ops.arithmetic_ops.fp_fetch_neg, a, param.order, b);
    }
}


/// @brief Check that calling a transaction atomic operation with any pointer
///        param as null is asserted.
TEST_F(BtDeathNullParams, transaction)
{
    // go through all params
    const auto params = test::ParamsTransaction::combinations();
    for (const auto& param : params)
    {
        // add trace
        SCOPED_TRACE(param.as_test_suffix());

        // get operations
        const auto pao = patomic_create_transaction(
            param.options,
            patomic_kinds_ALL,
            param.id
        );
        const auto& ops = pao.ops;

        // create vector of max number of pointers we'll need
        unsigned char uc {};
        std::vector<void *> ptrs;
        ptrs.resize(7, &uc);
        const auto& a = ptrs[0];
        const auto& b = ptrs[1];
        const auto& c = ptrs[2];
        const auto& d = ptrs[3];
        const auto& e = ptrs[4];
        const auto& f = ptrs[5];
        const auto& g = ptrs[6];

        // helpers to get non-void types lazily
        const auto as_res = [](void *p) noexcept {
            return reinterpret_cast<patomic_transaction_result_t *>(p);
        };
        const auto as_res_wfb = [](void *p) noexcept {
            return reinterpret_cast<patomic_transaction_result_wfb_t *>(p);
        };
        const auto as_cx = [](void *p, void *q, void *r) noexcept {
            patomic_transaction_cmpxchg_t cx {};
            cx.obj = p;
            cx.expected = q;
            cx.desired = r;
            return cx;
        };
        const auto as_cxs = [=](void *p, void *q, void *r, void *s, void *t, void *u) noexcept {
            static patomic_transaction_cmpxchg_t cxs[2] {};
            cxs[0] = as_cx(p, q, r);
            cxs[1] = as_cx(s, t, u);
            return &cxs[0];
        };
        const auto as_fn = [](void *p) noexcept {
            return reinterpret_cast<void(*)(void *)>(p);
        };
        const auto as_flag = [](void *p) noexcept {
            return reinterpret_cast<patomic_transaction_flag_t *>(p);
        };

        // valid configurations that won't skip any null checks
        patomic_transaction_config_t cfg {};
        cfg.width = 1;
        cfg.attempts = 1;
        patomic_transaction_config_wfb_t cfg_wfb {};
        cfg_wfb.width = 1;
        cfg_wfb.attempts = 1;
        cfg_wfb.fallback_attempts = 1;

        // attempt to call operations if non-null with pointers

        // load/store
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 3, ops.fp_store, a, b, cfg, as_res(c));
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 3, ops.fp_load, a, b, cfg, as_res(c));

        // xchg
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 4, ops.xchg_ops.fp_exchange, a, b, c, cfg, as_res(d));
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 4, ops.xchg_ops.fp_cmpxchg_weak, a, b, c, cfg_wfb, as_res_wfb(d));
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 4, ops.xchg_ops.fp_cmpxchg_strong, a, b, c, cfg_wfb, as_res_wfb(d));

        // bitwise
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 2, ops.bitwise_ops.fp_test, a, 0, cfg, as_res(b));
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 2, ops.bitwise_ops.fp_test_compl, a, 0, cfg, as_res(b));
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 2, ops.bitwise_ops.fp_test_set, a, 0, cfg, as_res(b));
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 2, ops.bitwise_ops.fp_test_reset, a, 0, cfg, as_res(b));

        // binary
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 3, ops.binary_ops.fp_or, a, b, cfg, as_res(c));
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 3, ops.binary_ops.fp_xor, a, b, cfg, as_res(c));
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 3, ops.binary_ops.fp_and, a, b, cfg, as_res(c));
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 2, ops.binary_ops.fp_not, a, cfg, as_res(b));
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 4, ops.binary_ops.fp_fetch_or, a, b, c, cfg, as_res(d));
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 4, ops.binary_ops.fp_fetch_xor, a, b, c, cfg, as_res(d));
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 4, ops.binary_ops.fp_fetch_and, a, b, c, cfg, as_res(d));
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 3, ops.binary_ops.fp_fetch_not, a, b, cfg, as_res(c));

        // arithmetic
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 3, ops.arithmetic_ops.fp_add, a, b, cfg, as_res(c));
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 3, ops.arithmetic_ops.fp_sub, a, b, cfg, as_res(c));
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 2, ops.arithmetic_ops.fp_inc, a, cfg, as_res(b));
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 2, ops.arithmetic_ops.fp_dec, a, cfg, as_res(b));
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 2, ops.arithmetic_ops.fp_neg, a, cfg, as_res(b));
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 4, ops.arithmetic_ops.fp_fetch_add, a, b, c, cfg, as_res(d));
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 4, ops.arithmetic_ops.fp_fetch_sub, a, b, c, cfg, as_res(d));
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 3, ops.arithmetic_ops.fp_fetch_inc, a, b, cfg, as_res(c));
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 3, ops.arithmetic_ops.fp_fetch_dec, a, b, cfg, as_res(c));
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 3, ops.arithmetic_ops.fp_fetch_neg, a, b, cfg, as_res(c));

        // special
        unsigned char unused {};
        void *const _ = &unused;
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 7, ops.special_ops.fp_double_cmpxchg, as_cx(a, b, c), as_cx(d, e, f), cfg_wfb, as_res_wfb(g));
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 7, ops.special_ops.fp_multi_cmpxchg, as_cxs(a, b, c, d, e, f), 2, cfg_wfb, as_res_wfb(g));
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 2, ops.special_ops.fp_generic, as_fn(a), _, cfg, as_res(b));
        ASSERT_DEATH_IF_NON_NULL_VEC(ptrs, 3, ops.special_ops.fp_generic_wfb, as_fn(a), _, as_fn(b), _, cfg_wfb, as_res_wfb(c));

        // flag
        ASSERT_DEATH_IF_NON_NULL(ops.flag_ops.fp_test, as_flag(a));
        ASSERT_DEATH_IF_NON_NULL(ops.flag_ops.fp_test_set, as_flag(a));
        ASSERT_DEATH_IF_NON_NULL(ops.flag_ops.fp_clear, as_flag(a));
    }
}
