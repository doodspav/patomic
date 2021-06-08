#ifndef PATOMIC_TEST_ALIGNED_VINT_HPP
#define PATOMIC_TEST_ALIGNED_VINT_HPP

#include <cassert>
#include <cstddef>
#include <cstring>
#include <limits>
#include <memory>
#include <type_traits>

#include <patomic_test/aligned_buffer.hpp>


namespace patomic {
    namespace test {

        class aligned_vint
        {
        private:
            aligned_buffer m_buf;

        public:
            aligned_vint(std::size_t width, std::size_t align)
                : m_buf(width, align)
            {}
            virtual ~aligned_vint() = default;

            std::size_t width() const noexcept { return m_buf.size; }
            unsigned char *data() noexcept { return m_buf.data; }
            const unsigned char *data() const noexcept { return m_buf.data; }

            void store(unsigned char *buf, std::size_t len) noexcept
            {
                assert(width() == len);
                std::memcpy(data(), buf, len);
            }

            virtual void add(const aligned_vint&) noexcept = 0;
            virtual void sub(const aligned_vint&) noexcept = 0;
            virtual void inc() noexcept = 0;
            virtual void dec() noexcept = 0;
            virtual void neg() noexcept = 0;

            virtual void min() noexcept = 0;
            virtual void max() noexcept = 0;
        };

        template <typename T>
        class aligned_vint_tmp
            : public aligned_vint
        {
        private:
            using self_t = aligned_vint_tmp<T>;
            using uint_t = typename std::make_unsigned<T>::type;
            using sint_t = typename std::make_signed<T>::type;
            T* m_val{};

        public:
            aligned_vint_tmp()
                : aligned_vint(sizeof(T), alignof(T))
            {
                m_val = new (this->data()) T{};
            }
            ~aligned_vint_tmp() override
            {
                m_val->~T();
            }
            aligned_vint_tmp(const aligned_vint_tmp&) = delete;

            void add(const aligned_vint& base_other) noexcept override
            {
                const auto& other = dynamic_cast<const self_t&>(base_other);
                auto u_val = static_cast<uint_t>(*m_val);
                auto u_other = static_cast<uint_t>(*(other.m_val));
                *m_val = static_cast<T>(u_val + u_other);
            }
            void sub(const aligned_vint& base_other) noexcept override
            {
                const auto& other = dynamic_cast<const self_t&>(base_other);
                auto u_val = static_cast<uint_t>(*m_val);
                auto u_other = static_cast<uint_t>(*(other.m_val));
                *m_val = static_cast<T>(u_val - u_other);
            }
            void inc() noexcept override
            {
                auto u_val = static_cast<uint_t>(*m_val);
                *m_val = static_cast<T>(++u_val);
            }
            void dec() noexcept override
            {
                auto u_val = static_cast<uint_t>(*m_val);
                *m_val = static_cast<T>(--u_val);
            }
            void neg() noexcept override
            {
                // msvc complains about unsigned negation (C4146)
                auto val = static_cast<sint_t>(*m_val);
                // C++20 guarantees 2s complement
                // -INT_MIN == INT_MIN
                constexpr auto min = std::numeric_limits<sint_t>::min();
                if (val != min) { *m_val = static_cast<T>(-val); }
            }

            void min() noexcept override
            {
                *m_val = std::numeric_limits<T>::min();
            }
            void max() noexcept override
            {
                *m_val = std::numeric_limits<T>::max();
            }
        };

        using avt_sc_t = aligned_vint_tmp<signed char>;
        using avt_ss_t = aligned_vint_tmp<signed short>;
        using avt_si_t = aligned_vint_tmp<signed int>;
        using avt_sl_t = aligned_vint_tmp<signed long>;
        using avt_sll_t = aligned_vint_tmp<signed long long>;

        using avt_uc_t = aligned_vint_tmp<unsigned char>;
        using avt_us_t = aligned_vint_tmp<unsigned short>;
        using avt_ui_t = aligned_vint_tmp<unsigned int>;
        using avt_ul_t = aligned_vint_tmp<unsigned long>;
        using avt_ull_t = aligned_vint_tmp<unsigned long long>;

        static std::unique_ptr<aligned_vint>
        create_vint(std::size_t width, bool is_signed)
        {
            if (is_signed)
            {
                if (width == sizeof(char)) { return std::make_unique<avt_sc_t>(); }
                else if (width == sizeof(short)) { return std::make_unique<avt_ss_t>(); }
                else if (width == sizeof(int)) { return std::make_unique<avt_si_t>(); }
                else if (width == sizeof(long)) { return std::make_unique<avt_sl_t>(); }
                else if (width == sizeof(long long)) { return std::make_unique<avt_sll_t>(); }
            }
            else
            {
                if (width == sizeof(char)) { return std::make_unique<avt_uc_t>(); }
                else if (width == sizeof(short)) { return std::make_unique<avt_us_t>(); }
                else if (width == sizeof(int)) { return std::make_unique<avt_ui_t>(); }
                else if (width == sizeof(long)) { return std::make_unique<avt_ul_t>(); }
                else if (width == sizeof(long long)) { return std::make_unique<avt_ull_t>(); }
            }
            return nullptr;
        }

}}  // ns patomic::test

#endif  // !PATOMIC_TEST_ALIGNED_VINT_HPP
