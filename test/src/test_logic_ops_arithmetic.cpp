#include <limits>
#include <random>
#include <vector>

#include <patomic_test/patomic_test.hpp>
#include <patomic_test/aligned_buffer.hpp>
#include <patomic_test/aligned_vint.hpp>
#include <patomic_test/curry_op.hpp>

#include <patomic/patomic.h>
#include <gtest/gtest.h>


class ArithmeticOpsLogicTestFixture
        : public testing::TestWithParam<patomic::test::sized_param>
{
protected:
    patomic_ops_arithmetic_t m_iops;  // implicit
    patomic_ops_explicit_arithmetic_t m_eops;  // explicit
    patomic_memory_order_t m_order;
    size_t m_width;
    size_t m_align;
    bool m_is_explicit;
    bool m_is_signed;
    std::vector<patomic::test::aligned_buffer> m_buffers;
    // non-owning pointers to aligned buffers
    unsigned char *m_obj;  // "atomic" object
    unsigned char *m_ret;  // potential return value
    std::vector<unsigned char *> m_arg1s;
    std::vector<unsigned char *> m_arg2s;
    static constexpr int m_argc = 50;

    void SetUpBuffers(size_t width, size_t align, unsigned int seed)
    {
        // setup arg buffers
        auto bufc = m_argc * 2;  // number of arg vectors
        for (int i = 0; i < bufc; ++i) { m_buffers.emplace_back(width, align); }
        // randomise buffer state
        constexpr auto umax = std::numeric_limits<unsigned char>::max();
        std::mt19937 gen(seed);
        std::uniform_int_distribution<unsigned int> dist(0u, umax);
        for (auto& buf : m_buffers)
        {
            auto begin = buf.data;
            auto end = begin + buf.size;
            while (begin != end) { *begin++ = dist(gen); }
        }
        // setup non-arg buffers and pointers
        for (int i = 0; i < 2; ++i) { m_buffers.emplace_back(width, align); }
        auto it = m_buffers.end();
        m_obj = (--it)->data;
        m_ret = (--it)->data;
        // setup arg pointers
        it = m_buffers.begin();
        for (int i = 0; i < m_argc; ++i)
        {
            m_arg1s.push_back((it++)->data);
            m_arg2s.push_back((it++)->data);
        }
    }

    void InsertMinMaxArgs()
    {
        auto min = patomic::test::create_vint(m_width, m_align, m_is_signed);
        auto max = patomic::test::create_vint(m_width, m_align, m_is_signed);
        min->min();
        max->max();
        // arg1s
        if (m_arg1s.size() >= 2)
        {
            auto it = m_arg1s.end();
            std::memcpy(*(--it), min->data(), m_width);
            std::memcpy(*(--it), max->data(), m_width);
        }
        // arg2s
        if (m_arg2s.size() >= 2)
        {
            auto it = m_arg2s.begin();
            std::memcpy(*it++, min->data(), m_width);
            std::memcpy(*it++, max->data(), m_width);
        }
    }

    void SetUpImplicit()
    {
        // get patomic
        auto &p = GetParam();
        auto opt = patomic_options_IGNORE_NON_ARG_IDS;
        auto pit = patomic_create(p.width, p.order, opt, 1, p.id);
        // setup members
        m_iops = p.is_signed ? pit.ops.signed_ops : pit.ops.unsigned_ops;
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
        m_eops = p.is_signed ? pet.ops.signed_ops : pet.ops.unsigned_ops;
        m_order = p.order;
        m_width = p.width;
        m_align = pet.align.recommended;
    }

    void SetUp() override
    {
        // setup conditionally
        auto &p = GetParam();
        m_is_explicit = p.is_explicit;
        m_is_signed = p.is_signed;
        if (p.is_explicit) { SetUpExplicit(); }
        else { SetUpImplicit(); }
        SetUpBuffers(p.width, m_align, p.seed);
        InsertMinMaxArgs();
        // record properties
        RecordProperty("IsExplicit", p.is_explicit);
        RecordProperty("IsSigned", p.is_signed);
        RecordProperty("Width", std::to_string(p.width));
        RecordProperty("Order", std::to_string(p.order));
        RecordProperty("OrderName", patomic::test::get_order_name(p.order));
        RecordProperty("ImplId", std::to_string(p.id));
        RecordProperty("ImplName", patomic::test::get_id_name(p.id));
        RecordProperty("Seed", std::to_string(p.seed));
    }
};


#define CURRY_OP_NO_RET(name)         \
    ::patomic::test::curry_op_no_ret( \
        this->m_iops.fp_##name,       \
        this->m_eops.fp_##name,       \
        this->m_is_explicit,          \
        this->m_order                 \
    )

#define CURRY_OP_RET(name)         \
    ::patomic::test::curry_op_ret( \
        this->m_iops.fp_##name,    \
        this->m_eops.fp_##name,    \
        this->m_is_explicit,       \
        this->m_order              \
    )


