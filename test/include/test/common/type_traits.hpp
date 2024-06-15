#ifndef PATOMIC_TEST_COMMON_TYPE_TRAITS_HPP
#define PATOMIC_TEST_COMMON_TYPE_TRAITS_HPP

#include <utility>
#include <type_traits>

namespace test
{


/// @brief
///   Type is implicitly convertible to any other type.
template <class T>
class convertible_to_any
{
public:
    /// @brief
    ///    Public constructor to provide value which will be converted.
    explicit constexpr convertible_to_any(const T& value)
        noexcept(std::is_nothrow_copy_constructible<T>::value)
        : m_val(value)
    {}

    /// @brief
    ///    Public constructor to provide value which will be converted.
    explicit constexpr convertible_to_any(T&& value)
        noexcept(std::is_nothrow_move_constructible<T>::value)
        : m_val(std::move(value))
    {}

    /// @brief
    ///   Implicit conversion operator.
    template <class U>
    constexpr operator U() const noexcept
    {
        return reinterpret_cast<U>(m_val);
    }

private:
    T m_val {};
};


}

#endif  // PATOMIC_TEST_COMMON_TYPE_TRAITS_HPP
