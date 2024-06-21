#ifndef PATOMIC_TEST_PATOMIC_MAKE_OPS_HPP
#define PATOMIC_TEST_PATOMIC_MAKE_OPS_HPP

#include <patomic/api/core.h>
#include <patomic/api/feature_check.h>
#include <patomic/api/ops.h>

#include <array>
#include <string>
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
///   Convert enum labels to strings.
std::string
to_string(ops_domain domain);


/// @brief
///   Helper type to convert from domain to patomic ops types.
template <ops_domain D>
struct ops_types;

template <>
struct ops_types<ops_domain::IMPLICIT>
{
    static constexpr unsigned int full_opcat = patomic_opcats_IMPLICIT;

    using base_t = patomic_t;
    using ldst_t = patomic_ops_t;
    using xchg_t = patomic_ops_xchg_t;
    using bitwise_t = patomic_ops_bitwise_t;
    using binary_t = patomic_ops_binary_t;
    using arithmetic_t = patomic_ops_arithmetic_t;
};

template <>
struct ops_types<ops_domain::EXPLICIT>
{
    static constexpr unsigned int full_opcat = patomic_opcats_EXPLICIT;

    using base_t = patomic_explicit_t;
    using ldst_t = patomic_ops_explicit_t;
    using xchg_t = patomic_ops_explicit_xchg_t;
    using bitwise_t = patomic_ops_explicit_bitwise_t;
    using binary_t = patomic_ops_explicit_binary_t;
    using arithmetic_t = patomic_ops_explicit_arithmetic_t;
};

template <>
struct ops_types<ops_domain::TRANSACTION>
{
    static constexpr unsigned int full_opcat = patomic_opcats_TRANSACTION;

    using base_t = patomic_transaction_t;
    using ldst_t = patomic_ops_transaction_t;
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
    unsigned int opkinds {};
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
    unsigned int opkinds_void {};
    unsigned int opkinds_fetch {};
};


/// @brief
///   Create a list of all valid patomic_opcat_t values with exactly zero or
///   one bits set.
std::vector<patomic_opcat_t>
make_opcats_all_solo();


/// @brief
///   Create a list of all valid patomic_opcat_t values with more than one bit
///   set.
std::vector<patomic_opcat_t>
make_opcats_all_combined();


/// @brief
///   Create a list of all valid patomic_opkind_t values with exactly zero or
///   one bits set.
std::vector<patomic_opkind_t>
make_opkinds_all_solo();


/// @brief
///   Create a list of all valid patomic_opkind_t values with more than one bit
///   set.
std::vector<patomic_opkind_t>
make_opkinds_all_combined();


/// @brief
///   Create a patomic_ops*_t object where all members are set to a provided
///   value.
template <ops_domain D>
typename ops_types<D>::ldst_t
make_ops_all_nonnull(void(*nonnull_value)()) noexcept;


/// @brief
///   Create a patomic_ops*_t object where all members are set to a non-null
///   value.
template <ops_domain D>
typename ops_types<D>::ldst_t
make_ops_all_nonnull() noexcept;


/// @brief
///   Create a set of patomic_ops*_t objects with all combinations of fp_store
///   and fp_load members set to a provided value. All other members are null.
template <ops_domain D>
std::vector<ops_any_all<typename ops_types<D>::ldst_t>>
make_ops_ldst_combinations(void(*nonnull_value)());


/// @brief
///   Create a set of patomic_ops*_t objects with all combinations of fp_store
///   and fp_load members set to null and non-null values. All other members
///   are null.
template <ops_domain D>
std::vector<ops_any_all<typename ops_types<D>::ldst_t>>
make_ops_ldst_combinations();


/// @brief
///   Create an array of fp_store and fp_load members in a patomic_ops*_t
///   object, with the types cast to void(*)().
template <ops_domain D>
std::array<void(*)(), 2>
make_ops_ldst_array(const typename ops_types<D>::ldst_t& ldst) noexcept;


/// @brief
///   Create a set of patomic_ops*_xchg_t objects with all combinations of
///   members set to a provided value. All other members are null.
template <ops_domain D>
std::vector<ops_any_all<typename ops_types<D>::xchg_t>>
make_ops_xchg_combinations(void(*nonnull_value)());


/// @brief
///   Create a set of patomic_ops*_xchg_t objects with all combinations of
///   members set to null and non-null values.
template <ops_domain D>
std::vector<ops_any_all<typename ops_types<D>::xchg_t>>
make_ops_xchg_combinations();


/// @brief
///   Create an array of members in a patomic_ops*_xchg_t object, with the
///   types cast to void(*)().
template <ops_domain D>
std::array<void(*)(), 3>
make_ops_xchg_array(const typename ops_types<D>::xchg_t& xchg) noexcept;


/// @brief
///   Create a set of patomic_ops*_bitwise_t objects with all combinations of
///   members set to a provided value. All other members are null.
template <ops_domain D>
std::vector<ops_any_all<typename ops_types<D>::bitwise_t>>
make_ops_bitwise_combinations(void(*nonnull_value)());


