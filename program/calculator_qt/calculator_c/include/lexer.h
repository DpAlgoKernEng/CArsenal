#ifndef LEXER_H
#define LEXER_H

#include "error.h"

// Token类型枚举
typedef enum {
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_FUNCTION,
    TOKEN_CONSTANT,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_END,
    TOKEN_ERROR
} TokenType;

// Token结构
typedef struct {
    TokenType type;
    double value;       // 当type为TOKEN_NUMBER时使用
    char name[32];      // 当type为TOKEN_FUNCTION或TOKEN_CONSTANT时使用
    char op;            // 当type为TOKEN_OPERATOR时使用
} Token;

// 词法分析器结构
typedef struct {
    const char* expression;
    int pos;
    Token current_token;
    CalcError error;
} Lexer;

// 函数声明
void init_lexer(Lexer* lexer, const char* expression);
Token get_next_token(Lexer* lexer);
void consume_token(Lexer* lexer);
int is_operator(char c);
int is_whitespace(char c);

#endif // LEXER_H