#pragma once

#include <memory>
#include <optional>
#include <string>

namespace cars::cmdline_parser {

/**
 * @brief Types of parsing errors
 */
enum class ParseErrorType {
    unknown_option,    ///< Unknown option encountered
    type_mismatch,     ///< Value cannot be converted to expected type
    missing_required,  ///< Required option not provided
    validation_failed, ///< Value failed validation constraints
    duplicate_option,  ///< Duplicate option violation
    invalid_format,    ///< Option format is invalid
    missing_value,     ///< Option requires a value but none provided
    extra_value,       ///< Option received unexpected value
    subcommand_error,  ///< Subcommand-related error
    internal_error     ///< Internal parser error (should not occur)
};

/**
 * @brief Represents a parsing error with contextual information
 */
class ParseError {
public:
    /**
     * @brief Construct a parsing error
     *
     * @param type Error type
     * @param message Human-readable error message
     * @param argument Offending command line argument (if applicable)
     * @param option_name Related option name (if applicable)
     */
    ParseError(ParseErrorType             type,
               std::string                message,
               std::optional<std::string> argument = std::nullopt,
               std::optional<std::string> option_name = std::nullopt);

    /**
     * @brief Destroy the ParseError object
     */
    ~ParseError();

    // Allow copying (errors are value types)
    ParseError(const ParseError&);
    ParseError& operator=(const ParseError&);

    // Allow moving
    ParseError(ParseError&&) noexcept;
    ParseError& operator=(ParseError&&) noexcept;

    /**
     * @brief Get the error type
     *
     * @return ParseErrorType Error classification
     */
    ParseErrorType type() const;

    /**
     * @brief Get the error message
     *
     * @return const std::string& Human-readable message
     */
    const std::string& message() const;

    /**
     * @brief Get the offending argument
     *
     * @return std::optional<std::string> Argument that caused the error, if
     * applicable
     */
    std::optional<std::string> argument() const;

    /**
     * @brief Get the related option name
     *
     * @return std::optional<std::string> Option name related to the error, if
     * applicable
     */
    std::optional<std::string> option_name() const;

    /**
     * @brief Convert error to string representation
     *
     * @return std::string Formatted error description
     */
    std::string to_string() const;

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace cars::cmdline_parser