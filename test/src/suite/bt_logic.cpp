#include <test/suite/bt_logic.hpp>

void
BtLogicImplicit::SetUp()
{
    // create patomic object
    const test::ParamsImplicit& p = GetParam();
    patomic_t pat = patomic_create(
        p.width, p.order, p.options, patomic_kinds_ALL, p.id
    );

    // initialize members
    m_ops = pat.ops;
    m_align = pat.align;
}


void
BtLogicExplicit::SetUp()
{
    // create patomic object
    const test::ParamsExplicit& p = GetParam();
    patomic_explicit_t pat = patomic_create_explicit(
        p.width, p.options, patomic_kinds_ALL, p.id
    );

    // initialize members
    m_ops = pat.ops;
    m_align = pat.align;
}


void
BtLogicTransaction::SetUp()
{
    // create patomic object
    const test::ParamsTransaction& p = GetParam();
    patomic_transaction_t pat = patomic_create_transaction(
        p.options, patomic_kinds_ALL, p.id
    );

    // initialize members
    m_ops = pat.ops;
}


INSTANTIATE_TEST_SUITE_P(
    ,  // no name necessary
    BtLogicImplicit,
    testing::ConvertGenerator(
        testing::ValuesIn(test::ParamsImplicit::combinations())
    ),
    [](const testing::TestParamInfo<test::ParamsImplicit>& info) -> std::string {
        return info.param.as_test_suffix();
    }
);


INSTANTIATE_TEST_SUITE_P(
    ,  // no name necessary
    BtLogicExplicit,
    testing::ConvertGenerator(
        testing::ValuesIn(test::ParamsExplicit::combinations())
    ),
    [](const testing::TestParamInfo<test::ParamsExplicit>& info) -> std::string {
        return info.param.as_test_suffix();
    }
);


INSTANTIATE_TEST_SUITE_P(
    ,  // no name necessary
    BtLogicTransaction,
    testing::ConvertGenerator(
        testing::ValuesIn(test::ParamsTransaction::combinations())
    ),
    [](const testing::TestParamInfo<test::ParamsTransaction>& info) -> std::string {
        return info.param.as_test_suffix();
    }
);
