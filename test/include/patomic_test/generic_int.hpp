#ifndef PATOMIC_TEST_GENERIC_INT_HPP
#define PATOMIC_TEST_GENERIC_INT_HPP

#include <cassert>
#include <cstring>
#include <limits>
#include <vector>

namespace patomic {
    namespace test {

        class generic_int
        {
        private:
            std::vector<unsigned char> m_buf;
            bool m_is_signed;

            static bool is_little_endian() noexcept
            {
                static_assert(sizeof(int) > 1, "int is a single byte");
                unsigned int x = 1u;
                return *(reinterpret_cast<unsigned char *>(&x));
            }
            static bool is_big_endian() noexcept
            {
                return !is_little_endian();
            }

        public:
            generic_int(std::size_t width, bool is_signed)
                : m_buf(width), m_is_signed(is_signed)
            {
                assert(width > 0);
            }

            bool is_signed() const noexcept
            {
                return m_is_signed;
            }
            std::size_t width() const noexcept
            {
                return m_buf.size();
            }
            unsigned char *data() noexcept
            {
                return &m_buf[0];
            }
            const unsigned char *data() const noexcept
            {
                return &m_buf[0];
            }

            void store(unsigned char *buf, std::size_t len) noexcept
            {
                assert(width() == len);
                std::memcpy(data(), buf, len);
            }

            void add(const generic_int& other) noexcept
            {
                /* setup */
                assert(this->width() == other.width());
                const auto uc_max = std::numeric_limits<unsigned char>::max();
                unsigned int carry = 0;
                static_assert(sizeof(int) > sizeof(char), "sizeof(int) == sizeof(char)");
                /* perform addition */
                for (std::size_t i = 0; i < this->width(); ++i)
                {
                    std::size_t idx;
                    if (is_little_endian()) { idx = i; }
                    else { idx = this->width() - i - 1; }
                    carry += this->data()[idx];
                    carry += other.data()[idx];
                    this->data()[idx] = static_cast<unsigned char>(carry & uc_max);
                    carry >>= CHAR_BIT;
                }
            }
            void sub(const generic_int& other) noexcept
            {
                auto arg = other;
                arg.neg();
                this->add(arg);
            }
            void inc() noexcept
            {
                auto one = generic_int(this->width(), this->is_signed());
                if (is_little_endian()) { one.data()[0] = 1; }
                else { one.data()[one.width() - 1] = 1; }
                this->add(one);
            }
            void dec() noexcept
            {
                auto one = generic_int(this->width(), this->is_signed());
                if (is_little_endian()) { one.data()[0] = 1; }
                else { one.data()[one.width() - 1] = 1; }
                this->sub(one);
            }
            void neg() noexcept
            {
                for (std::size_t i = 0; i < this->width(); ++i)
                {
                    this->data()[i] = static_cast<unsigned char>(~(this->data()[i]));
                }
                this->inc();
            }

            void min() noexcept
            {
                std::memset(this->data(), 0, this->width());
                if (this->is_signed())
                {
                    const auto sc_min = std::numeric_limits<signed char>::min();
                    const auto min = static_cast<unsigned char>(sc_min);
                    if (is_big_endian()) { this->data()[0] = min; }
                    else { this->data()[this->width() - 1] = min; }
                }
            }
            void max() noexcept
            {
                const auto uc_max = std::numeric_limits<unsigned char>::max();
                std::memset(this->data(), uc_max, this->width());
                if (this->is_signed())
                {
                    const auto sc_max = std::numeric_limits<signed char>::max();
                    const auto max = static_cast<unsigned char>(sc_max);
                    if (is_big_endian()) { this->data()[0] = max; }
                    else { this->data()[this->width() - 1] = max; }
                }
            }
        };

    }
}

#endif  /* !PATOMIC_TEST_GENERIC_INT_HPP */
