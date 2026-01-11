# Implementation Tasks: Command Line Parser Library

**Branch**: `001-cmdline-parser` | **Date**: 2026-01-11  
**Related**: [spec.md](spec.md), [plan.md](plan.md)

## Overview
Implementation tasks for the command line parser library following the design specified in contracts and research findings.

## Task Categories

### 1. Project Setup
- [ ] Create library directory structure: `library/cmdline_parser/`
- [ ] Create CMakeLists.txt for the library
- [ ] Add Catch2 dependency via vcpkg
- [ ] Update root CMakeLists.txt to include subdirectory
- [ ] Configure build options (header-only vs compiled)

### 2. Core Parser Implementation (Phase 1)
- [ ] Implement `ParseError` class with error types and messages
- [ ] Implement `ParseResult` class with value storage and error handling
- [ ] Implement basic POSIX-style argument parsing state machine
- [ ] Implement option classification (short, long, positional, end-of-options)
- [ ] Implement `-abc` → `-a -b -c` expansion for boolean flags
- [ ] Implement `--` end-of-options delimiter support
- [ ] Implement basic type conversion (string → bool/int/double)
- [ ] Create unit tests for core parser functionality

### 3. Builder API & Configuration (Phase 2)
- [ ] Implement `App` class with PImpl pattern
- [ ] Implement `OptionBuilder` fluent API
- [ ] Implement option configuration storage
- [ ] Implement duplicate option handling policies
- [ ] Implement required/optional option validation
- [ ] Implement default value support
- [ ] Implement help text generation
- [ ] Implement version flag support
- [ ] Create unit tests for builder API

### 4. Validation System (Phase 3)
- [ ] Implement `Validator` base class and hierarchy
- [ ] Implement `RangeValidator` for numeric ranges
- [ ] Implement `PatternValidator` for regex validation
- [ ] Implement `ChoiceValidator` for predefined values
- [ ] Implement `CustomValidator` for user-defined functions
- [ ] Integrate validation with option parsing
- [ ] Create validation error messages
- [ ] Create unit tests for validation system

### 5. Advanced Features (Phase 3)
- [ ] Implement subcommand support (nested `App` instances)
- [ ] Implement environment variable fallback
- [ ] Implement callback support for option parsing
- [ ] Implement option grouping for help organization
- [ ] Implement deprecated option support with suggestions
- [ ] Implement positional argument handling
- [ ] Implement Windows-style option parsing (`/help`)
- [ ] Create unit tests for advanced features

### 6. Integration Features (Phase 4)
- [ ] Implement configuration file parsing (INI/JSON/YAML)
- [ ] Implement automatic help generation with formatting
- [ ] Implement color output support (integrate with colorfmt library)
- [ ] Implement cross-platform path handling
- [ ] Create integration tests with real-world examples

### 7. Testing & Quality
- [ ] Set up Catch2 test framework
- [ ] Create comprehensive unit test suite
- [ ] Create integration tests with example programs
- [ ] Create benchmark tests for performance validation
- [ ] Set up code coverage reporting
- [ ] Run static analysis (clang-tidy) on codebase
- [ ] Ensure zero compiler warnings

### 8. Documentation
- [ ] Create API documentation (Doxygen)
- [ ] Create usage examples in `examples/` directory
- [ ] Update library README with quickstart guide
- [ ] Create migration guide from CLI11/gflags
- [ ] Document error handling patterns
- [ ] Create troubleshooting guide

### 9. Build & Distribution
- [ ] Configure header-only mode build option
- [ ] Configure compiled library mode build option
- [ ] Create installation targets (CMake install)
- [ ] Create package configuration for find_package()
- [ ] Test build on multiple platforms (Linux, macOS, Windows)
- [ ] Verify compatibility with different compilers (gcc, clang, MSVC)

### 10. Integration with CArsenal
- [ ] Update main program to use new parser (optional)
- [ ] Ensure compatibility with existing libraries (colorfmt, etc.)
- [ ] Update AGENTS.md with new library patterns
- [ ] Add library to project documentation

## Priority Order
1. **P1** (Must have): Core parser, basic option handling, error reporting
2. **P2** (Should have): Builder API, validation, help generation
3. **P3** (Nice to have): Subcommands, advanced features, integration

## Dependencies
- **C++17**: Minimum language standard
- **Catch2**: Testing framework (via vcpkg)
- **Standard library only**: No external dependencies for core functionality
- **Optional**: Boost.Regex for advanced pattern validation (fallback to std::regex)

## Success Criteria
- [ ] Library compiles with zero warnings
- [ ] All unit tests pass
- [ ] Integration tests with example programs work
- [ ] Performance: <1ms parsing time for typical command lines
- [ ] Memory: No memory leaks (valgrind clean)
- [ ] API: Follows modern C++ best practices
- [ ] Documentation: Complete and understandable
- [ ] Cross-platform: Works on Linux, macOS, Windows

## Risk Mitigation
- **Risk**: Overly complex API design
  - **Mitigation**: Follow established patterns (CLI11, cxxopts), keep API simple
- **Risk**: Performance issues with complex parsing
  - **Mitigation**: Profile early, optimize hot paths, use efficient data structures
- **Risk**: Incompatibility with existing code
  - **Mitigation**: Provide migration guide, maintain similar API to CLI11 where possible
- **Risk**: Missing edge cases in parsing
  - **Mitigation**: Comprehensive test suite covering POSIX/GNU/Windows conventions

## Next Steps
1. Begin with **Project Setup** tasks
2. Implement **Core Parser** with basic tests
3. Add **Builder API** for configuration
4. Expand with **Validation** and **Advanced Features**
5. Complete **Testing** and **Documentation**

## Notes
- Existing CLI11 and gflags libraries in `/third/cmdline_parser/` can be referenced for patterns
- Follow existing project patterns (see `library/colorfmt/` for CMake structure)
- Use namespace `cars::cmdline_parser` for consistency
- Prioritize clarity and usability over feature completeness