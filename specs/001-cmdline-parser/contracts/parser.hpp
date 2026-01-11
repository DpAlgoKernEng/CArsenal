#pragma once

#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace cars::cmdline_parser {

// Forward declarations
class OptionBuilder;
class ParseResult;
class ParseError;
class Validator;

/**
 * @brief Policy for handling duplicate options
 */
enum class DuplicatePolicy {
    error,     ///< Treat duplicate options as an error
    last_wins, ///< Use the last occurrence value
    accumulate ///< Accumulate values from all occurrences
};

/**
 * @brief Main application parser class
 *
 * Provides fluent builder API for configuring command line options
 * and parsing command line arguments.
 */
class App {
public:
    /**
     * @brief Construct a new command line application parser
     *
     * @param name Program name (used in help text)
     * @param description Program description (used in help text)
     */
    App(std::string name, std::string description = "");

    /**
     * @brief Destroy the App object
     */
    ~App();

    // Disable copying
    App(const App&) = delete;
    App& operator=(const App&) = delete;

    // Allow moving
    App(App&&) noexcept;
    App& operator=(App&&) noexcept;

    /**
     * @brief Add a command line option
     *
     * @param name Option name(s) in format "short,long" or "long"
     * @param description Help text description
     * @return OptionBuilder for fluent configuration
     */
    OptionBuilder add_option(std::string name, std::string description = "");

    /**
     * @brief Add a boolean flag (no value required)
     *
     * @param name Flag name(s) in format "short,long" or "long"
     * @param description Help text description
     * @return OptionBuilder for fluent configuration
     */
    OptionBuilder add_flag(std::string name, std::string description = "");

    /**
     * @brief Add a subcommand
     *
     * @param name Subcommand name
     * @param description Subcommand description
     * @return App& Reference to the subcommand App
     */
    App& add_subcommand(std::string name, std::string description = "");

    /**
     * @brief Parse command line arguments
     *
     * @param argc Argument count from main()
     * @param argv Argument values from main()
     * @return ParseResult containing parsed values or errors
     */
    ParseResult parse(int argc, char* argv[]);

    /**
     * @brief Parse command line arguments from vector
     *
     * @param args Vector of argument strings
     * @return ParseResult containing parsed values or errors
     */
    ParseResult parse(const std::vector<std::string>& args);

    /**
     * @brief Generate help text
     *
     * @return std::string Formatted help text
     */
    std::string help() const;

    /**
     * @brief Set program version
     *
     * @param version Version string
     * @return App& Reference to self for chaining
     */
    App& version(std::string version);

    /**
     * @brief Set footer text for help
     *
     * @param footer Footer text
     * @return App& Reference to self for chaining
     */
    App& footer(std::string footer);

    /**
     * @brief Configure whether to allow unknown options
     *
     * @param allow True to allow unknown options (continue parsing),
     *              false to treat as error (default)
     * @return App& Reference to self for chaining
     */
    App& allow_unknown_options(bool allow = true);

    /**
     * @brief Configure whether to enable POSIX-style short option grouping
     *
     * @param enable True to enable `-abc` → `-a -b -c` expansion (default),
     *               false to treat `-abc` as a single option
     * @return App& Reference to self for chaining
     */
    App& enable_posix_grouping(bool enable = true);

    /**
     * @brief Get the program name
     *
     * @return const std::string& Program name
     */
    const std::string& name() const;

    /**
     * @brief Get the program description
     *
     * @return const std::string& Program description
     */
    const std::string& description() const;

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

/**
 * @brief Fluent builder for option configuration
 *
 * Provides chainable methods for configuring option properties.
 */
class OptionBuilder {
public:
    /**
     * @brief Destroy the Option Builder object
     */
    ~OptionBuilder();

    /**
     * @brief Mark option as required
     *
     * @param required True if option must be provided (default true)
     * @return OptionBuilder& Reference to self for chaining
     */
    OptionBuilder& required(bool required = true);

    /**
     * @brief Set default value for optional option
     *
     * @tparam T Value type
     * @param value Default value
     * @return OptionBuilder& Reference to self for chaining
     */
    template <typename T> OptionBuilder& default_value(T value);

    /**
     * @brief Add validation constraint
     *
     * @param validator Validation function or validator object
     * @param message Custom error message for validation failure
     * @return OptionBuilder& Reference to self for chaining
     */
    OptionBuilder& check(std::function<bool(const std::string&)> validator,
                         std::string                             message = "");

    /**
     * @brief Add validation constraint using Validator object
     *
     * @param validator Validator object
     * @return OptionBuilder& Reference to self for chaining
     */
    OptionBuilder& check(Validator validator);

    /**
     * @brief Set environment variable for option value fallback
     *
     * @param env_var Environment variable name
     * @return OptionBuilder& Reference to self for chaining
     */
    OptionBuilder& env(std::string env_var);

    /**
     * @brief Set expected number of values
     *
     * @param count Expected count (0 for flags, 1 for single values, n for
     * multiples)
     * @return OptionBuilder& Reference to self for chaining
     */
    OptionBuilder& expected(size_t count);

    /**
     * @brief Set expected range of values
     *
     * @param min Minimum number of values
     * @param max Maximum number of values (0 for unlimited)
     * @return OptionBuilder& Reference to self for chaining
     */
    OptionBuilder& expected(size_t min, size_t max);

    /**
     * @brief Set custom callback to be invoked when option is parsed
     *
     * @tparam T Callback type (must be invocable with option value)
     * @param callback Callback function
     * @return OptionBuilder& Reference to self for chaining
     */
    template <typename T> OptionBuilder& callback(T callback);

    /**
     * @brief Set option group for help organization
     *
     * @param group Group name
     * @return OptionBuilder& Reference to self for chaining
     */
    OptionBuilder& group(std::string group);

    /**
     * @brief Mark option as deprecated
     *
     * @param message Deprecation message (shown when option is used)
     * @return OptionBuilder& Reference to self for chaining
     */
    OptionBuilder& deprecated(std::string message = "");

    /**
     * @brief Suggest alternative for deprecated option
     *
     * @param alternative Alternative option name
     * @return OptionBuilder& Reference to self for chaining
     */
    OptionBuilder& suggest(std::string alternative);

    /**
     * @brief Set option type (for explicit type specification)
     *
     * @tparam T Type to enforce
     * @return OptionBuilder& Reference to self for chaining
     */
    template <typename T> OptionBuilder& type();

    /**
     * @brief Configure how duplicate options are handled
     *
     * @param policy Duplicate handling policy
     * @return OptionBuilder& Reference to self for chaining
     */
    OptionBuilder& duplicate_policy(DuplicatePolicy policy);

private:
    friend class App;
    OptionBuilder(App& app, std::string name, std::string description);
    class Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace cars::cmdline_parser