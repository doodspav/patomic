#include <test/common/make_ops.hpp>
#include <test/common/type_traits.hpp>

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
    test::convertible_to_any<void(*)(void)> non_null { only_for_address };

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


}  // namespace test