#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 错误类型枚举
typedef enum {
    CALC_ERROR,
    LEXICAL_ERROR,
    SYNTAX_ERROR,
    EVALUATION_ERROR
} ErrorType;

// 错误信息结构
typedef struct {
    ErrorType type;
    char message[256];
} CalcError;

// 函数声明
void init_error(CalcError* error, ErrorType type, const char* message);
void print_error(const CalcError* error);
void free_error(CalcError* error);

#endif // ERROR_H