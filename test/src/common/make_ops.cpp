#include <test/common/make_ops.hpp>
#include <test/common/type_traits.hpp>


#define CREATE_SETTER_LAMBDA(name)                                \
    auto set_##name = [](T& ops) noexcept -> void {               \
        ops.fp_##name = test::convertible_to_any<void(*)(void)> { \
            only_for_address                                      \
        };                                                        \
    }


#define DEFINE_MAKE_OPS_COMBINATIONS_IET(fn_name, type_name)                                 \
    std::vector<OpsAnyAll<patomic_ops_##type_name##_t>>                                      \
    make_ops_##fn_name##_combinations_implicit()                                             \
    {                                                                                        \
        return make_ops_##fn_name##_combinations<patomic_ops_##type_name##_t>();             \
    }                                                                                        \
                                                                                             \
    std::vector<OpsAnyAll<patomic_ops_explicit_##type_name##_t>>                             \
    make_ops_##fn_name##_combinations_explicit()                                             \
    {                                                                                        \
        return make_ops_##fn_name##_combinations<patomic_ops_explicit_##type_name##_t>();    \
    }                                                                                        \
                                                                                             \
    std::vector<OpsAnyAll<patomic_ops_transaction_##type_name##_t>>                          \
    make_ops_##fn_name##_combinations_transaction()                                          \
    {                                                                                        \
        return make_ops_##fn_name##_combinations<patomic_ops_transaction_##type_name##_t>(); \
    }                                                                                        \
                                                                                             \
    static_assert(!!true, "require semicolon")


