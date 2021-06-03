#include <cstring>
#include <random>
#include <type_traits>
#include <vector>

#include <patomic_test/patomic_test.hpp>
#include <patomic_test/aligned_buffer.hpp>
#include <patomic_test/curry_op.hpp>

#include <patomic/patomic.h>
#include <gtest/gtest.h>


class BufferOpsLogicTestFixture
    : public testing::TestWithParam<patomic::test::sized_param>
{
protected:
    patomic_ops_t m_iops;  // implicit
    patomic_ops_explicit_t m_eops;  // explicit
    patomic_memory_order_t m_order;
    size_t m_width;
    size_t m_align;
    bool m_is_explicit;
    std::vector<patomic::test::aligned_buffer> m_buffers;
    // non-owning pointers to aligned buffers
    unsigned char *m_obj;  // "atomic" object
    unsigned char *m_old;  // old value to compare against
    unsigned char *m_ret;  // potential return value
    unsigned char *m_res;  // independent result
    std::vector<unsigned char *> m_arg1s;
    std::vector<unsigned char *> m_arg2s;
    std::vector<unsigned char *> m_arg3s;
    static constexpr int m_argc = 50;

    void SetUpBuffers(size_t width, size_t align, unsigned int seed)
    {
        // setup arg buffers
        auto bufc = m_argc * 3;  // number of arg vectors
        for (int i = 0; i < bufc; ++i) { m_buffers.emplace_back(width, align); }
        // randomise buffer state
        std::mt19937 gen(seed);
        std::uniform_int_distribution<unsigned char> dist;
        for (auto& buf : m_buffers)
        {
            auto begin = buf.data;
            auto end = begin + buf.size;
            while (begin != end) { *begin++ = dist(gen); }
        }
        // setup non-arg buffers and pointers
        for (int i = 0; i < 4; ++i) { m_buffers.emplace_back(width, align); }
        auto it = m_buffers.end();
        m_obj = (--it)->data;
        m_old = (--it)->data;
        m_ret = (--it)->data;
        m_res = (--it)->data;
        // setup arg pointers
        it = m_buffers.begin();
        for (int i = 0; i < m_argc; ++i)
        {
            m_arg1s.push_back((it++)->data);
            m_arg2s.push_back((it++)->data);
            m_arg3s.push_back((it++)->data);
        }
    }

    void SetUpImplicit()
    {
        // get patomic
        auto &p = GetParam();
        auto opt = patomic_options_IGNORE_NON_ARG_IDS;
        auto pit = patomic_create(p.width, p.order, opt, 1, p.id);
        // setup members
        m_iops = pit.ops;
        m_order = p.order;
        m_width = p.width;
        m_align = pit.align.recommended;
    }

    void SetUpExplicit()
    {
        // get patomic
        auto &p = GetParam();
        auto opt = patomic_options_IGNORE_NON_ARG_IDS;
        auto pet = patomic_create_explicit(p.width, opt, 1, p.id);
        // setup members
        m_eops = pet.ops;
        m_order = p.order;
        m_width = p.width;
        m_align = pet.align.recommended;
    }

    void SetUp() override
    {
        // setup conditionally
        auto &p = GetParam();
        m_is_explicit = p.is_explicit;
        if (p.is_explicit) { SetUpExplicit(); }
        else { SetUpImplicit(); }
        SetUpBuffers(p.width, m_align, p.seed);
        // record properties
        RecordProperty("IsExplicit", p.is_explicit);
        RecordProperty("Width", std::to_string(p.width));
        RecordProperty("Order", std::to_string(p.order));
        RecordProperty("OrderName", patomic::test::get_order_name(p.order));
        RecordProperty("ImplId", std::to_string(p.id));
        RecordProperty("ImplName", patomic::test::get_id_name(p.id));
        RecordProperty("Seed", std::to_string(p.seed));
    }
};


TEST_P(BufferOpsLogicTestFixture, fp_store)
{
    CURRY_OP_NO_RET(store, ops.fp_, obj, des);
    // skip
    if (!patomic_is_valid_store_order(m_order)) { GTEST_SKIP_("Invalid store order"); }
    else if (fp_store == nullptr) { GTEST_SKIP_("Not implemented"); }
    // test
    for (auto arg : m_arg1s)
    {
        fp_store(m_obj, arg);
        ASSERT_TRUE(std::memcmp(m_obj, arg, m_width) == 0);
    }
}

TEST_P(BufferOpsLogicTestFixture, fp_load)
{
    CURRY_OP_RET(load, ops.fp_, ret, obj);
    // skip
    if (!patomic_is_valid_load_order(m_order)) { GTEST_SKIP_("Invalid load order"); }
    else if (fp_load == nullptr) { GTEST_SKIP_("Not implemented"); }
    // test
    for (auto arg : m_arg1s)
    {
        std::memcpy(m_obj, arg, m_width);
        fp_load(m_obj, m_ret);
        ASSERT_TRUE(std::memcmp(m_obj, m_ret, m_width) == 0);
    }
}

