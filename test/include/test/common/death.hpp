#ifndef PATOMIC_TEST_COMMON_DEATH_HPP
#define PATOMIC_TEST_COMMON_DEATH_HPP

#include <gtest/gtest.h>


namespace test
{


/// @brief
///   Helper type which can be passed by a matcher to any GTest macro which
///   passes an exit code.
#if defined(GTEST_OS_WINDOWS) || defined(GTEST_OS_FUCHSIA)
#define PATOMIC_GTEST_HAS_KILLED_BY_SIGNAL 0
using KilledByAbortPredicateType = testing::ExitedWithCode;
#else
#define PATOMIC_GTEST_HAS_KILLED_BY_SIGNAL 1
using KilledByAbortPredicateType = testing::KilledBySignal;
#endif


/// @brief
///   Produces a predicate that checks if an int exit code is what would be
///   expected from a process killed by abort() (which may ask the runtime to
///   kill it in a variety of ways).
///
/// @note
///   Predicate is callable with the signature bool()(int).
KilledByAbortPredicateType
KilledByAbort();


}  // namespace test

#endif  // PATOMIC_TEST_COMMON_DEATH_HPP
