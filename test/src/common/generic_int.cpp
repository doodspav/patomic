#include <test/common/align.hpp>
#include <test/common/generic_int.hpp>

#include <algorithm>
#include <cassert>
#include <climits>
#include <iterator>
#include <limits>

namespace
{


/// @brief
///   Implementation of generic_integer requires that the platform uses
///   two's complement integer representation.
static_assert(
    -INT_MAX != INT_MIN,
    "integer representation must be two's complement"
);


/// @brief
///   Check if the runtime endianness is little-endian.
bool
is_little_endian() noexcept
{
    // ensure check is possible
    static_assert(
        sizeof(unsigned long long) > 1,
        "all integer types are 1 byte, cannot perform endianness check"
    );

    // check endianness
    unsigned int x = 1;
    return *reinterpret_cast<unsigned char *>(&x);
}


}  // namespace

namespace test
{


generic_integer::generic_integer(
    const std::size_t width,
    const std::size_t alignment,
    const bool is_signed
)
    : m_width(width),
      m_alignment(alignment),
      m_is_signed(is_signed)
{
    // check pre-conditions
    assert(width != 0);
    assert(alignment != 0);
    assert(alignment % 2u == 0);

    // reserve enough space for width and alignment padding
    m_buf.resize(width + alignment - 1u);

    // get offset to aligned representation
    void *ptr = aligned_pointer(m_buf.data(), m_buf.size(), alignment, width);
    m_offset = std::distance(m_buf.data(), static_cast<unsigned char *>(ptr));
}


bool
generic_integer::is_signed() const noexcept
{
    return m_is_signed;
}


std::size_t
generic_integer::width() const noexcept
{
    return m_width;
}


std::size_t
generic_integer::alignment() const noexcept
{
    return m_alignment;
}


unsigned char *
generic_integer::data() noexcept
{
    return std::next(m_buf.data(), m_offset);
}


const unsigned char *
generic_integer::data() const noexcept
{
    return std::next(m_buf.data(), m_offset);
}


void
generic_integer::store(
    const unsigned char *const buf, const std::size_t size
) noexcept
{
    // check pre-condition
    assert(size == width());

    // store value
    const auto ssize = static_cast<std::ptrdiff_t>(size);
    static_cast<void>(std::copy(buf, std::next(buf, ssize), data()));
}


void
generic_integer::add(const generic_integer& other) noexcept
{
    // check pre-condition
    assert(other.width() == width());

    // ensure operation is possible
    static_assert(
        sizeof(int) > sizeof(char),
        "int not big enough to hold carry for char without overflow"
    );

    // setup
    constexpr auto max_uc = std::numeric_limits<unsigned char>::max();
    unsigned int carry = 0;

    // perform operation
    for (std::size_t i = 0; i < width(); ++i)
    {
        // sum byte
        std::size_t index = is_little_endian() ? i : (width() - i - 1u);
        carry += data()[index];
        carry += other.data()[index];

        // write byte back
        data()[index] = static_cast<unsigned char>(carry & max_uc);
        carry >>= CHAR_BIT;
    }

    // carry can be discarded since we wraparound on overflow
    static_cast<void>(carry);
}


void
generic_integer::sub(const generic_integer& other) noexcept
{
    // negate other so we can defer to addition
    auto arg = other;
    arg.neg();

    // defer to addition
    add(arg);
}


void
generic_integer::inc() noexcept
{
    // create int with value of 1
    generic_integer one { width(), alignment(), is_signed() };
    const std::size_t lsbIndex = is_little_endian() ? 0u : (width() - 1u);
    one.data()[lsbIndex] = 1u;

    // defer to addition
    add(one);
}


void
generic_integer::dec() noexcept
{
    // create int with value of 1
    generic_integer one { width(), alignment(), is_signed() };
    const std::size_t lsbIndex = is_little_endian() ? 0u : (width() - 1u);
    one.data()[lsbIndex] = 1u;

    // defer to subtraction
    sub(one);
}


void
generic_integer::neg() noexcept
{
    // invert all bits and increment result
    inv();
    inc();
}


void
generic_integer::inv() noexcept
{
    // invert all bits
    for (std::size_t i = 0; i < width(); ++i)
    {
        data()[i] = static_cast<unsigned char>(~(data()[i]));
    }
}


void
generic_integer::set_min() noexcept
{
    // zero all bytes
    const auto ssize = static_cast<std::ptrdiff_t>(width());
    std::fill(data(), std::next(data(), ssize), 0);

    // special case for signed integers
    if (is_signed())
    {
        // set most significant bit to 1
        std::size_t msbIndex = is_little_endian() ? (width() - 1u) : 0u;
        unsigned char msb = 1u;
        msb <<= (std::numeric_limits<unsigned char>::digits - 1u);
        data()[msbIndex] = msb;
    }
}


void
generic_integer::set_max() noexcept
{
    // we can simply negate the value of min + 1 (two's complement)
    set_min();
    inc();
    neg();
}


bool
operator==(const generic_integer& lhs, const generic_integer& rhs) noexcept
{
    // check pre-condition
    assert(lhs.width() == rhs.width());

    // compare underlying buffer
    return lhs.m_buf == rhs.m_buf;
}


bool
operator!=(const generic_integer& lhs, const generic_integer& rhs) noexcept
{
    // check pre-condition
    assert(lhs.width() == rhs.width());

    // defer to equality comparison
    return !(lhs == rhs);
}


}  // namespace test
