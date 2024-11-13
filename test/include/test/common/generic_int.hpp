#ifndef PATOMIC_TEST_COMMON_GENERIC_INT_HPP
#define PATOMIC_TEST_COMMON_GENERIC_INT_HPP

#include <cstddef>
#include <vector>

namespace test
{


/// @brief
///   Generic integer type which can assume any width and signedness.
/// @note
///   Internal integer representation is two's complement.
class generic_integer
{
public:
    /// @brief
    ///   Typed boolean representing if an integer is signed or unsigned.
    enum class sign_flag
    {
        UNSIGNED = 0,
        SIGNED = 1
    };

    /// @brief
    ///   Create a generic integer of a given width and signedness.
    /// @pre
    ///   The width cannot be 0.
    /// @post
    ///   Initial value will be 0.
    generic_integer(std::size_t width, sign_flag is_signed);

    /// @brief
    ///   Check if the integer is signed.
    bool
    is_signed() const noexcept;

    /// @brief
    ///   Get the byte width of the integer.
    std::size_t
    width() const noexcept;

    /// @brief
    ///   Get a pointer to the start of the internal buffer.
    unsigned char *
    data() noexcept;

    /// @brief
    ///   Get a pointer to the start of the internal buffer.
    const unsigned char *
    data() const noexcept;

    /// @brief
    ///   Directly store a raw buffer as the integer value.
    /// @pre
    ///   size == .width()
    void
    store(const unsigned char *buf, std::size_t size) noexcept;

    /// @brief
    ///   Add another generic integer to this instance's value in-place.
    /// @pre
    ///   other.width() == .width()
    void
    add(const generic_integer& other) noexcept;

    /// @brief
    ///   Subtract another generic integer from this instance's value in-place.
    /// @pre
    ///   other.width() == .width()
    void
    sub(const generic_integer& other) noexcept;

    /// @brief
    ///   Increment this instance's value in-place.
    void
    inc() noexcept;

    /// @brief
    ///   Decrement this instance's value in-place.
    void
    dec() noexcept;


    /// @brief
    ///   Negate this instance's value in-place.
    void
    neg() noexcept;

    /// @brief
    ///   Set this instance's value to the minimum it can represent.
    void
    set_min() noexcept;

    /// @brief
    ///   Set this instance's value to the maximum it can represent.
    void
    set_max() noexcept;

private:
    /// @brief Flag holding signed state.
    sign_flag m_is_signed {};

    /// @brief Buffer holding internal representation.
    std::vector<unsigned char> m_buf;
};


}  // namespace test

#endif  // PATOMIC_TEST_COMMON_GENERIC_INT_HPP