TEST_P(ArithmeticOpsLogicTestFixture, fp_add)
{
    auto fp_add = CURRY_OP_NO_RET(add);
    // skip
    if (fp_add == nullptr) { GTEST_SKIP_("Not implemented"); }
    // create vints
    auto vobj = patomic::test::create_vint(m_width, m_align, m_is_signed);
    auto varg = patomic::test::create_vint(m_width, m_align, m_is_signed);
    if (!(vobj && varg)) { GTEST_FATAL_FAILURE_("Could not create vint"); }
    // test
    for (int i = 0; i < m_argc; ++i)
    {
        std::memcpy(m_obj, m_arg1s[i], m_width);
        vobj->store(m_arg1s[i], m_width);
        varg->store(m_arg2s[i], m_width);
        vobj->add(*varg);
        fp_add(m_obj, m_arg2s[i]);
        ASSERT_TRUE(std::memcmp(m_obj, vobj->data(), m_width) == 0);
    }
}

TEST_P(ArithmeticOpsLogicTestFixture, fp_sub)
{
    auto fp_sub = CURRY_OP_NO_RET(sub);
    // skip
    if (fp_sub == nullptr) { GTEST_SKIP_("Not implemented"); }
    // create vints
    auto vobj = patomic::test::create_vint(m_width, m_align, m_is_signed);
    auto varg = patomic::test::create_vint(m_width, m_align, m_is_signed);
    if (!(vobj && varg)) { GTEST_FATAL_FAILURE_("Could not create vint"); }
    // test
    for (int i = 0; i < m_argc; ++i)
    {
        std::memcpy(m_obj, m_arg1s[i], m_width);
        vobj->store(m_arg1s[i], m_width);
        varg->store(m_arg2s[i], m_width);
        vobj->sub(*varg);
        fp_sub(m_obj, m_arg2s[i]);
        ASSERT_TRUE(std::memcmp(m_obj, vobj->data(), m_width) == 0);
    }
}

TEST_P(ArithmeticOpsLogicTestFixture, fp_inc)
{
    auto fp_inc = CURRY_OP_NO_RET(inc);
    // skip
    if (fp_inc == nullptr) { GTEST_SKIP_("Not implemented"); }
    // create vint
    auto vobj = patomic::test::create_vint(m_width, m_align, m_is_signed);
    if (!vobj) { GTEST_FATAL_FAILURE_("Could not create vint"); }
    // test
    for (auto arg : m_arg1s)
    {
        std::memcpy(m_obj, arg, m_width);
        vobj->store(arg, m_width);
        vobj->inc();
        fp_inc(m_obj);
        ASSERT_TRUE(std::memcmp(m_obj, vobj->data(), m_width) == 0);
    }
}

TEST_P(ArithmeticOpsLogicTestFixture, fp_dec)
{
    auto fp_dec = CURRY_OP_NO_RET(dec);
    // skip
    if (fp_dec == nullptr) { GTEST_SKIP_("Not implemented"); }
    // create vint
    auto vobj = patomic::test::create_vint(m_width, m_align, m_is_signed);
    if (!vobj) { GTEST_FATAL_FAILURE_("Could not create vint"); }
    // test
    for (auto arg : m_arg1s)
    {
        std::memcpy(m_obj, arg, m_width);
        vobj->store(arg, m_width);
        vobj->dec();
        fp_dec(m_obj);
        ASSERT_TRUE(std::memcmp(m_obj, vobj->data(), m_width) == 0);
    }
}

TEST_P(ArithmeticOpsLogicTestFixture, fp_neg)
{
    auto fp_neg = CURRY_OP_NO_RET(neg);
    // skip
    if (fp_neg == nullptr) { GTEST_SKIP_("Not implemented"); }
    // create vint
    auto vobj = patomic::test::create_vint(m_width, m_align, m_is_signed);
    if (!vobj) { GTEST_FATAL_FAILURE_("Could not create vint"); }
    // test
    for (auto arg : m_arg1s)
    {
        std::memcpy(m_obj, arg, m_width);
        vobj->store(arg, m_width);
        vobj->neg();
        fp_neg(m_obj);
        ASSERT_TRUE(std::memcmp(m_obj, vobj->data(), m_width) == 0);
    }
}

TEST_P(ArithmeticOpsLogicTestFixture, fp_fetch_add)
{
    auto fp_fetch_add = CURRY_OP_RET(fetch_add);
    // skip
    if (fp_fetch_add == nullptr) { GTEST_SKIP_("Not implemented"); }
    // create vints
    auto vobj = patomic::test::create_vint(m_width, m_align, m_is_signed);
    auto varg = patomic::test::create_vint(m_width, m_align, m_is_signed);
    if (!(vobj && varg)) { GTEST_FATAL_FAILURE_("Could not create vint"); }
    // test
    for (int i = 0; i < m_argc; ++i)
    {
        std::memcpy(m_obj, m_arg1s[i], m_width);
        vobj->store(m_arg1s[i], m_width);
        varg->store(m_arg2s[i], m_width);
        vobj->add(*varg);
        fp_fetch_add(m_obj, m_arg2s[i], m_ret);
        ASSERT_TRUE(std::memcmp(m_obj, vobj->data(), m_width) == 0);
        ASSERT_TRUE(std::memcmp(m_ret, m_arg1s[i], m_width) == 0);
    }
}

