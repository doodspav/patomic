#include <csignal>
#include <random>
#include <vector>

#include <patomic_test/patomic_test.hpp>
#include <patomic_test/aligned_buffer.hpp>
#include <patomic_test/curry_op.hpp>

#include <patomic/patomic.h>
#include <gtest/gtest.h>


#ifdef _MSC_VER
    #define KilledBySignal(sig) ExitedWithCode(3)
#endif


class ValidOrderOpsDeathTestFixture
    : public testing::TestWithParam<patomic::test::sized_param>
{
protected:
    patomic_ops_explicit_t m_ops;
    patomic_memory_order_t m_order;
    std::vector<int> m_orders;
    static constexpr int m_argc = 50;

    void SetUp() override
    {
        // setup ops
        auto &p = GetParam();
        auto opt = patomic_options_IGNORE_NON_ARG_IDS;
        auto pet = patomic_create_explicit(p.width, opt, 1, p.id);
        m_ops = pet.ops;
        m_order = p.order;
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

TEST_P(ValidOrderBufferOpsDeathTestFixture, fp_exchange)
{
    auto fp_exchange = m_ops.xchg_ops.fp_exchange;
    if (fp_exchange == nullptr) { GTEST_SKIP_("Not implemented"); }
    for (auto order : m_orders)
    {
        ASSERT_EXIT(
            fp_exchange(nullptr, nullptr, order, nullptr),
            ::testing::KilledBySignal(SIGABRT),
            ".*"
        );
    }
}

TEST_P(ValidOrderBufferOpsDeathTestFixture, fp_cmpxchg_weak)
{
    auto fp_cmpxchg_weak = m_ops.xchg_ops.fp_cmpxchg_weak;
    if (fp_cmpxchg_weak == nullptr) { GTEST_SKIP_("Not implemented"); }
    // invalid succ and valid fail
    for (auto order : m_orders)
    {
        auto fail = patomic_RELAXED;
        ASSERT_EXIT(
            fp_cmpxchg_weak(nullptr, nullptr, nullptr, order, fail),
            ::testing::KilledBySignal(SIGABRT),
            ".*"
        );
    }
    // valid succ and invalid fail
    for (auto order : m_orders)
    {
        auto succ = patomic_SEQ_CST;
        ASSERT_EXIT(
            fp_cmpxchg_weak(nullptr, nullptr, nullptr, succ, order),
            ::testing::KilledBySignal(SIGABRT),
            ".*"
        );
    }
    // fail is stronger than succ
    ASSERT_EXIT(
        fp_cmpxchg_weak(nullptr, nullptr, nullptr, patomic_RELAXED, patomic_SEQ_CST),
        ::testing::KilledBySignal(SIGABRT),
        ".*"
    );
    // no test here for fail being valid order but invalid *load* order
    // tested in ValidLoadOrderOpsDeathTestFixture
}

TEST_P(ValidOrderBufferOpsDeathTestFixture, fp_cmpxchg_strong)
{
    auto fp_cmpxchg_strong = m_ops.xchg_ops.fp_cmpxchg_strong;
    if (fp_cmpxchg_strong == nullptr) { GTEST_SKIP_("Not implemented"); }
    // invalid succ and valid fail
    for (auto order : m_orders)
    {
        auto fail = patomic_RELAXED;
        ASSERT_EXIT(
            fp_cmpxchg_strong(nullptr, nullptr, nullptr, order, fail),
            ::testing::KilledBySignal(SIGABRT),
            ".*"
        );
    }
    // valid succ and invalid fail
    for (auto order : m_orders)
    {
        auto succ = patomic_SEQ_CST;
        ASSERT_EXIT(
            fp_cmpxchg_strong(nullptr, nullptr, nullptr, succ, order),
            ::testing::KilledBySignal(SIGABRT),
            ".*"
        );
    }
    // fail is stronger than succ
    ASSERT_EXIT(
        fp_cmpxchg_strong(nullptr, nullptr, nullptr, patomic_RELAXED, patomic_SEQ_CST),
        ::testing::KilledBySignal(SIGABRT),
        ".*"
    );
    // no test here for fail being valid order but invalid *load* order
    // tested in ValidLoadOrderOpsDeathTestFixture
}

TEST_P(ValidOrderBufferOpsDeathTestFixture, fp_test)
{
    auto fp_test = m_ops.bitwise_ops.fp_test;
    if (fp_test == nullptr) { GTEST_SKIP_("Not implemented"); }
    for (auto order : m_orders)
    {
        ASSERT_EXIT(
            fp_test(nullptr, 0, order),
            testing::KilledBySignal(SIGABRT),
            ".*"
        );
    }
}

TEST_P(ValidOrderBufferOpsDeathTestFixture, fp_test_compl)
{
    auto fp_test_compl = m_ops.bitwise_ops.fp_test_compl;
    if (fp_test_compl == nullptr) { GTEST_SKIP_("Not implemented"); }
    for (auto order : m_orders)
    {
        ASSERT_EXIT(
            fp_test_compl(nullptr, 0, order),
            testing::KilledBySignal(SIGABRT),
            ".*"
        );
    }
}

TEST_P(ValidOrderBufferOpsDeathTestFixture, fp_test_set)
{
    auto fp_test_set = m_ops.bitwise_ops.fp_test_set;
    if (fp_test_set == nullptr) { GTEST_SKIP_("Not implemented"); }
    for (auto order : m_orders)
    {
        ASSERT_EXIT(
            fp_test_set(nullptr, 0, order),
            testing::KilledBySignal(SIGABRT),
            ".*"
        );
    }
}

TEST_P(ValidOrderBufferOpsDeathTestFixture, fp_test_reset)
{
    auto fp_test_reset = m_ops.bitwise_ops.fp_test_reset;
    if (fp_test_reset == nullptr) { GTEST_SKIP_("Not implemented"); }
    for (auto order : m_orders)
    {
        ASSERT_EXIT(
            fp_test_reset(nullptr, 0, order),
            testing::KilledBySignal(SIGABRT),
            ".*"
        );
    }
}

TEST_P(ValidOrderBufferOpsDeathTestFixture, fp_or)
{
    auto fp_or = m_ops.binary_ops.fp_or;
    if (fp_or == nullptr) { GTEST_SKIP_("Not implemented"); }
    for (auto order : m_orders)
    {
        ASSERT_EXIT(
            fp_or(nullptr, nullptr, order),
            testing::KilledBySignal(SIGABRT),
            ".*"
        );
    }
}

TEST_P(ValidOrderBufferOpsDeathTestFixture, fp_xor)
{
    auto fp_xor = m_ops.binary_ops.fp_xor;
    if (fp_xor == nullptr) { GTEST_SKIP_("Not implemented"); }
    for (auto order : m_orders)
    {
        ASSERT_EXIT(
            fp_xor(nullptr, nullptr, order),
            testing::KilledBySignal(SIGABRT),
            ".*"
        );
    }
}

TEST_P(ValidOrderBufferOpsDeathTestFixture, fp_and)
{
    auto fp_and = m_ops.binary_ops.fp_and;
    if (fp_and == nullptr) { GTEST_SKIP_("Not implemented"); }
    for (auto order : m_orders)
    {
        ASSERT_EXIT(
            fp_and(nullptr, nullptr, order),
            testing::KilledBySignal(SIGABRT),
            ".*"
        );
    }
}

TEST_P(ValidOrderBufferOpsDeathTestFixture, fp_not)
{
    auto fp_not = m_ops.binary_ops.fp_not;
    if (fp_not == nullptr) { GTEST_SKIP_("Not implemented"); }
    for (auto order : m_orders)
    {
        ASSERT_EXIT(
            fp_not(nullptr, order),
            testing::KilledBySignal(SIGABRT),
            ".*"
        );
    }
}

TEST_P(ValidOrderBufferOpsDeathTestFixture, fp_fetch_or)
{
    auto fp_fetch_or = m_ops.binary_ops.fp_fetch_or;
    if (fp_fetch_or == nullptr) { GTEST_SKIP_("Not implemented"); }
    for (auto order : m_orders)
    {
        ASSERT_EXIT(
            fp_fetch_or(nullptr, nullptr, order, nullptr),
            testing::KilledBySignal(SIGABRT),
            ".*"
        );
    }
}

TEST_P(ValidOrderBufferOpsDeathTestFixture, fp_fetch_xor)
{
    auto fp_fetch_xor = m_ops.binary_ops.fp_fetch_xor;
    if (fp_fetch_xor == nullptr) { GTEST_SKIP_("Not implemented"); }
    for (auto order : m_orders)
    {
        ASSERT_EXIT(
            fp_fetch_xor(nullptr, nullptr, order, nullptr),
            testing::KilledBySignal(SIGABRT),
            ".*"
        );
    }
}

TEST_P(ValidOrderBufferOpsDeathTestFixture, fp_fetch_and)
{
    auto fp_fetch_and = m_ops.binary_ops.fp_fetch_and;
    if (fp_fetch_and == nullptr) { GTEST_SKIP_("Not implemented"); }
    for (auto order : m_orders)
    {
        ASSERT_EXIT(
            fp_fetch_and(nullptr, nullptr, order, nullptr),
            testing::KilledBySignal(SIGABRT),
            ".*"
        );
    }
}

TEST_P(ValidOrderBufferOpsDeathTestFixture, fp_fetch_not)
{
    auto fp_fetch_not = m_ops.binary_ops.fp_fetch_not;
    if (fp_fetch_not == nullptr) { GTEST_SKIP_("Not implemented"); }
    for (auto order : m_orders)
    {
        ASSERT_EXIT(
            fp_fetch_not(nullptr, order, nullptr),
            testing::KilledBySignal(SIGABRT),
            ".*"
        );
    }
}


class ValidOrderArithmeticOpsDeathTestFixture
    : public ValidOrderOpsDeathTestFixture
{
protected:
    bool m_is_signed;

    void SetUp() override
    {
        using Base = ValidOrderOpsDeathTestFixture;
        Base::SetUp();
        auto &p = GetParam();
        m_is_signed = p.is_signed;
        RecordProperty("IsSigned", p.is_signed);
    }
};


#define GET_ARI_OP(name)               \
    this->m_is_signed ?                \
    this->m_ops.signed_ops.fp_##name : \
    this->m_ops.unsigned_ops.fp_##name


TEST_P(ValidOrderArithmeticOpsDeathTestFixture, fp_add)
{
    auto fp_add = GET_ARI_OP(add);
    if (fp_add == nullptr) { GTEST_SKIP_("Not implemented"); }
    for (auto order : m_orders)
    {
        ASSERT_EXIT(
            fp_add(nullptr, nullptr, order),
            testing::KilledBySignal(SIGABRT),
            ".*"
        );
    }
}

TEST_P(ValidOrderArithmeticOpsDeathTestFixture, fp_sub)
{
    auto fp_sub = GET_ARI_OP(sub);
    if (fp_sub == nullptr) { GTEST_SKIP_("Not implemented"); }
    for (auto order : m_orders)
    {
        ASSERT_EXIT(
            fp_sub(nullptr, nullptr, order),
            testing::KilledBySignal(SIGABRT),
            ".*"
        );
    }
}

TEST_P(ValidOrderArithmeticOpsDeathTestFixture, fp_inc)
{
    auto fp_inc = GET_ARI_OP(inc);
    if (fp_inc == nullptr) { GTEST_SKIP_("Not implemented"); }
    for (auto order : m_orders)
    {
        ASSERT_EXIT(
            fp_inc(nullptr, order),
            testing::KilledBySignal(SIGABRT),
            ".*"
        );
    }
}

TEST_P(ValidOrderArithmeticOpsDeathTestFixture, fp_dec)
{
    auto fp_dec = GET_ARI_OP(dec);
    if (fp_dec == nullptr) { GTEST_SKIP_("Not implemented"); }
    for (auto order : m_orders)
    {
        ASSERT_EXIT(
            fp_dec(nullptr, order),
            testing::KilledBySignal(SIGABRT),
            ".*"
        );
    }
}

TEST_P(ValidOrderArithmeticOpsDeathTestFixture, fp_neg)
{
    auto fp_neg = GET_ARI_OP(neg);
    if (fp_neg == nullptr) { GTEST_SKIP_("Not implemented"); }
    for (auto order : m_orders)
    {
        ASSERT_EXIT(
            fp_neg(nullptr, order),
            testing::KilledBySignal(SIGABRT),
            ".*"
        );
    }
}

TEST_P(ValidOrderArithmeticOpsDeathTestFixture, fp_fetch_add)
{
    auto fp_fetch_add = GET_ARI_OP(fetch_add);
    if (fp_fetch_add == nullptr) { GTEST_SKIP_("Not implemented"); }
    for (auto order : m_orders)
    {
        ASSERT_EXIT(
            fp_fetch_add(nullptr, nullptr, order, nullptr),
            testing::KilledBySignal(SIGABRT),
            ".*"
        );
    }
}

TEST_P(ValidOrderArithmeticOpsDeathTestFixture, fp_fetch_sub)
{
    auto fp_fetch_sub = GET_ARI_OP(fetch_sub);
    if (fp_fetch_sub == nullptr) { GTEST_SKIP_("Not implemented"); }
    for (auto order : m_orders)
    {
        ASSERT_EXIT(
            fp_fetch_sub(nullptr, nullptr, order, nullptr),
            testing::KilledBySignal(SIGABRT),
            ".*"
        );
    }
}

TEST_P(ValidOrderArithmeticOpsDeathTestFixture, fp_fetch_inc)
{
    auto fp_fetch_inc = GET_ARI_OP(fetch_inc);
    if (fp_fetch_inc == nullptr) { GTEST_SKIP_("Not implemented"); }
    for (auto order : m_orders)
    {
        ASSERT_EXIT(
            fp_fetch_inc(nullptr, order, nullptr),
            testing::KilledBySignal(SIGABRT),
            ".*"
        );
    }
}

TEST_P(ValidOrderArithmeticOpsDeathTestFixture, fp_fetch_dec)
{
    auto fp_fetch_dec = GET_ARI_OP(fetch_dec);
    if (fp_fetch_dec == nullptr) { GTEST_SKIP_("Not implemented"); }
    for (auto order : m_orders)
    {
        ASSERT_EXIT(
            fp_fetch_dec(nullptr, order, nullptr),
            testing::KilledBySignal(SIGABRT),
            ".*"
        );
    }
}

TEST_P(ValidOrderArithmeticOpsDeathTestFixture, fp_fetch_neg)
{
    auto fp_fetch_neg = GET_ARI_OP(fetch_neg);
    if (fp_fetch_neg == nullptr) { GTEST_SKIP_("Not implemented"); }
    for (auto order : m_orders)
    {
        ASSERT_EXIT(
            fp_fetch_neg(nullptr, order, nullptr),
            testing::KilledBySignal(SIGABRT),
            ".*"
        );
    }
}


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

TEST_P(ValidLoadOrderOpsDeathTestFixture, fp_cmpxchg_weak)
{
    auto fp_cmpxchg_weak = m_ops.xchg_ops.fp_cmpxchg_weak;
    if (fp_cmpxchg_weak == nullptr) { GTEST_SKIP_("Not implemented"); }
    for (auto order : m_orders)
    {
        auto succ = patomic_SEQ_CST;
        ASSERT_EXIT(
            fp_cmpxchg_weak(nullptr, nullptr, nullptr, succ, order),
            ::testing::KilledBySignal(SIGABRT),
            ".*"
        );
    }
}

TEST_P(ValidLoadOrderOpsDeathTestFixture, fp_cmpxchg_strong)
{
    auto fp_cmpxchg_strong = m_ops.xchg_ops.fp_cmpxchg_strong;
    if (fp_cmpxchg_strong == nullptr) { GTEST_SKIP_("Not implemented"); }
    for (auto order : m_orders)
    {
        auto succ = patomic_SEQ_CST;
        ASSERT_EXIT(
            fp_cmpxchg_strong(nullptr, nullptr, nullptr, succ, order),
            ::testing::KilledBySignal(SIGABRT),
            ".*"
        );
    }
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

static std::vector<patomic::test::sized_param>&
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

INSTANTIATE_TEST_SUITE_P(
    ValidOrderArithmeticOpsDeathTest,
    ValidOrderArithmeticOpsDeathTestFixture,
    ::testing::ValuesIn(get_arithmetic_test_params().begin(),
                        get_arithmetic_test_params().end())
);

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
