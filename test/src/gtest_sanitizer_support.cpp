#include <gtest/gtest.h>

extern "C" {
    [[maybe_unused]] void
    __asan_on_error()
    {
        GTEST_FAIL() << "Encountered an address sanitizer error";
    }

    [[maybe_unused]] inline void
    __tsan_on_report()
    {
        GTEST_FAIL() << "Encountered a thread sanitizer error";
    }

    [[maybe_unused]] inline void
    __ubsan_on_report()
    {
        GTEST_FAIL() << "Encountered an undefined behaviour sanitizer error";
    }
}