TEST_P(ArithmeticOpsLogicTestFixture, fp_fetch_sub)
{
    auto fp_fetch_sub = CURRY_OP_RET(fetch_sub);
    // skip
    if (fp_fetch_sub == nullptr) { GTEST_SKIP_("Not implemented"); }
    // create vints
    auto vobj = patomic::test::create_vint(m_width, m_align, m_is_signed);
    auto varg = patomic::test::create_vint(m_width, m_align, m_is_signed);
    if (!(vobj && varg)) { GTEST_FATAL_FAILURE_("Could not create vint"); }
    // test
    for (int i = 0; i < m_argc; ++i)
    {
        std::memcpy(m_obj, m_arg1s[i], m_width);
        vobj->store(m_arg1s[i], m_width);
        varg->store(m_arg2s[i], m_width);
        vobj->sub(*varg);
        fp_fetch_sub(m_obj, m_arg2s[i], m_ret);
        ASSERT_TRUE(std::memcmp(m_obj, vobj->data(), m_width) == 0);
        ASSERT_TRUE(std::memcmp(m_ret, m_arg1s[i], m_width) == 0);
    }
}

TEST_P(ArithmeticOpsLogicTestFixture, fp_fetch_inc)
{
    auto fp_fetch_inc = CURRY_OP_RET(fetch_inc);
    // skip
    if (fp_fetch_inc == nullptr) { GTEST_SKIP_("Not implemented"); }
    // create vint
    auto vobj = patomic::test::create_vint(m_width, m_align, m_is_signed);
    if (!vobj) { GTEST_FATAL_FAILURE_("Could not create vint"); }
    // test
    for (auto arg : m_arg1s)
    {
        std::memcpy(m_obj, arg, m_width);
        vobj->store(arg, m_width);
        vobj->inc();
        fp_fetch_inc(m_obj, m_ret);
        ASSERT_TRUE(std::memcmp(m_obj, vobj->data(), m_width) == 0);
        ASSERT_TRUE(std::memcmp(m_ret, arg, m_width) == 0);
    }
}

TEST_P(ArithmeticOpsLogicTestFixture, fp_fetch_dec)
{
    auto fp_fetch_dec = CURRY_OP_RET(fetch_dec);
    // skip
    if (fp_fetch_dec == nullptr) { GTEST_SKIP_("Not implemented"); }
    // create vint
    auto vobj = patomic::test::create_vint(m_width, m_align, m_is_signed);
    if (!vobj) { GTEST_FATAL_FAILURE_("Could not create vint"); }
    // test
    for (auto arg : m_arg1s)
    {
        std::memcpy(m_obj, arg, m_width);
        vobj->store(arg, m_width);
        vobj->dec();
        fp_fetch_dec(m_obj, m_ret);
        ASSERT_TRUE(std::memcmp(m_obj, vobj->data(), m_width) == 0);
        ASSERT_TRUE(std::memcmp(m_ret, arg, m_width) == 0);
    }
}

TEST_P(ArithmeticOpsLogicTestFixture, fp_fetch_neg)
{
    auto fp_fetch_neg = CURRY_OP_RET(fetch_neg);
    // skip
    if (fp_fetch_neg == nullptr) { GTEST_SKIP_("Not implemented"); }
    // create vint
    auto vobj = patomic::test::create_vint(m_width, m_align, m_is_signed);
    if (!vobj) { GTEST_FATAL_FAILURE_("Could not create vint"); }
    // test
    for (auto arg : m_arg1s)
    {
        std::memcpy(m_obj, arg, m_width);
        vobj->store(arg, m_width);
        vobj->neg();
        fp_fetch_neg(m_obj, m_ret);
        ASSERT_TRUE(std::memcmp(m_obj, vobj->data(), m_width) == 0);
        ASSERT_TRUE(std::memcmp(m_ret, arg, m_width) == 0);
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
        // generate implicit unsigned params
        for (auto id : patomic::test::get_ids()) {
            // don't include NULL
            if (id == patomic_impl_id_NULL) { continue; }
            for (auto width : patomic::test::get_widths()) {
                // don't include unrepresentable widths
                auto uup = patomic::test::create_vint(width, width, false);
                auto sup = patomic::test::create_vint(width, width, true);
                if (uup == nullptr || sup == nullptr) { continue; }
                for (auto order : patomic::test::get_orders()) {
                    // .is_explicit= false, .is_signed=false
                    params.push_back({width, order, id, dist(gen), false, false});
                }
            }
        }
        // copy construct explicit params
        auto pe = params;
        for (auto& p : pe) { p.is_explicit = true; }
        params.insert(params.end(), pe.begin(), pe.end());
        // copy construct signed params
        auto ps = params;
        for (auto& p : ps) { p.is_signed = true; }
        params.insert(params.end(), ps.begin(), ps.end());
        // set flag
        once_flag = true;
    }
    return params;
}

INSTANTIATE_TEST_SUITE_P(
    ArithmeticOpsLogicTest,
    ArithmeticOpsLogicTestFixture,
    ::testing::ValuesIn(get_test_params().begin(), get_test_params().end())
);