#ifndef PATOMIC_TEST_COMMON_MATH_HPP
#define PATOMIC_TEST_COMMON_MATH_HPP

#include <type_traits>

namespace test
{
    /// @brief Checks if a value is a positive power of 2.
    template <class T, std::enable_if_t<std::is_integral<T>::value, int> = 0>
    constexpr bool
    is_positive_pow2(T value) noexcept
    {
        // check value is positive
        if (value <= 0)
        {
            return false;
        }

        // check value is a power of 2
        return (value & (value - 1)) == 0;
    }

}  // namespace test

#endif  // PATOMIC_TEST_COMMON_MATH_HPP