namespace
{


constexpr void
only_for_address() noexcept
{}

static_assert(&only_for_address != nullptr, "address must be non-null");


template <class T>
constexpr T
make_ops_nonnull_non_transaction_specific() noexcept
{
    // setup
    T ops {};
    constexpr test::convertible_to_any<void(*)(void)> non_null {
        only_for_address
    };

    // initialize all members to be non-null
    // LDST
    ops.fp_store = non_null;
    ops.fp_load  = non_null;
    // XCHG
    ops.xchg_ops.fp_exchange       = non_null;
    ops.xchg_ops.fp_cmpxchg_weak   = non_null;
    ops.xchg_ops.fp_cmpxchg_strong = non_null;
    // BIT
    ops.bitwise_ops.fp_test       = non_null;
    ops.bitwise_ops.fp_test_compl = non_null;
    ops.bitwise_ops.fp_test_set   = non_null;
    ops.bitwise_ops.fp_test_reset = non_null;
    // BIN_V
    ops.binary_ops.fp_or  = non_null;
    ops.binary_ops.fp_xor = non_null;
    ops.binary_ops.fp_and = non_null;
    ops.binary_ops.fp_not = non_null;
    // BIN_F
    ops.binary_ops.fp_fetch_or  = non_null;
    ops.binary_ops.fp_fetch_xor = non_null;
    ops.binary_ops.fp_fetch_and = non_null;
    ops.binary_ops.fp_fetch_not = non_null;
    // ARI_V
    ops.arithmetic_ops.fp_add = non_null;
    ops.arithmetic_ops.fp_sub = non_null;
    ops.arithmetic_ops.fp_inc = non_null;
    ops.arithmetic_ops.fp_dec = non_null;
    ops.arithmetic_ops.fp_neg = non_null;
    // ARI_F
    ops.arithmetic_ops.fp_fetch_add = non_null;
    ops.arithmetic_ops.fp_fetch_sub = non_null;
    ops.arithmetic_ops.fp_fetch_inc = non_null;
    ops.arithmetic_ops.fp_fetch_dec = non_null;
    ops.arithmetic_ops.fp_fetch_neg = non_null;

    // return fully initialized object
    return ops;
}


template <class T>
std::vector<test::OpsAnyAll<T>>
make_ops_combinations(const std::vector<void(*)(T&)>& setters)
{
    // setup
    std::vector<test::OpsAnyAll<T>> combinations;
    combinations.resize(1u << setters.size());

    // go through all combinations
    for (std::size_t i = 0; i < combinations.size(); ++i)
    {
        combinations[i].all = true;
        combinations[i].any = false;
        for (std::size_t j = 0; j < setters.size(); ++j)
        {
            if (i & (1u << j))
            {
                setters[j](combinations[i].ops);
                combinations[i].any = true;
            }
            else
            {
                combinations[i].all = false;
            }
        }
    }

    // return
    return combinations;
}


template <class T>
std::vector<test::OpsAnyAll<T>>
make_ops_xchg_combinations()
{
    // lambda helpers
    CREATE_SETTER_LAMBDA(exchange);
    CREATE_SETTER_LAMBDA(cmpxchg_weak);
    CREATE_SETTER_LAMBDA(cmpxchg_strong);
    std::vector<void(*)(T&)> setters {
        set_exchange,
        set_cmpxchg_weak,
        set_cmpxchg_strong
    };

    // create all combinations
    return make_ops_combinations(setters);
}


template <class T>
std::vector<test::OpsAnyAll<T>>
make_ops_bitwise_combinations()
{
    // lambda helpers
    CREATE_SETTER_LAMBDA(test);
    CREATE_SETTER_LAMBDA(test_compl);
    CREATE_SETTER_LAMBDA(test_set);
    CREATE_SETTER_LAMBDA(test_reset);
    std::vector<void(*)(T&)> setters {
        set_test,
        set_test_compl,
        set_test_set,
        set_test_reset
    };

    // create all combinations
    return make_ops_combinations(setters);
}


template <class T>
std::vector<test::OpsAnyAll<T>>
make_ops_binary_void_combinations()
{
    // lambda helpers
    CREATE_SETTER_LAMBDA(or);
    CREATE_SETTER_LAMBDA(xor);
    CREATE_SETTER_LAMBDA(and);
    CREATE_SETTER_LAMBDA(not);
    std::vector<void(*)(T&)> setters {
        set_or,
        set_xor,
        set_and,
        set_not
    };

    // create all combinations
    return make_ops_combinations(setters);
}


template <class T>
std::vector<test::OpsAnyAll<T>>
make_ops_binary_fetch_combinations()
{
    // lambda helpers
    CREATE_SETTER_LAMBDA(fetch_or);
    CREATE_SETTER_LAMBDA(fetch_xor);
    CREATE_SETTER_LAMBDA(fetch_and);
    CREATE_SETTER_LAMBDA(fetch_not);
    std::vector<void(*)(T&)> setters {
        set_fetch_or,
        set_fetch_xor,
        set_fetch_and,
        set_fetch_not
    };

    // create all combinations
    return make_ops_combinations(setters);
}


template <class T>
std::vector<test::OpsAnyAll<T>>
make_ops_arithmetic_void_combinations()
{
    // lambda helpers
    CREATE_SETTER_LAMBDA(add);
    CREATE_SETTER_LAMBDA(sub);
    CREATE_SETTER_LAMBDA(inc);
    CREATE_SETTER_LAMBDA(dec);
    CREATE_SETTER_LAMBDA(neg);
    std::vector<void(*)(T&)> setters {
        set_add,
        set_sub,
        set_inc,
        set_dec,
        set_neg
    };

    // create all combinations
    return make_ops_combinations(setters);
}


template <class T>
std::vector<test::OpsAnyAll<T>>
make_ops_arithmetic_fetch_combinations()
{
    // lambda helpers
    CREATE_SETTER_LAMBDA(fetch_add);
    CREATE_SETTER_LAMBDA(fetch_sub);
    CREATE_SETTER_LAMBDA(fetch_inc);
    CREATE_SETTER_LAMBDA(fetch_dec);
    CREATE_SETTER_LAMBDA(fetch_neg);
    std::vector<void(*)(T&)> setters {
        set_fetch_add,
        set_fetch_sub,
        set_fetch_inc,
        set_fetch_dec,
        set_fetch_neg
    };

    // create all combinations
    return make_ops_combinations(setters);
}


}  // namespace


