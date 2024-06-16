#ifndef PATOMIC_TEST_PATOMIC_MAKE_OPS_HPP
#define PATOMIC_TEST_PATOMIC_MAKE_OPS_HPP

#include <patomic/types/ops.h>


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


}  // namespace test

#endif  // PATOMIC_TEST_PATOMIC_MAKE_OPS_HPP
