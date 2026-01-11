#include "lexer.h"
#include "constants.h"
#include "functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void init_lexer(Lexer* lexer, const char* expression) {
    if (lexer == NULL || expression == NULL) {
        return;
    }
    
    lexer->expression = expression;
    lexer->pos = 0;
    lexer->current_token.type = TOKEN_END;
    lexer->error.type = CALC_ERROR;
    lexer->error.message[0] = '\0';
    
    // 获取第一个token
    consume_token(lexer);
}

void skip_whitespace(Lexer* lexer) {
    if (lexer == NULL || lexer->expression == NULL) {
        return;
    }
    
    while (lexer->expression[lexer->pos] != '\0' && 
           is_whitespace(lexer->expression[lexer->pos])) {
        lexer->pos++;
    }
}

int is_whitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

Token get_next_token(Lexer* lexer) {
    if (lexer == NULL || lexer->expression == NULL) {
        Token error_token = {TOKEN_ERROR, 0.0, "", 0};
        return error_token;
    }
    
    skip_whitespace(lexer);
    
    // 检查是否到达表达式末尾
    if (lexer->expression[lexer->pos] == '\0') {
        Token end_token = {TOKEN_END, 0.0, "", 0};
        return end_token;
    }
    
    char ch = lexer->expression[lexer->pos];
    
    // 处理数字
    if (isdigit(ch) || ch == '.') {
        int start = lexer->pos;
        while (lexer->expression[lexer->pos] != '\0' && 
               (isdigit(lexer->expression[lexer->pos]) || 
                lexer->expression[lexer->pos] == '.')) {
            lexer->pos++;
        }
        
        // 创建临时字符串来存储数字
        int len = lexer->pos - start;
        char* num_str = (char*)malloc(len + 1);
        if (num_str == NULL) {
            Token error_token = {TOKEN_ERROR, 0.0, "", 0};
            return error_token;
        }
        
        strncpy(num_str, &lexer->expression[start], len);
        num_str[len] = '\0';
        
        double value = atof(num_str);
        free(num_str);
        
        Token number_token = {TOKEN_NUMBER, value, "", 0};
        return number_token;
    }
    
    // 处理标识符（函数名或常量）
    if (isalpha(ch)) {
        int start = lexer->pos;
        while (lexer->expression[lexer->pos] != '\0' && 
               isalnum(lexer->expression[lexer->pos])) {
            lexer->pos++;
        }
        
        // 创建临时字符串来存储标识符
        int len = lexer->pos - start;
        if (len >= 32) {
            len = 31; // 防止缓冲区溢出
        }
        
        char name[32];
        strncpy(name, &lexer->expression[start], len);
        name[len] = '\0';
        
        // 检查是否为常量
        if (is_constant(name)) {
            Token constant_token = {TOKEN_CONSTANT, 0.0, "", 0};
            constant_token.type = TOKEN_CONSTANT;
            strcpy(constant_token.name, name);
            return constant_token;
        }
        
        // 检查是否为函数
        if (is_function(name)) {
            Token function_token = {TOKEN_FUNCTION, 0.0, "", 0};
            function_token.type = TOKEN_FUNCTION;
            strcpy(function_token.name, name);
            return function_token;
        }
        
        // 未知标识符
        Token error_token = {TOKEN_ERROR, 0.0, "", 0};
        return error_token;
    }
    
    // 处理操作符
    if (is_operator(ch)) {
        lexer->pos++;
        Token operator_token = {TOKEN_OPERATOR, 0.0, "", ch};
        return operator_token;
    }
    
    // 处理括号
    if (ch == '(') {
        lexer->pos++;
        Token lparen_token = {TOKEN_LPAREN, 0.0, "", 0};
        return lparen_token;
    }
    
    if (ch == ')') {
        lexer->pos++;
        Token rparen_token = {TOKEN_RPAREN, 0.0, "", 0};
        return rparen_token;
    }
    
    // 未知字符
    lexer->pos++;
    Token error_token = {TOKEN_ERROR, 0.0, "", 0};
    return error_token;
}

void consume_token(Lexer* lexer) {
    if (lexer == NULL) {
        return;
    }
    
    lexer->current_token = get_next_token(lexer);
}