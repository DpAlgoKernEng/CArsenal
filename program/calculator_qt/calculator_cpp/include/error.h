#ifndef ERROR_H
#define ERROR_H

#include <string>
#include <stdexcept>

// 自定义异常类
class CalcError : public std::runtime_error {
public:
    explicit CalcError(const std::string& message) : std::runtime_error(message) {}
};

// 词法分析错误
class LexicalError : public CalcError {
public:
    explicit LexicalError(const std::string& message) : CalcError("词法错误: " + message) {}
};

// 语法分析错误
class SyntaxError : public CalcError {
public:
    explicit SyntaxError(const std::string& message) : CalcError("语法错误: " + message) {}
};

// 计算错误
class EvaluationError : public CalcError {
public:
    explicit EvaluationError(const std::string& message) : CalcError("计算错误: " + message) {}
};

#endif // ERROR_H