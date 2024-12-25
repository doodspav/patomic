// Copyright (c) doodspav.
// SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception

extern "C" {

    /// @brief Called by asan to set default options.
    const char *
    __asan_default_options()
    {
        return "halt_on_error=0";
    }

    /// @brief Called by tsan to set default options.
    const char *
    __tsan_default_options()
    {
        return "";
    }

    /// @brief Called by ubsan to set default options.
    const char *
    __ubsan_default_options()
    {
        return "";
    }

}  // extern "C"
