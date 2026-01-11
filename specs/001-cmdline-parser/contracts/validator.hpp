#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace cars::cmdline_parser {

/**
 * @brief Base class for validation rules
 *
 * Validators check whether option values meet certain constraints.
 */
class Validator {
public:
    /**
     * @brief Destroy the Validator object
     */
    virtual ~Validator() = default;

    /**
     * @brief Validate a string value
     *
     * @param value The value to validate
     * @return std::pair<bool, std::string> First element is true if valid,
     *         second element is error message if invalid
     */
    virtual std::pair<bool, std::string>
    validate(const std::string& value) const = 0;

    /**
     * @brief Get a description of the validation rule
     *
     * @return std::string Human-readable description
     */
    virtual std::string description() const = 0;

    // Disable copying
    Validator(const Validator&) = delete;
    Validator& operator=(const Validator&) = delete;

    // Allow moving
    Validator(Validator&&) noexcept = default;
    Validator& operator=(Validator&&) noexcept = default;

protected:
    Validator() = default;
};

/**
 * @brief Range validator for numeric values
 *
 * Checks that a value falls within a specified inclusive range.
 */
class RangeValidator : public Validator {
public:
    /**
     * @brief Construct a range validator
     *
     * @tparam T Numeric type (int, double, etc.)
     * @param min Minimum allowed value (inclusive)
     * @param max Maximum allowed value (inclusive)
     */
    template <typename T> RangeValidator(T min, T max);

    std::pair<bool, std::string>
                validate(const std::string& value) const override;
    std::string description() const override;

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

/**
 * @brief Pattern validator using regular expressions
 *
 * Checks that a value matches a specified regular expression pattern.
 */
class PatternValidator : public Validator {
public:
    /**
     * @brief Construct a pattern validator
     *
     * @param pattern Regular expression pattern
     * @param description Optional description of the pattern
     */
    PatternValidator(std::string pattern, std::string description = "");

    std::pair<bool, std::string>
                validate(const std::string& value) const override;
    std::string description() const override;

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

/**
 * @brief Choice validator for predefined values
 *
 * Checks that a value is one of a set of allowed values.
 */
class ChoiceValidator : public Validator {
public:
    /**
     * @brief Construct a choice validator
     *
     * @param choices List of allowed values
     */
    ChoiceValidator(std::vector<std::string> choices);

    std::pair<bool, std::string>
                validate(const std::string& value) const override;
    std::string description() const override;

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

/**
 * @brief Custom function validator
 *
 * Wraps a user-provided validation function.
 */
class CustomValidator : public Validator {
public:
    /**
     * @brief Construct a custom validator
     *
     * @param validator Validation function
     * @param description Description of the validation rule
     */
    CustomValidator(std::function<bool(const std::string&)> validator,
                    std::string                             description = "");

    std::pair<bool, std::string>
                validate(const std::string& value) const override;
    std::string description() const override;

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

// Factory functions for common validators

/**
 * @brief Create a range validator
 *
 * @tparam T Numeric type
 * @param min Minimum value
 * @param max Maximum value
 * @return Validator Range validator instance
 */
template <typename T> Validator range(T min, T max);

/**
 * @brief Create a pattern validator
 *
 * @param pattern Regular expression pattern
 * @param description Optional description
 * @return Validator Pattern validator instance
 */
Validator pattern(std::string pattern, std::string description = "");

/**
 * @brief Create a choice validator
 *
 * @param choices Allowed values
 * @return Validator Choice validator instance
 */
Validator choice(std::vector<std::string> choices);

/**
 * @brief Create a custom validator
 *
 * @param validator Validation function
 * @param description Optional description
 * @return Validator Custom validator instance
 */
Validator custom(std::function<bool(const std::string&)> validator,
                 std::string                             description = "");

} // namespace cars::cmdline_parser