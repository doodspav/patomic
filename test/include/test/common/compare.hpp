#ifndef PATOMIC_TEST_COMMON_COMPARE_HPP
#define PATOMIC_TEST_COMMON_COMPARE_HPP


#include <patomic/api/align.h>


bool
operator==(const patomic_align_t& lhs, const patomic_align_t& rhs) noexcept;


bool
operator!=(const patomic_align_t& lhs, const patomic_align_t& rhs) noexcept;


#endif  // PATOMIC_TEST_COMMON_COMPARE_HPP
