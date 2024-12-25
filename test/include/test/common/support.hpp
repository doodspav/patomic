// Copyright (c) doodspav.
// SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception

#ifndef PATOMIC_TEST_COMMON_SUPPORT_HPP
#define PATOMIC_TEST_COMMON_SUPPORT_HPP

#include <patomic/api/ids.h>
#include <patomic/api/memory_order.h>
#include <patomic/api/options.h>

#include <cstddef>
#include <vector>
#include <string>
#include <tuple>

namespace test
{


/// @brief
///   Set of all widths which have support for at least one operation on any
///   platform.
std::vector<std::size_t>
supported_widths();


/// @brief
///   Set of all combinations of patomic_option_t labels (including NONE).
std::vector<unsigned int>
supported_options();


/// @brief
///   Set of all patomic_id_t labels (including NULL).
std::vector<patomic_id_t>
supported_ids();


/// @brief
///   Set of all valid memory orders.
std::vector<patomic_memory_order_t>
supported_orders();


/// @brief
///   Set of memory orders which can be used for store operations.
std::vector<patomic_memory_order_t>
supported_orders_store();


/// @brief
///   Set of memory orders which can be used for load operations.
std::vector<patomic_memory_order_t>
supported_orders_load();


}  // namespace test


#endif  // PATOMIC_TEST_COMMON_SUPPORT_HPP
