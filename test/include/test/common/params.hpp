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
///   Combination of parameters for creating a patomic implicit or explicit
///   struct.
struct ParamsImpExp
{
    /// @brief Implementation id to request.
    patomic_id_t id {};

    /// @brief Byte width of atomic object to be supported.
    std::size_t width {};

    /// @brief Memory order used for all operations.
    patomic_memory_order_t order {};

    /// @brief Options to pass to constructor.
    unsigned int options {};

    /// @brief Type passed to tuple constructor.
    using TupleT = std::tuple<
        patomic_id_t, std::size_t, patomic_memory_order_t, unsigned int
    >;

    /// @brief Construct from tuple of members.
    explicit ParamsImpExp(TupleT tup) noexcept;

    /// @brief Default constructor.
    ParamsImpExp() noexcept = default;

    /// @brief Convert to string suitable for use as a test name suffix.
    std::string
    as_test_suffix() const;

    /// @brief Create all combinations of params for tests.
    static std::vector<ParamsImpExp>
    combinations();

    /// @brief Create all combinations of params for tests with only memory
    ///        orders which are valid for store operations.
    static std::vector<ParamsImpExp>
    combinations_store();

    /// @brief Create all combinations of params for tests with only memory
    ///        orders which are valid for load operations.
    static std::vector<ParamsImpExp>
    combinations_load();
};


/// @brief
///   Combination of parameters for creating a patomic transaction struct.
struct ParamsTsx
{
    /// @brief Implementation id to request.
    patomic_id_t id {};

    /// @brief Options to pass to constructor.
    unsigned int options {};

    /// @brief Widths to use for operation.
    std::vector<std::size_t> widths {};

    /// @brief Type passed to tuple constructor.
    using TupleT = std::tuple<
        patomic_id_t, unsigned int, std::vector<std::size_t>
    >;

    /// @brief Construct from tuple of members.
    explicit ParamsTsx(TupleT tup) noexcept;

    /// @brief Default constructor.
    ParamsTsx() noexcept = default;

    /// @brief Convert to string suitable for use as a test name suffix.
    std::string
    as_test_suffix() const;

    /// @brief Create all combinations of params for tests.
    static std::vector<ParamsTsx>
    combinations();
};


}  // namespace test

#endif  // PATOMIC_TEST_COMMON_PARAMS_HPP
