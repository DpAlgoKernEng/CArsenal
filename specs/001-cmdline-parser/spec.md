# Feature Specification: Command Line Parser Library

**Feature Branch**: `001-cmdline-parser`  
**Created**: 2026-01-11  
**Status**: Draft  
**Input**: User description: "@library/cmdline_parser/ 目标实现命令行解析器，放入这个目录下，使用cmake子项目"

## Clarifications

### Session 2026-01-11

- Q: How should the library report parsing and configuration errors to the caller? → A: Mixed approach - Exceptions for configuration errors (programming mistakes), error objects/results for parsing failures (user input errors)
- Q: How should the library handle ambiguous short option grouping and duplicate options? → A: POSIX-style - `-abc` expands to `-a -b -c` (if all are boolean flags). Last duplicate value wins.
- Q: Should the library support reading options from sources other than the command line? → A: Command line only (strict scope) - Parse only from `argv`/`argc`
- Q: What non-functional requirements should be prioritized for this command line parsing library? → A: Performance, memory safety, and basic error handling - Balance performance with robustness
- Q: What API design style should be used for configuring and using the parser? → A: Builder/fluent API - Chain method calls to configure parser (`app.add_option(...).add_flag(...).parse()`)

## User Scenarios & Testing *(mandatory)*

<!--
  IMPORTANT: User stories should be PRIORITIZED as user journeys ordered by importance.
  Each user story/journey must be INDEPENDENTLY TESTABLE - meaning if you implement just ONE of them,
  you should still have a viable MVP (Minimum Viable Product) that delivers value.
  
  Assign priorities (P1, P2, P3, etc.) to each story, where P1 is the most critical.
  Think of each story as a standalone slice of functionality that can be:
  - Developed independently
  - Tested independently
  - Deployed independently
  - Demonstrated to users independently
-->

### User Story 1 - Parse Basic Command Line Options (Priority: P1)

As a developer, I want to parse basic command line options (short and long flags with values) so that I can easily access program configuration from the command line.

**Why this priority**: This is the core functionality of any command line parser library. Without basic option parsing, the library cannot serve its primary purpose. All other features build upon this foundation.

**Independent Test**: Can be fully tested by creating a simple program that uses the library to parse command line arguments and verify that parsed values match expected inputs. This delivers immediate value by enabling basic command line functionality.

**Acceptance Scenarios**:

1. **Given** a program with a configured parser for options `--input` (string) and `-v` (boolean flag), **When** the program is invoked with `--input data.txt -v`, **Then** the parser successfully extracts `input="data.txt"` and `v=true`
2. **Given** a program with a configured parser for numeric option `--count` (integer), **When** the program is invoked with `--count 42`, **Then** the parser successfully extracts `count=42` and converts it to integer type

---

### User Story 2 - Generate Help Text and Ensure Type Safety (Priority: P2)

As a developer, I want the library to automatically generate help text and provide type-safe parsing so that users get proper documentation and avoid runtime type errors.

**Why this priority**: Help text is essential for user-friendly command line tools, and type safety prevents common programming errors. These features significantly improve developer productivity and user experience.

**Independent Test**: Can be fully tested by configuring a parser with documented options and verifying that help text is generated correctly, and that type conversions are validated at parse time. This delivers value by reducing documentation burden and preventing runtime errors.

**Acceptance Scenarios**:

1. **Given** a parser configured with option `--output` (required string) and description "Output file path", **When** the program is invoked with `--help`, **Then** the help text includes the option description and indicates it's required
2. **Given** a parser configured for integer option `--port`, **When** the program is invoked with `--port abc`, **Then** the parser provides a clear error message about type mismatch rather than silently failing

---

### User Story 3 - Support Subcommands and Advanced Features (Priority: P3)

As a developer building complex command line tools, I want to support subcommands (like `git commit`, `git push`) and advanced features (default values, value validation, custom callbacks) so that I can create sophisticated CLI applications.

**Why this priority**: Subcommands are essential for organizing complex functionality in CLI tools, and advanced features provide flexibility for real-world use cases. While not required for basic tools, these features are necessary for professional-grade applications.

**Independent Test**: Can be fully tested by creating a program with multiple subcommands (e.g., `create`, `list`, `delete`) and verifying that each subcommand can have its own set of options and validation rules. This delivers value by enabling complex command line interfaces.

