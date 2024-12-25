// Copyright (c) doodspav.
// SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception

#include <test/common/death.hpp>

#include <csignal>
#include <cstdlib>


namespace test
{


KilledByAbortPredicateType
KilledByAbort()
{
#if PATOMIC_GTEST_HAS_KILLED_BY_SIGNAL
    return testing::KilledBySignal(SIGABRT);
#elif defined(GTEST_OS_WINDOWS)
    #if defined(_UCRT)
        const static unsigned int old_flags = _set_abort_behavior(0u, 0u);
        if (old_flags & _CALL_REPORTFAULT)
        {
            constexpr int status_stack_buffer_overflow = -1073740791;
            return testing::ExitedWithCode(status_stack_buffer_overflow);
        }
        else
        {
    #endif
            constexpr int abort_exit_code = 3;
            return testing::ExitedWithCode(abort_exit_code);
    #if defined(_UCRT)
        }
    #endif
#else
    return testing::ExitedWithCode(3);
#endif
}


}  // namespace test
