#ifndef PATOMIC_TEST_ALIGNED_BUFFER_HPP
#define PATOMIC_TEST_ALIGNED_BUFFER_HPP

#include <cassert>
#include <cstdlib>
#include <cstdint>
#include <new>

namespace patomic {
    namespace test {

        class aligned_buffer
        {
        private:
            void *base;

        public:
            unsigned char *data;
            std::size_t size;

            aligned_buffer() noexcept
                : base(nullptr), data(nullptr), size(0)
            {}
            aligned_buffer(std::size_t size, std::size_t align)
                : size(size)
            {
                assert(size != 0);
                assert(align != 0);
                assert((align & (align - 1)) == 0);
                base = ::operator new(size + align - 1);
                data = static_cast<unsigned char*>(base);
                while (reinterpret_cast<std::uintptr_t>(data) % align != 0) { ++data; }
            }
            aligned_buffer(aligned_buffer&& other) noexcept
            {
                base = other.base; other.base = nullptr;
                data = other.data; other.data = nullptr;
                size = other.size; other.size = 0;
            }
            ~aligned_buffer()
            {
                ::operator delete(base);
            }

            aligned_buffer& operator=(aligned_buffer&& other) noexcept
            {
                if (&other != this)
                {
                    ::operator delete(base);
                    base = other.base; other.base = nullptr;
                    data = other.data; other.data = nullptr;
                    size = other.size; other.size = 0;
                }
                return *this;
            }
        };

}}  // ns patomic::test

#endif  // !PATOMIC_TEST_ALIGNED_BUFFER_HPP
