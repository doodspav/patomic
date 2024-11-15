#ifndef PATOMIC_TEST_COMMON_SUPPORT_HPP
#define PATOMIC_TEST_COMMON_SUPPORT_HPP

#include <patomic/api/ids.h>
#include <patomic/api/memory_order.h>
#include <patomic/api/options.h>

#include <cstddef>
#include <set>
#include <tuple>

namespace test
{


/// @brief
///   Set of all widths which have support for at least one operation on any
///   platform.
std::set<std::size_t>
supported_widths();


/// @brief
///   Set of all combinations of patomic_option_t labels (including NONE).
std::set<unsigned int>
supported_options();


/// @brief
///   Set of memory orders which can be used for store operations.
std::set<patomic_memory_order_t>
supported_orders_store();


/// @brief
///   Set of memory orders which can be used for load operations.
std::set<patomic_memory_order_t>
supported_orders_load();


/// @brief
///   Set of all valid memory orders.
std::set<patomic_memory_order_t>
supported_orders();


/// @brief
///   Set of all patomic_id_t labels (including NULL).
std::set<patomic_id_t>
supported_ids();


/// @brief Combination of parameters for creating a patomic struct.
struct SupportCombination
{
    /// @brief Byte width of atomic object to be supported.
    std::size_t width {};

    /// @brief Memory order used for all operations.
    patomic_memory_order_t order {};

    /// @brief Implementation id to request.
    patomic_id_t id {};

    /// @brief Options to pass to constructor.
    unsigned int options {};

    /// @brief Type passed to tuple constructor.
    using TupleT = std::tuple<
        std::size_t, patomic_memory_order_t , patomic_id_t, unsigned int
    >;

    /// @brief Allow conversions from tuple.
    explicit SupportCombination(TupleT tup) noexcept;

    /// @brief Explicitly defined default constructor necessary.
    SupportCombination() noexcept = default;
};


}  // namespace test


#endif  // PATOMIC_TEST_COMMON_SUPPORT_HPP