/// @brief
///   Create a set of patomic_ops*_bitwise_t objects with all combinations of
///   members set to null and non-null values.
template <ops_domain D>
std::vector<ops_any_all<typename ops_types<D>::bitwise_t>>
make_ops_bitwise_combinations();


/// @brief
///   Create an array of members in a patomic_ops*_bitwise_t object, with the
///   types cast to void(*)().
template <ops_domain D>
std::array<void(*)(), 4>
make_ops_bitwise_array(const typename ops_types<D>::bitwise_t& bitwise) noexcept;


/// @brief
///   Create a set of patomic_ops*_binary_t objects with all combinations of
///   void and fetch members set to a provided value. All other members are
///   null.
template <ops_domain D>
std::vector<ops_any_all_vf<typename ops_types<D>::binary_t>>
make_ops_binary_combinations(void(*nonnull_value)());


/// @brief
///   Create a set of patomic_ops*_binary_t objects with all combinations of
///   void and fetch members set to null and non-null values.
template <ops_domain D>
std::vector<ops_any_all_vf<typename ops_types<D>::binary_t>>
make_ops_binary_combinations();


/// @brief
///   Create an array of members in a patomic_ops*_binary_t object, with the
///   types cast to void(*)().
template <ops_domain D>
std::array<void(*)(), 8>
make_ops_binary_array(const typename ops_types<D>::binary_t& binary) noexcept;


/// @brief
///   Create a set of patomic_ops*_arithmetic_t objects with all combinations of
///   void and fetch members set to a provided value. All other members are
///   null.
template <ops_domain D>
std::vector<ops_any_all_vf<typename ops_types<D>::arithmetic_t>>
make_ops_arithmetic_combinations(void(*nonnull_value)());


/// @brief
///   Create a set of patomic_ops*_arithmetic_t objects with all combinations of
///   void and fetch members set to null and non-null values.
template <ops_domain D>
std::vector<ops_any_all_vf<typename ops_types<D>::arithmetic_t>>
make_ops_arithmetic_combinations();


/// @brief
///   Create an array of members in a patomic_ops*_arithmetic_t object, with the
///   types cast to void(*)().
template <ops_domain D>
std::array<void(*)(), 10>
make_ops_arithmetic_array(const typename ops_types<D>::arithmetic_t& arithmetic) noexcept;


/// @brief
///   Create a set of patomic_ops_transaction_special_t objects with all
///   combinations of members set to a provided value. All other members are
///   null.
std::vector<ops_any_all<patomic_ops_transaction_special_t>>
make_ops_special_combinations_transaction(void(*nonnull_value)());


/// @brief
///   Create a set of patomic_ops_transaction_special_t objects with all
///   combinations of members set to null and non-null values.
std::vector<ops_any_all<patomic_ops_transaction_special_t>>
make_ops_special_combinations_transaction();


/// @brief
///   Create an array of members in a patomic_ops_transaction_special_t object,
///   with the types cast to void(*)().
std::array<void(*)(), 4>
make_ops_special_array_transaction(const patomic_ops_transaction_special_t& special) noexcept;


/// @brief
///   Create a set of patomic_ops_transaction_flag_t objects with all
///   combinations of members set to a provided value. All other members are
///   null.
std::vector<ops_any_all<patomic_ops_transaction_flag_t>>
make_ops_flag_combinations_transaction(void(*nonnull_value)());


/// @brief
///   Create a set of patomic_ops_transaction_flag_t objects with all
///   combinations of members set to null and non-null values.
std::vector<ops_any_all<patomic_ops_transaction_flag_t>>
make_ops_flag_combinations_transaction();


/// @brief
///   Create an array of members in a patomic_ops_transaction_flag_t object,
///   with the types cast to void(*)().
std::array<void(*)(), 3>
make_ops_flag_array_transaction(const patomic_ops_transaction_flag_t& flag) noexcept;


/// @brief
///   Create a set of patomic_ops_transaction_raw_t objects with all
///   combinations of members set to a provided value. All other members are
///   null.
std::vector<ops_any_all<patomic_ops_transaction_raw_t>>
make_ops_raw_combinations_transaction(void(*nonnull_value)());


/// @brief
///   Create a set of patomic_ops_transaction_raw_t objects with all
///   combinations of members set to null and non-null values.
std::vector<ops_any_all<patomic_ops_transaction_raw_t>>
make_ops_raw_combinations_transaction();


/// @brief
///   Create an array of members in a patomic_ops_transaction_raw_t object,
///   with the types cast to void(*)().
std::array<void(*)(), 4>
make_ops_raw_array_transaction(const patomic_ops_transaction_raw_t& raw) noexcept;


}  // namespace test

#endif  // PATOMIC_TEST_PATOMIC_MAKE_OPS_HPP
