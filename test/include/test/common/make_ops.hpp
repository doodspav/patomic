#ifndef PATOMIC_TEST_PATOMIC_MAKE_OPS_HPP
#define PATOMIC_TEST_PATOMIC_MAKE_OPS_HPP

#include <patomic/types/ops.h>

#include <vector>


namespace test
{


/// @brief
///   Create a patomic_ops_t object where all function pointers are set to an
///   unspecified non-null value.
///
/// @warning
///   It is undefined behaviour to call function pointers set to the
///   unspecified value. Value comparison is valid.
patomic_ops_t
make_ops_nonnull_implicit() noexcept;


/// @brief
///   Create a patomic_ops_explicit_t object where all function pointers are
///   set to an unspecified non-null value.
///
/// @warning
///   It is undefined behaviour to call function pointers set to the
///   unspecified value. Value comparison is valid.
patomic_ops_explicit_t
make_ops_nonnull_explicit() noexcept;


/// @brief
///   Create a patomic_ops_transaction_t object where all function pointers are
///   set to an unspecified non-null value.
///
/// @warning
///   It is undefined behaviour to call function pointers set to the
///   unspecified value. Value comparison is valid.
patomic_ops_transaction_t
make_ops_nonnull_transaction() noexcept;


/// @brief
///   Helper struct to indicate if any/all function pointers are set in the ops
///   member.
template <class T>
struct OpsAnyAll
{
    T ops {};
    bool any {};
    bool all {};
};


/// @brief
///   Create a set of patomic_ops_xchg_t objects with all combinations of
///   members set to null and non-null values.
std::vector<OpsAnyAll<patomic_ops_xchg_t>>
make_ops_xchg_combinations_implicit();


/// @brief
///   Create a set of patomic_ops_explicit_xchg_t objects with all combinations
///   of members set to null and non-null values.
std::vector<OpsAnyAll<patomic_ops_explicit_xchg_t>>
make_ops_xchg_combinations_explicit();


/// @brief
///   Create a set of patomic_ops_transaction_xchg_t objects with all
///   combinations of members set to null and non-null values.
std::vector<OpsAnyAll<patomic_ops_transaction_xchg_t>>
make_ops_xchg_combinations_transaction();


/// @brief
///   Create a set of patomic_ops_bitwise_t objects with all combinations of
///   members set to null and non-null values.
std::vector<OpsAnyAll<patomic_ops_bitwise_t>>
make_ops_bitwise_combinations_implicit();


/// @brief
///   Create a set of patomic_ops_explicit_bitwise_t objects with all
///   combinations of members set to null and non-null values.
std::vector<OpsAnyAll<patomic_ops_explicit_bitwise_t>>
make_ops_bitwise_combinations_explicit();


/// @brief
///   Create a set of patomic_ops_transaction_bitwise_t objects with all
///   combinations of members set to null and non-null values.
std::vector<OpsAnyAll<patomic_ops_transaction_bitwise_t>>
make_ops_bitwise_combinations_transaction();


/// @brief
///   Create a set of patomic_ops_binary_t objects with all combinations of
///   void members set to null and non-null values.
std::vector<OpsAnyAll<patomic_ops_binary_t>>
make_ops_binary_void_combinations_implicit();


/// @brief
///   Create a set of patomic_ops_explicit_binary_t objects with all
///   combinations of void members set to null and non-null values.
std::vector<OpsAnyAll<patomic_ops_explicit_binary_t>>
make_ops_binary_void_combinations_explicit();


/// @brief
///   Create a set of patomic_ops_transaction_binary_t objects with all
///   combinations of void members set to null and non-null values.
std::vector<OpsAnyAll<patomic_ops_transaction_binary_t>>
make_ops_binary_void_combinations_transaction();


/// @brief
///   Create a set of patomic_ops_binary_t objects with all combinations of
///   fetch members set to null and non-null values.
std::vector<OpsAnyAll<patomic_ops_binary_t>>
make_ops_binary_fetch_combinations_implicit();


/// @brief
///   Create a set of patomic_ops_explicit_binary_t objects with all
///   combinations of fetch members set to null and non-null values.
std::vector<OpsAnyAll<patomic_ops_explicit_binary_t>>
make_ops_binary_fetch_combinations_explicit();


/// @brief
///   Create a set of patomic_ops_transaction_binary_t objects with all
///   combinations of fetch members set to null and non-null values.
std::vector<OpsAnyAll<patomic_ops_transaction_binary_t>>
make_ops_binary_fetch_combinations_transaction();


/// @brief
///   Create a set of patomic_ops_arithmetic_t objects with all combinations of
///   void members set to null and non-null values.
std::vector<OpsAnyAll<patomic_ops_arithmetic_t>>
make_ops_arithmetic_void_combinations_implicit();


/// @brief
///   Create a set of patomic_ops_explicit_arithmetic_t objects with all
///   combinations of void members set to null and non-null values.
std::vector<OpsAnyAll<patomic_ops_explicit_arithmetic_t>>
make_ops_arithmetic_void_combinations_explicit();


/// @brief
///   Create a set of patomic_ops_transaction_arithmetic_t objects with all
///   combinations of void members set to null and non-null values.
std::vector<OpsAnyAll<patomic_ops_transaction_arithmetic_t>>
make_ops_arithmetic_void_combinations_transaction();


/// @brief
///   Create a set of patomic_ops_arithmetic_t objects with all combinations of
///   fetch members set to null and non-null values.
std::vector<OpsAnyAll<patomic_ops_arithmetic_t>>
make_ops_arithmetic_fetch_combinations_implicit();


/// @brief
///   Create a set of patomic_ops_explicit_arithmetic_t objects with all
///   combinations of fetch members set to null and non-null values.
std::vector<OpsAnyAll<patomic_ops_explicit_arithmetic_t>>
make_ops_arithmetic_fetch_combinations_explicit();


/// @brief
///   Create a set of patomic_ops_transaction_arithmetic_t objects with all
///   combinations of fetch members set to null and non-null values.
std::vector<OpsAnyAll<patomic_ops_transaction_arithmetic_t>>
make_ops_arithmetic_fetch_combinations_transaction();


/// @brief
///   Create a set of patomic_ops_transaction_special_t objects with all
///   combinations of members set to null and non-null values.
std::vector<OpsAnyAll<patomic_ops_transaction_special_t>>
make_ops_special_combinations_transaction();


/// @brief
///   Create a set of patomic_ops_transaction_flag_t objects with all
///   combinations of members set to null and non-null values.
std::vector<OpsAnyAll<patomic_ops_transaction_flag_t>>
make_ops_flag_combinations_transaction();


/// @brief
///   Create a set of patomic_ops_transaction_raw_t objects with all
///   combinations of members set to null and non-null values.
std::vector<OpsAnyAll<patomic_ops_transaction_raw_t>>
make_ops_raw_combinations_transaction();


}  // namespace test

#endif  // PATOMIC_TEST_PATOMIC_MAKE_OPS_HPP