**Acceptance Scenarios**:

1. **Given** a program with subcommands `create` (with `--name` option) and `list` (with `--verbose` flag), **When** the program is invoked with `create --name project1`, **Then** the parser correctly routes to the `create` subcommand with `name="project1"`
2. **Given** a parser with option `--level` that must be between 1 and 10, **When** the program is invoked with `--level 15`, **Then** the parser provides a validation error message

---

[Add more user stories as needed, each with an assigned priority]

### Edge Cases

- What happens when users provide unknown options? (Returns parsing error with clear message)
- How does system handle ambiguous options? (POSIX-style: `-abc` expands to `-a -b -c` when all are boolean flags)
- How are duplicate options handled? (Last duplicate value wins)
- What happens when required options are missing?
- How are empty string values handled for string options?
- How does the parser handle `--` (end of options delimiter)?
- What happens with option values that start with `-` (e.g., `--file -name.txt`)?
- How does the parser handle extremely long command lines?

## Requirements *(mandatory)*

<!--
  ACTION REQUIRED: The content in this section represents placeholders.
  Fill them out with the right functional requirements.
-->

### Functional Requirements

- **FR-001**: Library MUST parse short options (single dash, e.g., `-v`) and long options (double dash, e.g., `--verbose`)
- **FR-002**: Library MUST support option values of different types (boolean flags, strings, integers, floating-point numbers)
- **FR-003**: Library MUST generate automatic help text based on option configurations
- **FR-004**: Library MUST support subcommands with separate option sets for each subcommand
- **FR-005**: Library MUST validate option values (type checking, range validation, custom validation rules)
- **FR-006**: Library MUST provide clear error messages for parsing failures (unknown options, type errors, missing required options) using a mixed approach: exceptions for configuration errors (programming mistakes), error objects/results for parsing failures (user input errors)
- **FR-007**: Library MUST support default values for optional options
- **FR-008**: Library MUST handle the `--` delimiter (end of options)
- **FR-009**: Library MUST be usable as a CMake subproject (self-contained build system)
- **FR-010**: Library MUST follow C++ best practices (RAII, exception safety, modern C++ standards)
- **FR-011**: Library MUST follow POSIX-style expansion for ambiguous short options (`-abc` expands to `-a -b -c` when all are boolean flags) and last duplicate value wins
- **FR-012**: Library MUST use builder/fluent API design pattern for configuration (chainable method calls like `app.add_option(...).add_flag(...).parse()`)

### Key Entities *(include if feature involves data)*

- **Option**: Represents a command line option with properties like name, description, type, default value, and validation rules
- **Parser**: Main component that processes command line arguments according to configured options and subcommands
- **ValidationRule**: Defines constraints for option values (range, pattern, custom logic)
- **ParseResult**: Container for parsed values and any errors encountered during parsing

## Success Criteria *(mandatory)*

<!--
  ACTION REQUIRED: Define measurable success criteria.
  These must be technology-agnostic and measurable.
-->

### Assumptions and Dependencies

- The library will be implemented in C++ and use modern C++ standards (C++17 or later)
- The library will be built using CMake as a self-contained subproject
- Users are developers familiar with command line interface conventions
- The library will be used primarily in console applications
- No external dependencies beyond standard library (except possibly for testing)
- Scope is limited to command line argument parsing only (no environment variables or configuration file support)

### Non-Functional Requirements

- **Performance**: Library must add negligible overhead for typical command line parsing tasks
- **Memory Safety**: Must avoid common memory errors (use RAII, avoid raw pointers where possible)
- **Error Handling**: Robust error reporting with clear messages for user input errors
- **Portability**: Should work across major platforms (Linux, macOS, Windows) with standard C++ compilers
- **Developer Experience**: Clean, intuitive API with comprehensive documentation

### Measurable Outcomes

- **SC-001**: Developers can integrate the library into a project and parse basic command line options within 10 minutes of reading documentation
- **SC-002**: Command line tools using this library reduce parsing-related bugs by 90% compared to manual argument parsing
- **SC-003**: 95% of common command line patterns (short/long options, flags, values, help) are supported without custom code
- **SC-004**: Library adds negligible performance overhead for typical command line parsing tasks
