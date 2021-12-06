# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/).
This project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html)
from `1.0.0` onwards. Releases before that with a version matching `0.y.z` may
increment the `minor` version for breaking changes.

## [Unreleased] [Minor]
### Added
- transaction helper types in new transaction.h including `flag_t`, `config_t`,
  `status_t`, and `result_t` (with appropriate name prefixes), as well as `wfb`
  (with fallback) variants where necessary
- transaction ops types in ops.h with extension of `flag_ops`, `special_ops`,
  and `raw_ops`
- `PATOMIC_MAX_CACHE_LINE_SIZE` macro in `align.h`
- corresponding `patomic_max_cache_line_size` function in `align.h`/`align.c`
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