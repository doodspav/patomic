# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

- None

## [1.1.0] - 2024-04-01

### Added

- Add implementation to support implicit and explicit atomic operations using
  the `_Interlocked` API
- Implementation has id `patomic_id_MSVC`, kind `patomic_kind_ASM`, and
  supports up to `128` bit operations

## [1.0.0] - 2024-12-25

### Added

- Initial stable API and ABI
- Initial implementation supporting only atomic operations using `<stdatomic.h>` 
(no transaction operations are supported)
- Complete binary testing except for thread-safety tests
