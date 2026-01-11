#pragma once

#include <map>
#include <memory>
#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace cars::cmdline_parser {

class ParseError;

/**
 * @brief Result of command line parsing operation
 *
 * Contains parsed values, any errors encountered, and additional parsing
 * context. Supports both successful parsing (no errors) and partial/failed
 * parsing.
 */
class ParseResult {
public:
    /**
     * @brief Construct a successful parse result with values
     *
     * @param values Parsed option values
     * @param subcommand Selected subcommand (if any)
     * @param remaining_args Arguments after `--` delimiter
     */
    ParseResult(std::map<std::string,
                         std::variant<std::string,
                                      bool,
                                      int,
                                      double,
                                      std::vector<std::string>>> values,
                std::optional<std::string> subcommand = std::nullopt,
                std::vector<std::string>   remaining_args = {});

    /**
     * @brief Construct a parse result with errors
     *
     * @param errors Parsing errors encountered
     * @param partial_values Partially parsed values (if parsing continued after
     * errors)
     * @param subcommand Selected subcommand (if any)
     * @param remaining_args Arguments after `--` delimiter
     */
    ParseResult(
        std::vector<ParseError>                          errors,
        std::map<std::string,
                 std::variant<std::string,
                              bool,
                              int,
                              double,
                              std::vector<std::string>>> partial_values = {},
        std::optional<std::string> subcommand = std::nullopt,
        std::vector<std::string>   remaining_args = {});

    /**
     * @brief Destroy the ParseResult object
     */
    ~ParseResult();

    // Disable copying
    ParseResult(const ParseResult&) = delete;
    ParseResult& operator=(const ParseResult&) = delete;

    // Allow moving
    ParseResult(ParseResult&&) noexcept;
    ParseResult& operator=(ParseResult&&) noexcept;

    /**
     * @brief Check if parsing was successful (no errors)
     *
     * @return true if no errors were encountered
     * @return false if any errors were encountered
     */
    bool success() const;

    /**
     * @brief Check if parsing failed (has errors)
     *
     * @return true if any errors were encountered
     * @return false if no errors were encountered
     */
    bool failed() const;

    /**
     * @brief Get the number of errors encountered
     *
     * @return size_t Error count
     */
    size_t error_count() const;

    /**
     * @brief Get reference to all parsing errors
     *
     * @return const std::vector<ParseError>& List of errors
     */
    const std::vector<ParseError>& errors() const;

    /**
     * @brief Get parsed value for an option
     *
     * @tparam T Expected value type (bool, int, double, std::string,
     * std::vector<std::string>)
     * @param option_name Name of the option
     * @return T Parsed value
     * @throws std::out_of_range if option not found
     * @throws std::bad_variant_access if type mismatch
     */
    template <typename T> T get(const std::string& option_name) const;

    /**
     * @brief Try to get parsed value for an option
     *
     * @tparam T Expected value type
     * @param option_name Name of the option
     * @return std::optional<T> Value if present and of correct type, nullopt
     * otherwise
     */
    template <typename T>
    std::optional<T> try_get(const std::string& option_name) const;

    /**
     * @brief Check if an option was parsed (has a value)
     *
     * @param option_name Name of the option
     * @return true if option has a parsed value
     * @return false if option not present or has no value
     */
    bool has(const std::string& option_name) const;

    /**
     * @brief Get all parsed values as a map
     *
     * @return const std::map<std::string, std::variant<...>>& Map of all parsed
     * values
     */
    const std::map<
        std::string,
        std::variant<std::string, bool, int, double, std::vector<std::string>>>&
    values() const;

    /**
     * @brief Get selected subcommand (if any)
     *
     * @return std::optional<std::string> Subcommand name, or nullopt if no
     * subcommand selected
     */
    std::optional<std::string> subcommand() const;

    /**
     * @brief Get remaining arguments after `--` delimiter
     *
     * @return const std::vector<std::string>& Arguments after `--`
     */
    const std::vector<std::string>& remaining_args() const;

    /**
     * @brief Convert to bool (success check)
     *
     * @return true if successful (no errors)
     * @return false if failed (has errors)
     */
    explicit operator bool() const;

    /**
     * @brief Get error message summary
     *
     * @return std::string Concatenated error messages
     */
    std::string error_message() const;

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace cars::cmdline_parser