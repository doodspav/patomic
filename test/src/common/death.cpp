#include <test/common/death.hpp>

#include <csignal>

namespace test
{


KilledBySignalType
KilledByAbort()
{
#if PATOMIC_GTEST_HAS_KILLED_BY_SIGNAL
    return testing::KilledBySignal(SIGABRT);
#else
    return testing::ExitedWithCode(3);
#endif
}


}  // namespace test