namespace test
{


patomic_ops_t
make_ops_nonnull_implicit() noexcept
{
    using OpsT = patomic_ops_t;
    return make_ops_nonnull_non_transaction_specific<OpsT>();
}


patomic_ops_explicit_t
make_ops_nonnull_explicit() noexcept
{
    using OpsT = patomic_ops_explicit_t;
    return make_ops_nonnull_non_transaction_specific<OpsT>();
}


patomic_ops_transaction_t
make_ops_nonnull_transaction() noexcept
{
    // setup
    using OpsT = patomic_ops_transaction_t;
    OpsT ops = make_ops_nonnull_non_transaction_specific<OpsT>();
    test::convertible_to_any<void(*)(void)> non_null { only_for_address };

    // initialize all transaction specific members to be non-null
    // TSPEC
    ops.special_ops.fp_double_cmpxchg = non_null;
    ops.special_ops.fp_multi_cmpxchg  = non_null;
    ops.special_ops.fp_generic        = non_null;
    ops.special_ops.fp_generic_wfb    = non_null;
    // TFLAG
    ops.flag_ops.fp_test     = non_null;
    ops.flag_ops.fp_test_set = non_null;
    ops.flag_ops.fp_clear    = non_null;
    // TRAW
    ops.raw_ops.fp_tbegin  = non_null;
    ops.raw_ops.fp_tabort  = non_null;
    ops.raw_ops.fp_tcommit = non_null;
    ops.raw_ops.fp_ttest   = non_null;

    // return fully nonnull ops
    return ops;
}


DEFINE_MAKE_OPS_COMBINATIONS_IET(xchg, xchg);


DEFINE_MAKE_OPS_COMBINATIONS_IET(bitwise, bitwise);


DEFINE_MAKE_OPS_COMBINATIONS_IET(binary_void, binary);


DEFINE_MAKE_OPS_COMBINATIONS_IET(binary_fetch, binary);


DEFINE_MAKE_OPS_COMBINATIONS_IET(arithmetic_void, arithmetic);


DEFINE_MAKE_OPS_COMBINATIONS_IET(arithmetic_fetch, arithmetic);


std::vector<OpsAnyAll<patomic_ops_transaction_special_t>>
make_ops_special_combinations_transaction()
{
    // lambda helpers
    using T = patomic_ops_transaction_special_t;
    CREATE_SETTER_LAMBDA(double_cmpxchg);
    CREATE_SETTER_LAMBDA(multi_cmpxchg);
    CREATE_SETTER_LAMBDA(generic);
    CREATE_SETTER_LAMBDA(generic_wfb);
    std::vector<void(*)(T&)> setters {
        set_double_cmpxchg,
        set_multi_cmpxchg,
        set_generic,
        set_generic_wfb
    };

    // create all combinations
    return make_ops_combinations(setters);
}


std::vector<OpsAnyAll<patomic_ops_transaction_flag_t>>
make_ops_flag_combinations_transaction()
{
    // lambda helpers
    using T = patomic_ops_transaction_flag_t;
    CREATE_SETTER_LAMBDA(test);
    CREATE_SETTER_LAMBDA(test_set);
    CREATE_SETTER_LAMBDA(clear);
    std::vector<void(*)(T&)> setters {
        set_test,
        set_test_set,
        set_clear
    };

    // create all combinations
    return make_ops_combinations(setters);
}


std::vector<OpsAnyAll<patomic_ops_transaction_raw_t>>
make_ops_raw_combinations_transaction()
{
    // lambda helpers
    using T = patomic_ops_transaction_raw_t;
    CREATE_SETTER_LAMBDA(tbegin);
    CREATE_SETTER_LAMBDA(tabort);
    CREATE_SETTER_LAMBDA(tcommit);
    CREATE_SETTER_LAMBDA(ttest);
    std::vector<void(*)(T&)> setters {
        set_tbegin,
        set_tabort,
        set_tcommit,
        set_ttest
    };

    // create all combinations
    return make_ops_combinations(setters);
}


}  // namespace test