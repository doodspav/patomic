#include <test/common/compare.hpp>


bool
operator==(const patomic_align_t& lhs, const patomic_align_t& rhs) noexcept
{
    return lhs.recommended == rhs.recommended &&
           lhs.minimum     == rhs.minimum     &&
           lhs.size_within == rhs.size_within;
}


bool
operator!=(const patomic_align_t& lhs, const patomic_align_t& rhs) noexcept
{
    return !(lhs == rhs);
}
