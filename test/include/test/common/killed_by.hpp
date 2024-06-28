#ifndef PATOMIC_TEST_COMMON_KILLED_BY_HPP
#define PATOMIC_TEST_COMMON_KILLED_BY_HPP

#include <csignal>
#include <gtest/gtest.h>


namespace test
{


/// @brief
///   Produces a predicate that checks if an int exit code is what would be
///   expected from a process killed by SIGABRT (or OS equivalent).
///
/// @note
///   Predicate is callable with the signature bool()(int).
inline auto
KilledByAbort() noexcept
{
#if defined(GTEST_OS_WINDOWS) || defined (GTEST_OS_FUCHSIA)
    return testing:ExitedWithCode(3);
#else
    return testing::KilledBySignal(SIGABRT);
#endif
}


}  // namespace test

#endif  // PATOMIC_TEST_COMMON_KILLED_BY_HPP
