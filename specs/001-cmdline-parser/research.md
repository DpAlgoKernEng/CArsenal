# Research Findings: Command Line Parser Library

**Date**: 2026-01-11  
**Purpose**: Resolve technical unknowns and inform implementation decisions

## Research Tasks

### 1. Testing Framework Selection
**Status**: Resolved
**Decision**: Use Catch2 testing framework
**Rationale**: 
- No testing framework currently established in the CArsenal project (confirmed by codebase analysis)
- Catch2 is mentioned in project documentation as an example vcpkg dependency
- Catch2 is header-only, modern, and well-suited for C++ libraries
- Provides good support for both unit and integration testing
- Compatible with the project's vcpkg-based dependency management
**Alternatives considered**: Google Test (more complex setup), doctest (less mature), Boost.Test (heavyweight)

### 2. C++ Standard Version
**Status**: Resolved
**Decision**: C++17 minimum, with C++20 features considered if beneficial
**Rationale**: 
- CMakeLists.txt sets `CMAKE_CXX_STANDARD 17`
- C++17 provides modern features (std::optional, std::variant, string_view) useful for parsing
- C++20 could be considered for concepts and ranges but not required
**Alternatives considered**: C++11 (too limited), C++14 (missing useful features)

### 3. Library Architecture (Header-only vs Compiled)
**Status**: Research Needed
**Unknown**: Whether library should be header-only or compiled
**Considerations**:
- Header-only simplifies integration but increases compile times
- Compiled library reduces compile times but requires linking
- Many modern C++ libraries (CLI11, cxxopts) are header-only
- Existing library (colorfmt) is compiled (C library)

**Pending**: Results from librarian agent on industry patterns.

### 4. Error Handling Implementation
**Status**: Resolved (from spec clarification)
**Decision**: Mixed approach - exceptions for configuration errors, error objects/results for parsing failures
**Rationale**: 
- Configuration errors (programming mistakes) should fail fast with exceptions
- Parsing failures (user input errors) should be recoverable with error objects
- Follows modern C++ error handling best practices

### 5. API Design Patterns
**Status**: Resolved (from spec clarification)
**Decision**: Builder/fluent API pattern
**Rationale**:
- Provides clean, readable configuration syntax
- Chainable methods enable expressive API
- Common pattern in modern C++ libraries (CLI11 uses similar approach)

### 6. POSIX-style Parsing Implementation
**Status**: Research Needed
**Unknown**: Specific implementation details for POSIX-style option parsing
**Considerations**:
- Need to handle `-abc` → `-a -b -c` expansion for boolean flags
- Support for `--` end-of-options delimiter
- Handling of option values starting with `-`

**Pending**: Results from librarian agent on implementation patterns.

### 7. CMake Subproject Configuration
**Status**: Resolved
**Decision**: Follow existing library patterns with compiled library approach
**Rationale**: 
- Existing library (colorfmt) uses simple CMake pattern: `add_library(colorfmt STATIC colorfmt.c)` with `target_include_directories()`
- Main project integrates libraries via `add_subdirectory(library/colorfmt)`
- Libraries are placed in `library/` directory with own CMakeLists.txt
- Public headers should be placed in `include/carsenal/cmdline_parser/` for external consumption
- Internal headers remain in library directory
**Pattern to follow**: Create `library/cmdline_parser/CMakeLists.txt` with library definition, update root CMakeLists.txt to include via `add_subdirectory`

### 8. Project Structure Patterns
**Status**: Resolved
**Decision**: Follow existing CArsenal library patterns with C++ adaptations
**Rationale**: 
- Project uses clear separation: `library/` for project libraries, `include/` for public headers, `third/` for dependencies
- Existing `colorfmt` library shows C library pattern: simple CMakeLists.txt, header/source separation
- For C++ library: Use namespace `carsenal::cmdline_parser`, modern C++ features, but maintain similar build integration
- Directory structure: `library/cmdline_parser/include/carsenal/cmdline_parser/` for public headers, `src/` for implementation
- Testing: Create `tests/` directory within library with Catch2 tests
**Key patterns identified**:
- CMake: `add_library()` with `target_include_directories()`
- Integration: Main CMakeLists.txt uses `add_subdirectory()`
- Documentation: Each library has `.md` file explaining purpose
- Naming: Lowercase with underscores for files, clear function naming

