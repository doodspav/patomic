// Copyright (c) doodspav.
// SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception

#ifndef PATOMIC_TEST_COMMON_GENERIC_INT_HPP
#define PATOMIC_TEST_COMMON_GENERIC_INT_HPP

#include <cstddef>
#include <ostream>
#include <vector>

namespace test
{


/// @brief
///   Generic integer type which can assume any width, alignment, and
///   signedness.
/// @note
///   Internal integer representation is two's complement.
class generic_integer
{
public:
    /// @brief
    ///   Create a generic integer of a given width, alignment, and signedness.
    /// @pre
    ///   The width cannot be 0.
    /// @pre
    ///   The alignment must be a power of 2.
    /// @post
    ///   Initial value will be 0.
    generic_integer(std::size_t width, std::size_t alignment, bool is_signed);

    /// @brief
    ///   Check if the integer is signed.
    bool
    is_signed() const noexcept;

    /// @brief
    ///   Get the byte width of the integer.
    std::size_t
    width() const noexcept;

    /// @brief
    ///   Get the bit width of the integer.
    std::size_t
    bit_width() const noexcept;

    /// @brief
    ///   Get the alignment of the integer.
    std::size_t
    alignment() const noexcept;

    /// @brief
    ///   Get a pointer to the start of the internal buffer.
    unsigned char *
    data() noexcept;

    /// @brief
    ///   Get a pointer to the start of the internal buffer.
    const unsigned char *
    data() const noexcept;

    /// @brief
    ///   Swap the values of two integers.
    /// @pre
    ///   other.width() == .width()         &&
    ///   other.alignment() == .alignment() &&
    ///   other.is_signed() == .is_signed()
    void
    swap(generic_integer& other) noexcept;

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
    ///   Flip all bits in this instance's value in-place.
    void
    inv() noexcept;

    /// @brief
    ///   Flip a single bit at a given offset in this instance's value in-place.
    /// @pre
    ///   offset == 0 || offset < (.width() * CHAR_BIT)
    void
    inv_at(unsigned long long offset) noexcept;

    /// @brief
    ///   Directly store a raw buffer as the integer value.
    /// @pre
    ///   size == .width()
    void
    store(const unsigned char *buf, std::size_t size) noexcept;

    /// @brief
    ///   Set value to zero.
    void
    store_zero() noexcept;

    /// @brief
    ///   Set this instance's value to the minimum it can represent.
    void
    store_min() noexcept;

    /// @brief
    ///   Set this instance's value to the maximum it can represent.
    void
    store_max() noexcept;

    /// @brief
    ///   Implicitly convertible to void *.
    operator void *() noexcept;

    /// @brief
    ///   Implicitly convertible to const void *.
    operator const void *() const noexcept;

    /// @brief
    ///   Stream to output.
    friend std::ostream&
    operator<<(std::ostream& os, const generic_integer& gi);

    /// @brief
    ///   Equality comparison.
    /// @pre
    ///   other.width() == width()
    friend bool
    operator==(const generic_integer& lhs, const generic_integer& rhs) noexcept;

    /// @brief
    ///   Inequality comparison.
    /// @pre
    ///   other.width() == width()
    friend bool
    operator!=(const generic_integer& lhs, const generic_integer& rhs) noexcept;

private:
    /// @brief Width of integer.
    std::size_t m_width {};

    /// @brief Alignment of integer.
    std::size_t m_alignment {};

    /// @brief If the underlying integer is signed or not.
    bool m_is_signed {};

    /// @brief Index to first byte of integer representation in buffer.
    std::ptrdiff_t m_offset {};

    /// @brief Buffer holding aligned internal representation.
    std::vector<unsigned char> m_buf;
};


}  // namespace test

#endif  // PATOMIC_TEST_COMMON_GENERIC_INT_HPP
