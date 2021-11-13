# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/).
This project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html)
from `1.0.0` onwards. Releases before that with a version matching `0.y.z` may
increment the `minor` version for breaking changes.

## [Unreleased] [Patch]
### Fixed
- gcc build example now works correctly

## [0.1.0] [Major] - 2021-10-28
### Added
- Complete `std` implementation
- Mostly complete `msvc` implementation (missing availability checks for old 
  compiler versions)
- Logic testing (does not check for correctness in multi-threaded environments)
- Workflows for GitHub actions (`on-push.yml` and `on-release.yml`)
- CMake files and presets to portably build and install project
- Information files (`README.md`, `ARCHITECTURE.md`, `CHANGELOG.md`)
