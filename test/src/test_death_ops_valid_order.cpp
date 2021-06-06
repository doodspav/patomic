#include <csignal>
#include <random>
#include <vector>

#include <patomic_test/patomic_test.hpp>
#include <patomic_test/aligned_buffer.hpp>
#include <patomic_test/curry_op.hpp>

#include <patomic/patomic.h>
#include <gtest/gtest.h>


class ValidOrderOpsDeathTestFixture
    : public testing::TestWithParam<patomic::test::sized_param>
{
protected:
    patomic_ops_explicit_t m_ops;
    patomic_memory_order_t m_order;
    bool m_is_signed;
    std::vector<int> m_orders;
    static constexpr int m_argc = 5;  // death tests are really expensive

    void SetUp() override
    {
        // setup ops
        auto &p = GetParam();
        auto opt = patomic_options_IGNORE_NON_ARG_IDS;
        auto pet = patomic_create_explicit(p.width, opt, 1, p.id);
        m_ops = pet.ops;
        m_order = p.order;
        m_is_signed = p.is_signed;
        // setup order inputs
        auto umin = static_cast<unsigned int>(patomic_SEQ_CST) + 1u;
        ASSERT_FALSE(patomic_is_valid_order(umin));
        std::mt19937 gen(p.seed);
        std::uniform_int_distribution<unsigned int> dist(umin);
        m_orders.resize(m_argc);
        for (auto& o : m_orders) { o = static_cast<int>(dist(gen)); }
        for (auto& o : m_orders) { ASSERT_FALSE(patomic_is_valid_order(o)); }
        // record properties
        RecordProperty("Width", std::to_string(p.width));
        RecordProperty("ImplId", std::to_string(p.id));
        RecordProperty("ImplName", patomic::test::get_id_name(p.id));
        RecordProperty("Seed", std::to_string(p.seed));
    }
};


class ValidOrderBufferOpsDeathTestFixture
    : public ValidOrderOpsDeathTestFixture
{};


TEST_P(ValidOrderBufferOpsDeathTestFixture, fp_store)
{
    auto fp_store = m_ops.fp_store;
    if (fp_store == nullptr) { GTEST_SKIP_("Not implemented"); }
    for (auto order : m_orders)
    {
        ASSERT_EXIT(
            fp_store(nullptr, nullptr, order),
            ::testing::KilledBySignal(SIGABRT),
            ".*"
        );
    }
}

TEST_P(ValidOrderBufferOpsDeathTestFixture, fp_load)
{
    auto fp_load = m_ops.fp_load;
    if (fp_load == nullptr) { GTEST_SKIP_("Not implemented"); }
    for (auto order : m_orders)
    {
        ASSERT_EXIT(
            fp_load(nullptr, order, nullptr),
            ::testing::KilledBySignal(SIGABRT),
            ".*"
        );
    }
}


/*class ValidOrderArithmeticOpsDeathTestFixture
    : public ValidOrderOpsDeathTestFixture
{};*/


class ValidLoadOrderOpsDeathTestFixture
    : public ValidOrderOpsDeathTestFixture
{};


TEST_P(ValidLoadOrderOpsDeathTestFixture, fp_load)
{
    auto fp_load = m_ops.fp_load;
    if (fp_load == nullptr) { GTEST_SKIP_("Not implemented"); }
    ASSERT_EXIT(
        fp_load(nullptr, m_order, nullptr),
        ::testing::KilledBySignal(SIGABRT),
        ".*"
    );
}


class ValidStoreOrderOpsDeathTestFixture
    : public ValidOrderOpsDeathTestFixture
{};


TEST_P(ValidStoreOrderOpsDeathTestFixture, fp_store)
{
    auto fp_store = m_ops.fp_store;
    if (fp_store == nullptr) { GTEST_SKIP_("Not implemented"); }
    ASSERT_EXIT(
        fp_store(nullptr, nullptr, m_order),
        ::testing::KilledBySignal(SIGABRT),
        ".*"
    );
}


