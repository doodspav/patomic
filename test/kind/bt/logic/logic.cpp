#include <test/suite/bt_logic.hpp>

#include <patomic/patomic.h>


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

    // initialize base members
    m_ops = pat.ops;
    m_recommended = pat.recommended;

    // configure generous attempts
    auto make_generous = [](unsigned long attempts) noexcept -> unsigned long {
        const unsigned long extra_attempts = 100ul + (attempts / 10ul);
        return std::max(attempts, attempts + extra_attempts);
    };
    m_config.attempts = make_generous(std::max(
        m_recommended.min_rmw_attempts,
        m_recommended.min_load_attempts
    ));
    m_config_wfb.attempts = make_generous(m_recommended.min_rmw_attempts);
    m_config_wfb.fallback_attempts = make_generous(m_recommended.min_load_attempts);
}


INSTANTIATE_TEST_SUITE_P(
    patomic,  // necessary, otherwise CTest pattern won't find tests
    BtLogicImplicit,
    testing::ConvertGenerator(
        testing::ValuesIn(test::ParamsImplicit::combinations())
    ),
    [](const testing::TestParamInfo<test::ParamsImplicit>& info) -> std::string {
        return info.param.as_test_suffix();
    }
);


INSTANTIATE_TEST_SUITE_P(
    patomic,  // necessary, otherwise CTest pattern won't find tests
    BtLogicImplicitStore,
    testing::ConvertGenerator(
        testing::ValuesIn(test::ParamsImplicit::combinations_store())
    ),
    [](const testing::TestParamInfo<test::ParamsImplicit>& info) -> std::string {
        return info.param.as_test_suffix();
    }
);


INSTANTIATE_TEST_SUITE_P(
    patomic,  // necessary, otherwise CTest pattern won't find tests
    BtLogicImplicitLoad,
    testing::ConvertGenerator(
        testing::ValuesIn(test::ParamsImplicit::combinations_load())
    ),
    [](const testing::TestParamInfo<test::ParamsImplicit>& info) -> std::string {
        return info.param.as_test_suffix();
    }
);


INSTANTIATE_TEST_SUITE_P(
    patomic,  // necessary, otherwise CTest pattern won't find tests
    BtLogicExplicit,
    testing::ConvertGenerator(
        testing::ValuesIn(test::ParamsExplicit::combinations())
    ),
    [](const testing::TestParamInfo<test::ParamsExplicit>& info) -> std::string {
        return info.param.as_test_suffix();
    }
);


INSTANTIATE_TEST_SUITE_P(
    patomic,  // necessary, otherwise CTest pattern won't find tests
    BtLogicExplicitStore,
    testing::ConvertGenerator(
        testing::ValuesIn(test::ParamsExplicit::combinations_store())
    ),
    [](const testing::TestParamInfo<test::ParamsExplicit>& info) -> std::string {
        return info.param.as_test_suffix();
    }
);


INSTANTIATE_TEST_SUITE_P(
    patomic,  // necessary, otherwise CTest pattern won't find tests
    BtLogicExplicitLoad,
    testing::ConvertGenerator(
        testing::ValuesIn(test::ParamsExplicit::combinations_load())
    ),
    [](const testing::TestParamInfo<test::ParamsExplicit>& info) -> std::string {
        return info.param.as_test_suffix();
    }
);


INSTANTIATE_TEST_SUITE_P(
    patomic,  // necessary, otherwise CTest pattern won't find tests
    BtLogicTransaction,
    testing::ConvertGenerator(
        testing::ValuesIn(test::ParamsTransaction::combinations())
    ),
    [](const testing::TestParamInfo<test::ParamsTransaction>& info) -> std::string {
        return info.param.as_test_suffix();
    }
);
