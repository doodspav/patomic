# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/).
This project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html)
from `1.0.0` onwards. Releases before that with a version matching `0.y.z` may
increment the `minor` version for breaking changes.

## [Unreleased] [Patch]
### Added
- following config macros for internal use:
  - `PATOMIC_HAVE_GNU_SYNC`
  - `PATOMIC_HAVE_GNU_ATOMIC`
  - `PATOMIC_HAVE_GNU_SYNC_LOCK_FREE_{N}` for `N` in `[1, 2, 4, 8, 16]`
  - `PATOMIC_HAVE_LONG_LONG_EXTN`
  - `PATOMIC_HAVE_MS_INT128`
  - `PATOMIC_HAVE_MS_INT128_EXTN`
- following macros and corresponding config macros for internal use:
  - `PATOMIC_LIKELY`
  - `PATOMIC_UNLIKELY`
- alignment helpers in `stdalign.h`
### Changed
- removed `do_assert`, `do_assert_aligned`, and `do_memcpy` parameters from
  wrapped macros; now use overrideable default macros from `wrapped/do.h`

## [0.3.1] [Patch] - 2021-12-16
### Added
- `wrapped` macro implementations of all ops:
  - `fetch.h` wraps `fetch_op` to define `op`
  - `direct.h` wraps an atomic operation to define that same operation
  - `cmpxchg.h` wraps `cmpxchg_weak_explicit` to define any other operation
### Changed
- renamed `types/intptr.h` to `stdlib/stdint.h`
- `std` implementation now uses `wrapped` macros instead of defining whole
  functions directly
### Fixed
- `std` implementation now also uses `ATOMIC_LLONG_IS_LOCK_FREE` in the create
  functions to make sure the corresponding functions are defined
- `std` implementation no longer assumes `sizeof(T)` will return a power of 2

## [0.3.0] [Minor] - 2021-12-11
### Added
- `patomic_assert` and `patomic_assert_{always|unreachable}` for internal use
- `patomic_` versions of `string.h` functions for internal use
- `patomic_assert(_unreachable)` supports `NDEBUG`
- macro `NNDEBUG` can be defined to override `NDEBUG` (for patomic variants)
- following macros and corresponding config macros for internal use:
  - `PATOMIC_NOINLINE` (MS/GNU)
  - `PATOMIC_NORETURN` (MS/GNU/STD)
  - `PATOMIC_RESTRICT` (MS/GNU/STD)
  - `PATOMIC_FUNC_NAME` (MS/GNU/STD)
  - `PATOMIC_UNREACHABLE()` (MS/GNU)
- `patomic_(u)intptr_t` type in `intptr.h` and corresponding config macros
- alignment check functions (`patomic_align_meets_{recommended|minimum}`)
- disabled warning `C4710` on msvc in presets (could not inline function)
- std impl ops assert that inputs are not `NULL` and are suitable aligned
### Changed
- replace all uses of `assert` with `patomic_assert*` variants
- std impl no longer unconditionally includes any non-freestanding headers
### Fixed
- mark all non-static functions and globals which aren't part of the public api
  with `PATOMIC_NO_EXPORT`
- std impl no longer requires the alignment of `_Atomic(T)` and `T` to match
- std impl assertions in non-fetch binary or arithmetic functions now display
  non-fetch function name in assertion errors (instead of fetch name)
### Removed
- fake stdatomic (`/src/include/patomic/fake/stdatomic.h`)

## [0.2.2] [Minor] - 2021-12-07
### Added
- transaction API (with `transaction.h`) including:
  - transaction helper types in new transaction.h including `flag_t`, `config_t`,
  `status_t`, and `result_t` (with appropriate name prefixes), as well as `wfb`
  (with fallback) variants where necessary
  - `patomic_transaction_abort_reason` helper function
  - transaction ops types in ops.h with additional ops `flag_ops`, `special_ops`,
  and `raw_ops`
  - `transaction_t` extras: `recommended` (recommended limits for transactions)
  and `sstring` (safe string operations that won't abort the transaction)
  - multiple checks for transaction implementations
- feature check API in `feature_check.h`
- `PATOMIC_MAX_CACHE_LINE_SIZE` macro in `align.h`
- corresponding `patomic_cache_line_size` function in `align.h`/`align.c`
- config macros to check for MS/GNU alignment extensions
### Changed
- `std` implementation now checks for MS/GNU alignment extensions before
  defaulting to using `sizeof` in place of `_Alignof`

## [0.2.1] [Patch] - 2021-11-27
### Added
- `HelperFunctionsTest` test suite (for version and memory order functions)
- `PATOMIC_GNU_INLINE_ALWAYS_INLINE_ATTR` config macro
### Changed
- `patomic_version_*` functions no longer marked `PATOMIC_FORCE_INLINE`
  (if inlining is needed, use the `PATOMIC_VERSION_*` macros provided)
- `PATOMIC_FORCE_INLINE` can now use `__inline__` instead of `inline` if
  available
### Fixed
- `patomic_version.h` declarations now wrapped in `extern "C" {}` in C++
  (caused linker errors when compiling with C++)

## [0.2.0] [Minor] - 2021-11-23
### Added
- exported version functions corresponding to macros
- function definitions are marked `PATOMIC_FORCE_INLINE`
### Changed
- split `memory_order` into `.h` and `.c`, so helper functions are no
  longer `static`
- function definitions are marked `PATOMIC_FORCE_INLINE`

## [0.1.2] [Patch] - 2021-11-15
### Changed
- moved all feature macros from `have_*_.h` into new `patomic_config.h`
- listed new macros in `README.md`
- updated include paths
### Added
- `patomic_config.h` (with default macro definitions)
- created `CompilerFeatureChecks.cmake` to check feature macros
- created `cmake/in` directory for `.in` files
- added `patomic_config.h.in` to generate `_patomic_config.h`
- `_patomic_config.h` is then included in `patomic_config.h`
### Removed
- all `have_*_.h` files from `src/include/patomic/macros`

## [0.1.1] [Patch] - 2021-11-14
### Added
- Enable IPO (if supported) when config is not Debug
- Link `libatomic.so` if it exists
### Fixed
- gcc build example now works correctly
### Changed
- Changed `.cmake` file case from `kebab-case` to `PascalCase`

## [0.1.0] [Major] - 2021-10-28
### Added
- Complete `std` implementation
- Mostly complete `msvc` implementation (missing availability checks for old 
  compiler versions)
- Logic testing (does not check for correctness in multi-threaded environments)
- Workflows for GitHub actions (`on-push.yml` and `on-release.yml`)
- CMake files and presets to portably build and install project
- Information files (`README.md`, `ARCHITECTURE.md`, `CHANGELOG.md`)
