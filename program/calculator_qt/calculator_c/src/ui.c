#include "ui.h"
#include <stdio.h>
#include <string.h>

void show_welcome() {
    printf("========================================\n");
    printf("       科学计算器 v1.0 (C语言版本)\n");
    printf("========================================\n");
    printf("支持基本算术运算、科学函数、常量等\n");
    printf("输入 'help' 查看帮助，输入 'quit' 退出\n");
    printf("========================================\n\n");
}

void show_help() {
    printf("\n========== 帮助信息 ==========\n");
    printf("支持的运算符:\n");
    printf("  +, -, *, /, ^ (幂运算)\n\n");
    printf("支持的函数:\n");
    printf("  sin, cos, tan, log, ln, exp, sqrt, abs\n\n");
    printf("支持的常量:\n");
    printf("  pi, e\n\n");
    printf("示例:\n");
    printf("  2 + 3 * 4\n");
    printf("  sin(pi/2)\n");
    printf("  sqrt(16) + log(100)\n");
    printf("=============================\n\n");
}

void get_user_input(char* buffer, int size) {
    if (buffer == NULL || size <= 0) {
        return;
    }
    
    printf(">>> ");
    fflush(stdout);
    
    if (fgets(buffer, size, stdin) != NULL) {
        // 移除换行符
        int len = strlen(buffer);
        if (len > 0 && buffer[len-1] == '\n') {
            buffer[len-1] = '\0';
        }
    } else {
        buffer[0] = '\0';
    }
}

void show_result(double result) {
    printf("= %.10g\n\n", result);
}

void show_error(const char* error) {
    if (error != NULL) {
        printf("错误: %s\n\n", error);
    }
}