# Data Model: Command Line Parser Library

**Date**: 2026-01-11  
**Purpose**: Define core entities, relationships, and data structures for the command line parsing library.

## Entities

### Option
Represents a command line option with configuration properties.

**Attributes**:
- `name` (string): Primary identifier for the option (e.g., "verbose", "output")
- `short_name` (char, optional): Single-character short option (e.g., 'v')
- `long_name` (string, optional): Long option name (e.g., "verbose")
- `description` (string): Help text description
- `type` (enum): Data type (boolean, string, integer, floating_point, custom)
- `default_value` (variant, optional): Default value if option not provided
- `required` (boolean): Whether option must be provided
- `repeatable` (boolean): Whether option can appear multiple times (accumulate values)
- `validation_rules` (list of ValidationRule): Constraints for option values
- `callback` (function, optional): Custom handler invoked when option is parsed

**Relationships**:
- Belongs to a Parser (many-to-one)
- Has zero or more ValidationRules (one-to-many)

### Parser
Main component that processes command line arguments according to configured options.

**Attributes**:
- `options` (map<string, Option>): Configured options by name
- `subcommands` (map<string, Parser>): Nested parsers for subcommands
- `program_name` (string): Name of the program for help text
- `program_description` (string): Description of the program
- `version` (string): Version information for `--version` flag
- `allow_unknown_options` (boolean): Whether to reject unknown options (default: false)
- `posix_style` (boolean): Whether to enable POSIX-style short option grouping (default: true)

**Relationships**:
- Contains multiple Options (one-to-many)
- Can contain nested Parsers for subcommands (recursive)

### ValidationRule
Defines constraints for option values.

**Attributes**:
- `type` (enum): Validation type (range, pattern, custom, etc.)
- `parameters` (map<string, variant>): Validation-specific parameters
- `error_message` (string): Custom error message for validation failures

**Relationships**:
- Associated with an Option (many-to-one)

### ParseResult
Container for parsed values and any errors encountered during parsing.

**Attributes**:
- `values` (map<string, variant>): Parsed option values keyed by option name
- `errors` (list of ParseError): Any parsing errors encountered
- `subcommand` (string, optional): Selected subcommand if applicable
- `remaining_args` (list of string): Arguments after `--` delimiter

**Relationships**:
- Result of Parser parsing operation

### ParseError
Represents a parsing error with contextual information.

**Attributes**:
- `type` (enum): Error type (unknown_option, type_mismatch, missing_required, validation_failed)
- `message` (string): Human-readable error message
- `argument` (string, optional): Offending command line argument
- `option_name` (string, optional): Related option name

## Data Types

### Supported Option Value Types
1. **boolean**: True/false flag (no value required)
2. **string**: Text value
3. **integer**: Whole number
4. **floating_point**: Decimal number
5. **string_list**: Collection of strings (for repeatable options)

### Validation Rule Types
1. **range**: Numeric range validation (min, max)
2. **pattern**: Regular expression pattern matching
3. **choice**: Value must be from predefined set
4. **custom**: User-defined validation function

## State Transitions

### Parser Configuration Lifecycle
1. **Construction**: Parser created with default settings
2. **Configuration**: Options and subcommands added via builder API
3. **Validation**: Configuration validated (no duplicate options, valid names)
4. **Ready**: Parser ready to parse arguments
5. **Parsing**: Process command line arguments, produce ParseResult
6. **Result**: ParseResult contains values or errors

### ParseResult States
1. **Success**: No errors, values populated
2. **Partial**: Some errors but parsing continued (configurable)
3. **Failure**: Critical errors, parsing terminated early

## Constraints

### Naming Constraints
- Short option names: Single character, alphanumeric
- Long option names: Alphanumeric with hyphens, cannot start with hyphen
- Option names must be unique within a parser/subcommand scope

### Value Constraints
- Boolean options cannot have explicit values (except `--flag=true` style)
- Required options must be provided unless a default value is specified
- Validation rules are applied after type conversion

## Relationships Diagram

```
Parser 1───* Option
Option 1───* ValidationRule
Parser 1───* Parser (subcommands)
Parser 1───1 ParseResult (output)
ParseResult 1───* ParseError
```