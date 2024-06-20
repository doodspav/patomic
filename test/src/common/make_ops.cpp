#include <test/common/make_ops.hpp>
#include <test/common/utility.hpp>


#define CREATE_SETTER_LAMBDA(name, opkind)                                     \
    const auto set_##name = [](T& ops, unsigned int& opkinds,                  \
                               void(*nonnull_value)()) noexcept -> void {      \
        ops.fp_##name = test::convertible_to_any<void(*)()> { nonnull_value }; \
        opkinds |= patomic_opkind_##opkind;                                    \
    }


#define DEFINE_MAKE_OPS_COMBINATIONS_IET(fn_name, type_name, ops_holder_type)          \
    template <>                                                                        \
    std::vector<ops_holder_type<patomic_ops##type_name##t>>                            \
    make_ops_##fn_name##_combinations<ops_domain::IMPLICIT>()                          \
    {                                                                                  \
        using T = patomic_ops##type_name##t;                                           \
        return ::make_ops_##fn_name##_combinations<T>(&::only_for_address);            \
    }                                                                                  \
                                                                                       \
    template <>                                                                        \
    std::vector<ops_holder_type<patomic_ops##type_name##t>>                            \
    make_ops_##fn_name##_combinations<ops_domain::IMPLICIT>(void(*nonnull_value)())    \
    {                                                                                  \
        using T = patomic_ops##type_name##t;                                           \
        return ::make_ops_##fn_name##_combinations<T>(nonnull_value);                  \
    }                                                                                  \
                                                                                       \
    template <>                                                                        \
    std::vector<ops_holder_type<patomic_ops_explicit##type_name##t>>                   \
    make_ops_##fn_name##_combinations<ops_domain::EXPLICIT>()                          \
    {                                                                                  \
        using T = patomic_ops_explicit##type_name##t;                                  \
        return ::make_ops_##fn_name##_combinations<T>(&::only_for_address);            \
    }                                                                                  \
                                                                                       \
    template <>                                                                        \
    std::vector<ops_holder_type<patomic_ops_explicit##type_name##t>>                   \
    make_ops_##fn_name##_combinations<ops_domain::EXPLICIT>(void(*nonnull_value)())    \
    {                                                                                  \
        using T = patomic_ops_explicit##type_name##t;                                  \
        return ::make_ops_##fn_name##_combinations<T>(nonnull_value);                  \
    }                                                                                  \
                                                                                       \
    template <>                                                                        \
    std::vector<ops_holder_type<patomic_ops_transaction##type_name##t>>                \
    make_ops_##fn_name##_combinations<ops_domain::TRANSACTION>()                       \
    {                                                                                  \
        using T = patomic_ops_transaction##type_name##t;                               \
        return ::make_ops_##fn_name##_combinations<T>(&::only_for_address);            \
    }                                                                                  \
                                                                                       \
    template <>                                                                        \
    std::vector<ops_holder_type<patomic_ops_transaction##type_name##t>>                \
    make_ops_##fn_name##_combinations<ops_domain::TRANSACTION>(void(*nonnull_value)()) \
    {                                                                                  \
        using T = patomic_ops_transaction##type_name##t;                               \
        return ::make_ops_##fn_name##_combinations<T>(nonnull_value);                  \
    }                                                                                  \
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
make_ops_all_nonnull_except_transaction_specific(void(*nonnull_value)()) noexcept
{
    // setup
    T ops {};
    const test::convertible_to_any<void(*)(void)> non_null {
        nonnull_value
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
std::vector<test::ops_any_all<T>>
make_ops_combinations(const std::vector<void(*)(T&, unsigned int&, void(*)())>& setters,
                      void(*nonnull_value)())
{
    // setup
    std::vector<test::ops_any_all<T>> combinations;
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
                setters[j](combinations[i].ops, combinations[i].opkinds, nonnull_value);
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
class setters_vf
{
public:
    setters_vf() noexcept = default;

    setters_vf& v(void(*set_v)(T&, unsigned int&, void(*)())) noexcept
    {
        this->set_void = set_v;
        return *this;
    }

    setters_vf& f(void(*set_f)(T&, unsigned int&, void(*)())) noexcept
    {
        this->set_fetch = set_f;
        return *this;
    }

    void (*set_void)(T&, unsigned int&, void(*)()) = nullptr;
    void (*set_fetch)(T&, unsigned int&, void(*)()) = nullptr;
};


template <class T>
std::vector<test::ops_any_all_vf<T>>
make_ops_combinations(const std::vector<setters_vf<T>>& setters, void(*nonnull_value)())
{
    // setup
    const auto sqrt_size = (1u << setters.size());
    std::vector<test::ops_any_all_vf<T>> combinations;
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
                    setters[j].set_void(
                        combinations[i].ops, combinations[i].opkinds_void, nonnull_value);
                    combinations[i].any_void = true;
                }
                else
                {
                    combinations[i].all_void = false;
                }

                // conditionally set fetch operations
                if (i_fetch & (1u << j))
                {
                    setters[j].set_fetch(
                        combinations[i].ops, combinations[i].opkinds_fetch, nonnull_value);
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
std::vector<test::ops_any_all<T>>
make_ops_ldst_combinations(void(*nonnull_value)())
{
    // lambda helpers
    CREATE_SETTER_LAMBDA(store, STORE);
    CREATE_SETTER_LAMBDA(load, LOAD);
    const std::vector<void(*)(T&, unsigned int&, void(*)())> setters {
        set_store,
        set_load
    };

    // create all combinations
    return make_ops_combinations(setters, nonnull_value);
}


template <class T>
std::vector<test::ops_any_all<T>>
make_ops_xchg_combinations(void(*nonnull_value)())
{
    // lambda helpers
    CREATE_SETTER_LAMBDA(exchange, EXCHANGE);
    CREATE_SETTER_LAMBDA(cmpxchg_weak, CMPXCHG_WEAK);
    CREATE_SETTER_LAMBDA(cmpxchg_strong, CMPXCHG_STRONG);
    const std::vector<void(*)(T&, unsigned int&, void(*)())> setters {
        set_exchange,
        set_cmpxchg_weak,
        set_cmpxchg_strong
    };

    // create all combinations
    return make_ops_combinations(setters, nonnull_value);
}


template <class T>
std::vector<test::ops_any_all<T>>
make_ops_bitwise_combinations(void(*nonnull_value)())
{
    // lambda helpers
    CREATE_SETTER_LAMBDA(test, TEST);
    CREATE_SETTER_LAMBDA(test_compl, TEST_COMPL);
    CREATE_SETTER_LAMBDA(test_set, TEST_SET);
    CREATE_SETTER_LAMBDA(test_reset, TEST_RESET);
    const std::vector<void(*)(T&, unsigned int&, void(*)())> setters {
        set_test,
        set_test_compl,
        set_test_set,
        set_test_reset
    };

    // create all combinations
    return make_ops_combinations(setters, nonnull_value);
}


template <class T>
std::vector<test::ops_any_all_vf<T>>
make_ops_binary_combinations(void(*nonnull_value)())
{
    // lambda helpers
    CREATE_SETTER_LAMBDA(or, OR);
    CREATE_SETTER_LAMBDA(xor, XOR);
    CREATE_SETTER_LAMBDA(and, AND);
    CREATE_SETTER_LAMBDA(not, NOT);
    CREATE_SETTER_LAMBDA(fetch_or, OR);
    CREATE_SETTER_LAMBDA(fetch_xor, XOR);
    CREATE_SETTER_LAMBDA(fetch_and, AND);
    CREATE_SETTER_LAMBDA(fetch_not, NOT);
    const std::vector<setters_vf<T>> setters {
        setters_vf<T>().v(set_or).f(set_fetch_or),
        setters_vf<T>().v(set_xor).f(set_fetch_xor),
        setters_vf<T>().v(set_and).f(set_fetch_and),
        setters_vf<T>().v(set_not).f(set_fetch_not)
    };

    // create all combinations
    return make_ops_combinations(setters, nonnull_value);
}


template <class T>
std::vector<test::ops_any_all_vf<T>>
make_ops_arithmetic_combinations(void(*nonnull_value)())
{
    // lambda helpers
    CREATE_SETTER_LAMBDA(add, ADD);
    CREATE_SETTER_LAMBDA(sub, SUB);
    CREATE_SETTER_LAMBDA(inc, INC);
    CREATE_SETTER_LAMBDA(dec, DEC);
    CREATE_SETTER_LAMBDA(neg, NEG);
    CREATE_SETTER_LAMBDA(fetch_add, ADD);
    CREATE_SETTER_LAMBDA(fetch_sub, SUB);
    CREATE_SETTER_LAMBDA(fetch_inc, INC);
    CREATE_SETTER_LAMBDA(fetch_dec, DEC);
    CREATE_SETTER_LAMBDA(fetch_neg, NEG);
    const std::vector<setters_vf<T>> setters {
        setters_vf<T>().v(set_add).f(set_fetch_add),
        setters_vf<T>().v(set_sub).f(set_fetch_sub),
        setters_vf<T>().v(set_inc).f(set_fetch_inc),
        setters_vf<T>().v(set_dec).f(set_fetch_dec),
        setters_vf<T>().v(set_neg).f(set_fetch_neg),
    };

    // create all combinations
    return make_ops_combinations(setters, nonnull_value);
}


}  // namespace


namespace test
{


std::string
to_string(ops_domain domain)
{
    switch (domain)
    {
        case ops_domain::IMPLICIT:
            return "IMPLICIT";
        case ops_domain::EXPLICIT:
            return "EXPLICIT";
        case ops_domain::TRANSACTION:
            return "TRANSACTION";
        default:
            return "(unknown)";
    }
}


std::vector<patomic_opcat_t>
make_opcats_all_solo()
{
    return {
        patomic_opcat_NONE,
        patomic_opcat_LDST,
        patomic_opcat_XCHG,
        patomic_opcat_BIT,
        patomic_opcat_BIN_V,
        patomic_opcat_BIN_F,
        patomic_opcat_ARI_V,
        patomic_opcat_ARI_F,
        patomic_opcat_TSPEC,
        patomic_opcat_TFLAG,
        patomic_opcat_TRAW
    };
}


std::vector<patomic_opcat_t>
make_opcats_all_combined()
{
    return {
        patomic_opcats_BIN,
        patomic_opcats_ARI,
        patomic_opcats_IMPLICIT,
        patomic_opcats_EXPLICIT,
        patomic_opcats_TRANSACTION
    };
}


std::vector<patomic_opkind_t>
make_opkinds_all_solo()
{
    return {
        patomic_opkind_NONE,
        patomic_opkind_LOAD,
        patomic_opkind_STORE,
        patomic_opkind_EXCHANGE,
        patomic_opkind_CMPXCHG_WEAK,
        patomic_opkind_CMPXCHG_STRONG,
        patomic_opkind_TEST,
        patomic_opkind_TEST_SET,
        patomic_opkind_TEST_RESET,
        patomic_opkind_TEST_COMPL,
        patomic_opkind_CLEAR,
        patomic_opkind_OR,
        patomic_opkind_XOR,
        patomic_opkind_AND,
        patomic_opkind_NOT,
        patomic_opkind_ADD,
        patomic_opkind_SUB,
        patomic_opkind_INC,
        patomic_opkind_DEC,
        patomic_opkind_NEG,
        patomic_opkind_DOUBLE_CMPXCHG,
        patomic_opkind_MULTI_CMPXCHG,
        patomic_opkind_GENERIC,
        patomic_opkind_GENERIC_WFB,
        patomic_opkind_TBEGIN,
        patomic_opkind_TABORT,
        patomic_opkind_TCOMMIT,
        patomic_opkind_TTEST
    };
}


std::vector<patomic_opkind_t>
make_opkinds_all_combined()
{
    return {
        patomic_opkinds_LDST,
        patomic_opkinds_XCHG,
        patomic_opkinds_BIT,
        patomic_opkinds_BIN,
        patomic_opkinds_ARI,
        patomic_opkinds_TSPEC,
        patomic_opkinds_TFLAG,
        patomic_opkinds_TRAW
    };
}


template <>
patomic_ops_t
make_ops_all_nonnull<ops_domain::IMPLICIT>(void(*nonnull_value)()) noexcept
{
    using ops_t = patomic_ops_t;
    return ::make_ops_all_nonnull_except_transaction_specific<ops_t>(nonnull_value);
}


template <>
patomic_ops_explicit_t
make_ops_all_nonnull<ops_domain::EXPLICIT>(void(*nonnull_value)()) noexcept
{
    using ops_t = patomic_ops_explicit_t;
    return ::make_ops_all_nonnull_except_transaction_specific<ops_t>(nonnull_value);
}


template <>
patomic_ops_transaction_t
make_ops_all_nonnull<ops_domain::TRANSACTION>(void(*nonnull_value)()) noexcept
{
    using ops_t = patomic_ops_transaction_t;
    ops_t ops = ::make_ops_all_nonnull_except_transaction_specific<ops_t>(nonnull_value);
    const test::convertible_to_any<void(*)()> non_null {
        nonnull_value
    };

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


template <>
patomic_ops_t
make_ops_all_nonnull<ops_domain::IMPLICIT>() noexcept
{
    constexpr auto nonnull_value = &only_for_address;
    return make_ops_all_nonnull<ops_domain::IMPLICIT>(nonnull_value);
}


template <>
patomic_ops_explicit_t
make_ops_all_nonnull<ops_domain::EXPLICIT>() noexcept
{
    constexpr auto nonnull_value = &only_for_address;
    return make_ops_all_nonnull<ops_domain::EXPLICIT>(nonnull_value);
}


template <>
patomic_ops_transaction_t
make_ops_all_nonnull<ops_domain::TRANSACTION>() noexcept
{
    constexpr auto nonnull_value = &only_for_address;
    return make_ops_all_nonnull<ops_domain::TRANSACTION>(nonnull_value);
}


DEFINE_MAKE_OPS_COMBINATIONS_IET(ldst, _, ops_any_all);


DEFINE_MAKE_OPS_COMBINATIONS_IET(xchg, _xchg_, ops_any_all);


DEFINE_MAKE_OPS_COMBINATIONS_IET(bitwise, _bitwise_, ops_any_all);


DEFINE_MAKE_OPS_COMBINATIONS_IET(binary, _binary_, ops_any_all_vf);


DEFINE_MAKE_OPS_COMBINATIONS_IET(arithmetic, _arithmetic_, ops_any_all_vf);


std::vector<ops_any_all<patomic_ops_transaction_special_t>>
make_ops_special_combinations_transaction(void(*nonnull_value)())
{
    // lambda helpers
    using T = patomic_ops_transaction_special_t;
    CREATE_SETTER_LAMBDA(double_cmpxchg, DOUBLE_CMPXCHG);
    CREATE_SETTER_LAMBDA(multi_cmpxchg, MULTI_CMPXCHG);
    CREATE_SETTER_LAMBDA(generic, GENERIC);
    CREATE_SETTER_LAMBDA(generic_wfb, GENERIC_WFB);
    const std::vector<void(*)(T&, unsigned int&, void(*)())> setters {
        set_double_cmpxchg,
        set_multi_cmpxchg,
        set_generic,
        set_generic_wfb
    };

    // create all combinations
    return make_ops_combinations(setters, nonnull_value);
}


std::vector<ops_any_all<patomic_ops_transaction_special_t>>
make_ops_special_combinations_transaction()
{
    // defer to overload
    return make_ops_special_combinations_transaction(&::only_for_address);
}


std::vector<ops_any_all<patomic_ops_transaction_flag_t>>
make_ops_flag_combinations_transaction(void(*nonnull_value)())
{
    // lambda helpers
    using T = patomic_ops_transaction_flag_t;
    CREATE_SETTER_LAMBDA(test, TEST);
    CREATE_SETTER_LAMBDA(test_set, TEST_SET);
    CREATE_SETTER_LAMBDA(clear, CLEAR);
    const std::vector<void(*)(T&, unsigned int&, void(*)())> setters {
        set_test,
        set_test_set,
        set_clear
    };

    // create all combinations
    return make_ops_combinations(setters, nonnull_value);
}


std::vector<ops_any_all<patomic_ops_transaction_flag_t>>
make_ops_flag_combinations_transaction()
{
    // defer to overload
    return make_ops_flag_combinations_transaction(&::only_for_address);
}


std::vector<ops_any_all<patomic_ops_transaction_raw_t>>
make_ops_raw_combinations_transaction(void(*nonnull_value)())
{
    // lambda helpers
    using T = patomic_ops_transaction_raw_t;
    CREATE_SETTER_LAMBDA(tbegin, TBEGIN);
    CREATE_SETTER_LAMBDA(tabort, TABORT);
    CREATE_SETTER_LAMBDA(tcommit, TCOMMIT);
    CREATE_SETTER_LAMBDA(ttest, TTEST);
    const std::vector<void(*)(T&, unsigned int&, void(*)())> setters {
        set_tbegin,
        set_tabort,
        set_tcommit,
        set_ttest
    };

    // create all combinations
    return make_ops_combinations(setters, nonnull_value);
}


std::vector<ops_any_all<patomic_ops_transaction_raw_t>>
make_ops_raw_combinations_transaction()
{
    // defer to overload
    return make_ops_raw_combinations_transaction(&::only_for_address);
}


}  // namespace test
