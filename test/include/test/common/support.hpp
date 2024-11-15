#ifndef PATOMIC_TEST_COMMON_SUPPORT_HPP
#define PATOMIC_TEST_COMMON_SUPPORT_HPP

#include <patomic/api/ids.h>
#include <patomic/api/options.h>

#include <cstddef>
#include <set>

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
///   Set of all patomic_id_t labels (including NULL).
std::set<patomic_id_t>
supported_ids();


}  // namespace test


#endif  // PATOMIC_TEST_COMMON_SUPPORT_HPP
