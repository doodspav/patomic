#ifndef PATOMIC_TEST_COMMON_NAME_HPP
#define PATOMIC_TEST_COMMON_NAME_HPP

#include <patomic/api/ids.h>
#include <patomic/api/memory_order.h>

#include <string>

namespace test
{


/// @brief
///   Convert an implementation id to a string.
std::string
name_id(patomic_id_t id);


/// @brief
///   Convert one or more combined options to a string.
std::string
name_options(unsigned int options);


/// @brief
///   Convert a memory order to a string.
std::string
name_order(patomic_memory_order_t order);


}  // namespace test

#endif  // PATOMIC_TEST_COMMON_NAME_HPP