TEST_P(BufferOpsLogicTestFixture, fp_exchange)
{
    CURRY_OP_RET(exchange, ops.xchg_ops.fp_, ret, obj, des);
    // skip
    if (fp_exchange == nullptr) { GTEST_SKIP_("Not implemented"); }
    // test
    for (int i = 0; i < m_argc; ++i)
    {
        auto current = m_arg1s[i];
        auto desired = m_arg2s[i];
        std::memcpy(m_obj, current, m_width);
        fp_exchange(m_obj, desired, m_ret);
        ASSERT_TRUE(std::memcmp(m_obj, desired, m_width) == 0);
        ASSERT_TRUE(std::memcmp(m_ret, current, m_width) == 0);
    }
}

TEST_P(BufferOpsLogicTestFixture, fp_cmpxchg_weak)
{
    CURRY_OP_CMPXCHG(cmpxchg_weak, ops.xchg_ops.fp_, obj, exp, des);
    // skip
    if (fp_cmpxchg_weak == nullptr) { GTEST_SKIP_("Not implemented"); }
    // test
    for (int i = 0; i < m_argc; ++i)
    {
        std::memcpy(m_obj , m_arg1s[i], m_width);
        // make sure on failure that obj NOT modified
        std::memcpy(m_old, m_obj, m_width);
        auto expected = m_arg2s[i];
        auto desired = m_arg3s[i];
        // obj == expected
        if (i % 2 == 0 || std::memcmp(m_obj, expected, m_width) == 0)
        {
            // set expected <- obj, and save expected value
            std::memcpy(expected, m_obj, m_width);
            std::memcpy(m_res, expected, m_width);
            bool ok = fp_cmpxchg_weak(m_obj, expected, desired);
            if (ok)
            {
                ASSERT_TRUE(std::memcmp(m_obj, desired, m_width) == 0);
                ASSERT_TRUE(std::memcmp(m_res, expected, m_width) == 0);
            }
            // spurious failure
            else
            {
                ASSERT_TRUE(std::memcmp(m_obj, expected, m_width) == 0);
                ASSERT_TRUE(std::memcmp(m_obj, m_old, m_width) == 0);
            }
        }
        // obj != expected
        else
        {
            bool ok = fp_cmpxchg_weak(m_obj, expected, desired);
            ASSERT_FALSE(ok);
            ASSERT_TRUE(std::memcmp(m_obj, expected, m_width) == 0);
            ASSERT_TRUE(std::memcmp(m_obj, m_old, m_width) == 0);
        }
    }
}

TEST_P(BufferOpsLogicTestFixture, fp_cmpxchg_strong)
{
    CURRY_OP_CMPXCHG(cmpxchg_strong, ops.xchg_ops.fp_, obj, exp, des);
    // skip
    if (fp_cmpxchg_strong == nullptr) { GTEST_SKIP_("Not implemented"); }
    // test
    for (int i = 0; i < m_argc; ++i)
    {
        std::memcpy(m_obj, m_arg1s[i], m_width);
        // make sure on failure that obj NOT modified
        std::memcpy(m_old, m_obj, m_width);
        auto expected = m_arg2s[i];
        auto desired = m_arg3s[i];
        // obj == expected
        if (i % 2 == 0 || std::memcmp(m_obj, expected, m_width) == 0)
        {
            // set expected <- obj, and save expected value
            std::memcpy(expected, m_obj, m_width);
            std::memcpy(m_res, expected, m_width);
            bool ok = fp_cmpxchg_strong(m_obj, expected, desired);
            ASSERT_TRUE(ok);
            ASSERT_TRUE(std::memcmp(m_obj, desired, m_width) == 0);
            ASSERT_TRUE(std::memcmp(m_res, expected, m_width) == 0);
        }
        // obj != expected
        else
        {
            bool ok = fp_cmpxchg_strong(m_obj, expected, desired);
            ASSERT_FALSE(ok);
            ASSERT_TRUE(std::memcmp(m_obj, expected, m_width) == 0);
            ASSERT_TRUE(std::memcmp(m_obj, m_old, m_width) == 0);
        }
    }
}


static auto get_test_params() -> const std::vector<patomic::test::sized_param>&
{
    static bool once_flag;
    static std::vector<patomic::test::sized_param> params;
    if (!once_flag)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<unsigned int> dist;
        // generate implicit params
        for (auto id : patomic::test::get_ids()) {
            // don't include NULL
            if (id == patomic_impl_id_NULL) { continue; }
            for (auto width : patomic::test::get_widths()) {
                for (auto order : patomic::test::get_orders()) {
                    // .is_explicit=false
                    params.push_back({width, order, id, dist(gen), false});
                }
            }
        }
        // copy construct explicit params
        auto pe = params;
        for (auto& p : pe) { p.is_explicit = true; }
        params.insert(params.end(), pe.begin(), pe.end());
        // set flag
        once_flag = true;
    }
    return params;
}

INSTANTIATE_TEST_SUITE_P(
        BufferOpsLogicTest,
        BufferOpsLogicTestFixture,
        ::testing::ValuesIn(get_test_params().begin(), get_test_params().end())
);
