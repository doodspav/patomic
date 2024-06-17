#ifndef PATOMIC_TEST_PATOMIC_MAKE_OPS_HPP
#define PATOMIC_TEST_PATOMIC_MAKE_OPS_HPP

#include <patomic/types/ops.h>

#include <vector>


namespace test
{


/// @brief
///   Helper enum to use as a template parameter.
enum class ops_domain
{
    IMPLICIT,
    EXPLICIT,
    TRANSACTION
};


/// @brief
///   Helper type to convert from domain to patomic ops types.
template <ops_domain D>
struct ops_types;

template <>
struct ops_types<ops_domain::IMPLICIT>
{
    using base_t = patomic_ops_t;
    using xchg_t = patomic_ops_xchg_t;
    using bitwise_t = patomic_ops_bitwise_t;
    using binary_t = patomic_ops_binary_t;
    using arithmetic_t = patomic_ops_arithmetic_t;
};

template <>
struct ops_types<ops_domain::EXPLICIT>
{
    using base_t = patomic_ops_explicit_t;
    using xchg_t = patomic_ops_explicit_xchg_t;
    using bitwise_t = patomic_ops_explicit_bitwise_t;
    using binary_t = patomic_ops_explicit_binary_t;
    using arithmetic_t = patomic_ops_explicit_arithmetic_t;
};

template <>
struct ops_types<ops_domain::TRANSACTION>
{
    using base_t = patomic_ops_transaction_t;
    using xchg_t = patomic_ops_transaction_xchg_t;
    using bitwise_t = patomic_ops_transaction_bitwise_t;
    using binary_t = patomic_ops_transaction_binary_t;
    using arithmetic_t = patomic_ops_transaction_arithmetic_t;
    using special_t = patomic_ops_transaction_special_t;
    using flag_t = patomic_ops_transaction_flag_t;
    using raw_t = patomic_ops_transaction_raw_t;
};


/// @brief
///   Helper struct to indicate if any/all function pointers are set in the ops
///   member.
template <class T>
struct ops_any_all
{
    T ops {};
    bool any {};
    bool all {};
};


/// @brief
///   Helper struct to indicate if any/all function pointers are set in the ops
///   member. Differentiates between void and fetch operations.
template <class T>
struct ops_any_all_vf
{
    T ops {};
    bool any_void {};
    bool any_fetch {};
    bool all_void {};
    bool all_fetch {};
};


/// @brief
///   Create a set of patomic_ops*_t objects with all combinations of fp_store
///   and fp_load members set to null and non-null values. All other members
///   are null.
template <ops_domain D>
std::vector<ops_any_all<typename ops_types<D>::base_t>>
make_ops_ldst_combinations();


/// @brief
///   Create a set of patomic_ops*_xchg_t objects with all combinations of
///   members set to null and non-null values.
template <ops_domain D>
std::vector<ops_any_all<typename ops_types<D>::xchg_t>>
make_ops_xchg_combinations();


/// @brief
///   Create a set of patomic_ops*_bitwise_t objects with all combinations of
///   members set to null and non-null values.
template <ops_domain D>
std::vector<ops_any_all<typename ops_types<D>::bitwise_t>>
make_ops_bitwise_combinations();


/// @brief
///   Create a set of patomic_ops*_binary_t objects with all combinations of
///   void and fetch members set to null and non-null values.
template <ops_domain D>
std::vector<ops_any_all_vf<typename ops_types<D>::binary_t>>
make_ops_binary_combinations();


/// @brief
///   Create a set of patomic_ops*_arithmetic_t objects with all combinations of
///   void and fetch members set to null and non-null values.
template <ops_domain D>
std::vector<ops_any_all_vf<typename ops_types<D>::arithmetic_t>>
make_ops_arithmetic_combinations();


/// @brief
///   Create a set of patomic_ops_transaction_special_t objects with all
///   combinations of members set to null and non-null values.
std::vector<ops_any_all<patomic_ops_transaction_special_t>>
make_ops_special_combinations_transaction();


/// @brief
///   Create a set of patomic_ops_transaction_flag_t objects with all
///   combinations of members set to null and non-null values.
std::vector<ops_any_all<patomic_ops_transaction_flag_t>>
make_ops_flag_combinations_transaction();


/// @brief
///   Create a set of patomic_ops_transaction_raw_t objects with all
///   combinations of members set to null and non-null values.
std::vector<ops_any_all<patomic_ops_transaction_raw_t>>
make_ops_raw_combinations_transaction();


}  // namespace test

#endif  // PATOMIC_TEST_PATOMIC_MAKE_OPS_HPP