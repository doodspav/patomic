#include <test/common/make_ops.hpp>
#include <test/common/type_traits.hpp>


#define CREATE_SETTER_LAMBDA(name)                                \
    const auto set_##name = [](T& ops) noexcept -> void {         \
        ops.fp_##name = test::convertible_to_any<void(*)(void)> { \
            only_for_address                                      \
        };                                                        \
    }


#define DEFINE_MAKE_OPS_COMBINATIONS_IET(fn_name, type_name, ops_holder_type)              \
    std::vector<ops_holder_type<patomic_ops##type_name##t>>                                \
    make_ops_##fn_name##_combinations_implicit()                                           \
    {                                                                                      \
        return make_ops_##fn_name##_combinations<patomic_ops##type_name##t>();             \
    }                                                                                      \
                                                                                           \
    std::vector<ops_holder_type<patomic_ops_explicit##type_name##t>>                       \
    make_ops_##fn_name##_combinations_explicit()                                           \
    {                                                                                      \
        return make_ops_##fn_name##_combinations<patomic_ops_explicit##type_name##t>();    \
    }                                                                                      \
                                                                                           \
    std::vector<ops_holder_type<patomic_ops_transaction##type_name##t>>                    \
    make_ops_##fn_name##_combinations_transaction()                                        \
    {                                                                                      \
        return make_ops_##fn_name##_combinations<patomic_ops_transaction##type_name##t>(); \
    }                                                                                      \
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
        // set initial values
        combinations[i].all = true;
        combinations[i].any = false;

        // set necessary members and update values
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
class SettersVf
{
public:
    SettersVf() noexcept = default;

    SettersVf& v(void(*set_v)(T&)) noexcept
    {
        this->set_void = set_v;
        return *this;
    }

    SettersVf& f(void(*set_f)(T&)) noexcept
    {
        this->set_fetch = set_f;
        return *this;
    }

    void (*set_void)(T&) = nullptr;
    void (*set_fetch)(T&) = nullptr;
};


template <class T>
std::vector<test::OpsAnyAllVf<T>>
make_ops_combinations(const std::vector<SettersVf<T>>& setters)
{
    // setup
    const auto sqrt_size = (1u << setters.size());
    std::vector<test::OpsAnyAllVf<T>> combinations;
    combinations.resize(sqrt_size * sqrt_size);

    // go through all combinations
    for (std::size_t i_fetch = 0; i_fetch < sqrt_size; ++i_fetch)
    {
        for (std::size_t i_void = 0; i_void < sqrt_size; ++i_void)
        {
            // set initial values
            const auto i = i_void + (i_fetch * sqrt_size);
            combinations[i].all_void = true;
            combinations[i].all_fetch = true;
            combinations[i].any_void = false;
            combinations[i].any_fetch = false;

            for (std::size_t j = 0; j < setters.size(); ++j)
            {
                // conditionally set void operations
                if (i_void & (1u << j))
                {
                    setters[j].set_void(combinations[i].ops);
                    combinations[i].any_void = true;
                }
                else
                {
                    combinations[i].all_void = false;
                }

                // conditionally set fetch operations
                if (i_fetch & (1u << j))
                {
                    setters[j].set_fetch(combinations[i].ops);
                    combinations[i].any_fetch = true;
                }
                else
                {
                    combinations[i].all_fetch = false;
                }
            }
        }
    }

    // return
    return combinations;
}


template <class T>
std::vector<test::OpsAnyAll<T>>
make_ops_ldst_combinations()
{
    // lambda helpers
    CREATE_SETTER_LAMBDA(store);
    CREATE_SETTER_LAMBDA(load);
    const std::vector<void(*)(T&)> setters {
        set_store,
        set_load
    };

    // create all combinations
    return make_ops_combinations(setters);
}


template <class T>
std::vector<test::OpsAnyAll<T>>
make_ops_xchg_combinations()
{
    // lambda helpers
    CREATE_SETTER_LAMBDA(exchange);
    CREATE_SETTER_LAMBDA(cmpxchg_weak);
    CREATE_SETTER_LAMBDA(cmpxchg_strong);
    const std::vector<void(*)(T&)> setters {
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
std::vector<test::OpsAnyAllVf<T>>
make_ops_binary_combinations()
{
    // lambda helpers
    CREATE_SETTER_LAMBDA(or);
    CREATE_SETTER_LAMBDA(xor);
    CREATE_SETTER_LAMBDA(and);
    CREATE_SETTER_LAMBDA(not);
    CREATE_SETTER_LAMBDA(fetch_or);
    CREATE_SETTER_LAMBDA(fetch_xor);
    CREATE_SETTER_LAMBDA(fetch_and);
    CREATE_SETTER_LAMBDA(fetch_not);
    std::vector<SettersVf<T>> setters {
        SettersVf<T>().v(set_or).f(set_fetch_or),
        SettersVf<T>().v(set_xor).f(set_fetch_xor),
        SettersVf<T>().v(set_and).f(set_fetch_and),
        SettersVf<T>().v(set_not).f(set_fetch_not)
    };

    // create all combinations
    return make_ops_combinations(setters);
}


template <class T>
std::vector<test::OpsAnyAllVf<T>>
make_ops_arithmetic_combinations()
{
    // lambda helpers
    CREATE_SETTER_LAMBDA(add);
    CREATE_SETTER_LAMBDA(sub);
    CREATE_SETTER_LAMBDA(inc);
    CREATE_SETTER_LAMBDA(dec);
    CREATE_SETTER_LAMBDA(neg);
    CREATE_SETTER_LAMBDA(fetch_add);
    CREATE_SETTER_LAMBDA(fetch_sub);
    CREATE_SETTER_LAMBDA(fetch_inc);
    CREATE_SETTER_LAMBDA(fetch_dec);
    CREATE_SETTER_LAMBDA(fetch_neg);
    std::vector<SettersVf<T>> setters {
        SettersVf<T>().v(set_add).f(set_fetch_add),
        SettersVf<T>().v(set_sub).f(set_fetch_sub),
        SettersVf<T>().v(set_inc).f(set_fetch_inc),
        SettersVf<T>().v(set_dec).f(set_fetch_dec),
        SettersVf<T>().v(set_neg).f(set_fetch_neg),
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


DEFINE_MAKE_OPS_COMBINATIONS_IET(ldst, _, OpsAnyAll);


DEFINE_MAKE_OPS_COMBINATIONS_IET(xchg, _xchg_, OpsAnyAll);


DEFINE_MAKE_OPS_COMBINATIONS_IET(bitwise, _bitwise_, OpsAnyAll);


DEFINE_MAKE_OPS_COMBINATIONS_IET(binary, _binary_, OpsAnyAllVf);


DEFINE_MAKE_OPS_COMBINATIONS_IET(arithmetic, _arithmetic_, OpsAnyAllVf);


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