// Copyright (c) doodspav.
// SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception

#ifndef PATOMIC_TEST_COMMON_PARAMS_HPP
#define PATOMIC_TEST_COMMON_PARAMS_HPP

#include <patomic/api/ids.h>
#include <patomic/api/memory_order.h>

#include <cstddef>
#include <string>
#include <tuple>
#include <vector>

namespace test
{


/// @brief
///   Combination of parameters for creating a patomic implicit struct.
struct ParamsImplicit
{
    /// @brief
    ///   Implementation id to request.
    patomic_id_t id {};

    /// @brief
    ///   Byte width of atomic object to be supported.
    std::size_t width {};

    /// @brief
    ///   Memory order used for all operations.
    patomic_memory_order_t order {};

    /// @brief
    ///   Options to pass to constructor.
    unsigned int options {};

    /// @brief
    ///   Type passed to tuple constructor.
    using TupleT = std::tuple<
        patomic_id_t, std::size_t, patomic_memory_order_t, unsigned int
    >;

    /// @brief
    ///   Construct from tuple of members.
    explicit ParamsImplicit(TupleT tup) noexcept;

    /// @brief
    ///   Default constructor.
    ParamsImplicit() noexcept = default;

    /// @brief
    ///   Convert to string suitable for use as a test name suffix.
    std::string
    as_test_suffix() const;

    /// @brief
    ///  Create all combinations of params for tests.
    static std::vector<ParamsImplicit>
    combinations();

    /// @brief
    ///   Create all combinations of params for tests with given memory orders.
    static std::vector<ParamsImplicit>
    combinations_with(const std::vector<patomic_memory_order_t>& orders);

    /// @brief
    ///   Create all combinations of params for tests with only memory orders
    ///   which are valid for store operations.
    static std::vector<ParamsImplicit>
    combinations_store();

    /// @brief
    ///   Create all combinations of params for tests with only memory orders
    ///   which are valid for load operations.
    static std::vector<ParamsImplicit>
    combinations_load();
};


/// @brief
///   Combination of parameters for creating a patomic explicit struct.
struct ParamsExplicit
{
    /// @brief
    ///   Implementation id to request.
    patomic_id_t id {};

    /// @brief
    ///   Byte width of atomic object to be supported.
    std::size_t width {};

    /// @brief
    ///   Memory order used for all operations.
    patomic_memory_order_t order {};

    /// @brief
    ///   Options to pass to constructor.
    unsigned int options {};

    /// @brief
    ///   Type passed to tuple constructor.
    using TupleT = std::tuple<
        patomic_id_t, std::size_t, patomic_memory_order_t, unsigned int
    >;

    /// @brief
    ///   Construct from tuple of members.
    explicit ParamsExplicit(TupleT tup) noexcept;

    /// @brief
    ///   Default constructor.
    ParamsExplicit() noexcept = default;

    /// @brief
    ///   Convert to string suitable for use as a test name suffix.
    std::string
    as_test_suffix() const;

    /// @brief
    ///   Create all combinations of params for tests.
    static std::vector<ParamsExplicit>
    combinations();

    /// @brief
    ///   Create all combinations of params for tests with only memory orders
    ///   which are valid for store operations.
    static std::vector<ParamsExplicit>
    combinations_store();

    /// @brief
    ///   Create all combinations of params for tests with only memory orders
    ///   which are valid for load operations.
    static std::vector<ParamsExplicit>
    combinations_load();
};


/// @brief
///   Combination of parameters for creating a patomic transaction struct.
struct ParamsTransaction
{
    /// @brief
    ///   Implementation id to request.
    patomic_id_t id {};

    /// @brief
    ///   Options to pass to constructor.
    unsigned int options {};

    /// @brief
    ///   Type passed to tuple constructor.
    using TupleT = std::tuple<
        patomic_id_t, unsigned int
    >;

    /// @brief
    ///   Construct from tuple of members.
    explicit ParamsTransaction(TupleT tup) noexcept;

    /// @brief
    ///   Default constructor.
    ParamsTransaction() noexcept = default;

    /// @brief
    ///   Convert to string suitable for use as a test name suffix.
    std::string
    as_test_suffix() const;

    /// @brief
    ///   Create all combinations of params for tests.
    static std::vector<ParamsTransaction>
    combinations();
};


}  // namespace test

#endif  // PATOMIC_TEST_COMMON_PARAMS_HPP
