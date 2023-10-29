#include <gtest/gtest.h>


extern "C" {

    /// @brief Called by asan when an error is detected.
    /// @note  Asan may still abort after calling this function.
    void
    __asan_on_error()
    {
        GTEST_FAIL() << "Encountered an address sanitizer (asan) error";
    }

    /// @brief Called by tsan when an error is detected.
    /// @note  Tsan may still abort after calling this function.
    void
    __tsan_on_report()
    {
        GTEST_FAIL() << "Encountered a thread sanitizer (tsan) error";
    }

    /// @brief Called by ubsan when an error is detected .
    /// @note  Ubsan may still abort after calling this function.
    void
    __ubsan_on_report()
    {
        GTEST_FAIL() << "Encountered an undefined behaviour sanitizer (ubsan) error";
    }

}  // extern "C"
