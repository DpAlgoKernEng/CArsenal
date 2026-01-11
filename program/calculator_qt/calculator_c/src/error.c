#include "error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void init_error(CalcError* error, ErrorType type, const char* message) {
    if (error == NULL || message == NULL) {
        return;
    }
    
    error->type = type;
    strncpy(error->message, message, sizeof(error->message) - 1);
    error->message[sizeof(error->message) - 1] = '\0'; // 确保字符串结尾
}

void print_error(const CalcError* error) {
    if (error == NULL) {
        return;
    }
    
    switch (error->type) {
        case LEXICAL_ERROR:
            printf("词法错误: %s\n", error->message);
            break;
        case SYNTAX_ERROR:
            printf("语法错误: %s\n", error->message);
            break;
        case EVALUATION_ERROR:
            printf("计算错误: %s\n", error->message);
            break;
        default:
            printf("错误: %s\n", error->message);
            break;
    }
}

void free_error(CalcError* error) {
    // 在这个简单的实现中，不需要释放内存
    // 但如果error结构变得更复杂，可能需要释放资源
    if (error != NULL) {
        error->type = CALC_ERROR;
        error->message[0] = '\0';
    }
}