## Consolidated Decisions

1. **C++ Standard**: C++17 minimum (matches project standard)
2. **Testing Framework**: Catch2 (header-only, modern, compatible with vcpkg)
3. **CMake Integration**: Follow existing library pattern with `add_subdirectory()` integration
4. **Project Structure**: Library in `library/cmdline_parser/` with namespace separation
5. **Error Handling**: Mixed approach (exceptions for config, error objects for parsing)
6. **API Design**: Builder/fluent API pattern
7. **POSIX Parsing**: Implement POSIX-style option expansion (`-abc` → `-a -b -c`)

### 9. Existing Command Line Parser Libraries in Project
**Status**: Critical Finding
**Discovery**: CArsenal project already includes:
1. **CLI11** (v2.5.0) in `/third/cmdline_parser/cli11/CLI11.hpp` - Modern header-only C++ library
2. **gflags** in `/third/cmdline_parser/gflags/` - Google's command line flags library  
3. **Boost.Program_options** usage in `src/CArsenal.cpp` - Traditional Boost-based approach

**Implications**: 
- Project already has a mature command line parsing library (CLI11)
- Main program uses Boost.Program_options with fluent builder pattern
- Need to decide: implement new library vs wrap/adapt existing CLI11

**Decision**: Implement new library inspired by CLI11 patterns but tailored for CArsenal
**Rationale**:
- Learning opportunity: Implement parser from scratch to understand internals
- Customization: Can tailor specifically to CArsenal project needs and patterns  
- Modern C++: Can use C++17/20 features not available in existing CLI11 version
- Integration: Better integration with CArsenal's build system and conventions
**Alternatives considered**: 
- Use existing CLI11 directly (misses learning/customization opportunity)
- Extend existing CLI11 (version may be outdated, less control)

### 10. Library Architecture Decision
**Status**: Resolved
**Decision**: Header-only library with optional compiled components
**Rationale**:
- Best of both worlds: Easy integration (header-only) with performance optimization options
- Modern C++ libraries (CLI11, cxxopts) use header-only approach successfully
- Can provide separate compilation unit for non-template code to reduce compile times
- Follows industry trend for modern C++ libraries
**Pattern**: Main header with template code, optional `.cpp` for non-template implementation

### 11. POSIX Parsing Implementation Details
**Status**: Resolved
**Decision**: Implement POSIX-compliant parser with `getopt_long` compatibility
**Key Requirements**:
- Short options: `-a`, `-b`, `-c` (can be combined: `-abc`)
- Long options: `--help`, `--verbose`
- Option arguments: `-f file`, `--file=file`
- Option termination: `--` to mark end of options
- Windows-style support: `/help` for cross-platform compatibility

**Implementation Approach**: 
- State machine with classifier enum (NONE, POSITIONAL_MARK, SHORT, LONG, etc.)
- Separate parsing logic from configuration/validation
- Support for combined short options with validation (only boolean flags can be combined)

## Consolidated Decisions

1. **C++ Standard**: C++17 minimum (matches project standard)
2. **Testing Framework**: Catch2 (header-only, modern, compatible with vcpkg)
3. **CMake Integration**: Follow existing library pattern with `add_subdirectory()` integration
4. **Project Structure**: Library in `library/cmdline_parser/` with namespace separation
5. **Error Handling**: Mixed approach (exceptions for config, error objects for parsing)
6. **API Design**: Builder/fluent API pattern (inspired by CLI11 but tailored)
7. **POSIX Parsing**: Implement POSIX-style option expansion (`-abc` → `-a -b -c`)
8. **Library Architecture**: Header-only with optional compiled components
9. **Existing Libraries**: Acknowledge but implement new library for learning/customization
10. **Implementation Priority**: Core parser → Builder API → Advanced features

**Implementation Strategy**:
- **Phase 1**: Core POSIX parser with basic option support
- **Phase 2**: Builder/fluent API for configuration
- **Phase 3**: Advanced features (validation, subcommands, help generation)
- **Phase 4**: Integration features (config files, environment variables)