static std::vector<patomic::test::sized_param>&
get_buffer_test_params()
{
    static bool once_flag;
    static std::vector<patomic::test::sized_param> params;
    if (!once_flag)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<unsigned int> dist;
        auto order = patomic_SEQ_CST;
        // generate params (we only care about id, width, and seed)
        for (auto id : patomic::test::get_ids()) {
            // don't include NULL
            if (id == patomic_impl_id_NULL) { continue; }
            for (auto width : patomic::test::get_widths()) {
                params.push_back({width, order, id, dist(gen), false, false});
            }
        }
        // set flag
        once_flag = true;
    }
    return params;
}

/*static std::vector<patomic::test::sized_param>&
get_arithmetic_test_params()
{
    static bool once_flag;
    static std::vector<patomic::test::sized_param> params;
    if (!once_flag)
    {
        // create signed and unsigned duplicates
        auto ps = get_buffer_test_params();
        params.reserve(ps.size() * 2);
        for (auto &p : ps) { p.is_signed = false; }
        params.insert(params.end(), ps.begin(), ps.end());
        for (auto &p : ps) { p.is_signed = true; }
        params.insert(params.end(), ps.begin(), ps.end());
        // set flag
        once_flag = true;
    }
    return params;
}
*/

static std::vector<patomic::test::sized_param>&
get_load_test_params()
{
    static bool once_flag;
    static std::vector<patomic::test::sized_param> params;
    if (!once_flag)
    {
        std::vector<patomic_memory_order_t> invalid_load_orders {
            // RELAXED
            // CONSUME
            // ACQUIRE
            patomic_RELEASE,
            patomic_ACQ_REL
            // SEQ_CST
        };
        // create invalid load order duplicates
        auto ps = get_buffer_test_params();
        params.reserve(ps.size() * invalid_load_orders.size());
        for (auto order : invalid_load_orders) {
            for (auto &p : ps) { p.order = order; }
            params.insert(params.end(), ps.begin(), ps.end());
        }
        // set flag
        once_flag = true;
    }
    return params;
}

static std::vector<patomic::test::sized_param>&
get_store_test_params()
{
    static bool once_flag;
    static std::vector<patomic::test::sized_param> params;
    if (!once_flag)
    {
        std::vector<patomic_memory_order_t> invalid_store_orders {
            // RELAXED
            patomic_CONSUME,
            patomic_ACQUIRE,
            // RELEASE
            patomic_ACQ_REL,
            // SEQ_CST
        };
        // create invalid store order duplicates
        auto ps = get_buffer_test_params();
        params.reserve(ps.size() * invalid_store_orders.size());
        for (auto order : invalid_store_orders) {
            for (auto &p : ps) { p.order = order; }
            params.insert(params.end(), ps.begin(), ps.end());
        }
        // set flag
        once_flag = true;
    }
    return params;
}


INSTANTIATE_TEST_SUITE_P(
    ValidOrderBufferOpsDeathTest,
    ValidOrderBufferOpsDeathTestFixture,
    ::testing::ValuesIn(get_buffer_test_params().begin(),
                        get_buffer_test_params().end())
);

/*INSTANTIATE_TEST_SUITE_P(
    ValidOrderArithmeticOpsDeathTest,
    ValidOrderArithmeticOpsDeathTestFixture,
    ::testing::ValuesIn(get_arithmetic_test_params().begin(),
                        get_arithmetic_test_params().end())
);*/

INSTANTIATE_TEST_SUITE_P(
    ValidLoadOrderOpsDeathTest,
    ValidLoadOrderOpsDeathTestFixture,
    ::testing::ValuesIn(get_load_test_params().begin(),
                        get_load_test_params().end())
);

INSTANTIATE_TEST_SUITE_P(
    ValidStoreOrderOpsDeathTest,
    ValidStoreOrderOpsDeathTestFixture,
    ::testing::ValuesIn(get_store_test_params().begin(),
                        get_store_test_params().end())
);
