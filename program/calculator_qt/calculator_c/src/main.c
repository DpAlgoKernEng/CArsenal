#include "ui.h"
#include "parser.h"
#include "calculator.h"
#include "error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    show_welcome();
    
    char input[256];
    
    while (1) {
        get_user_input(input, sizeof(input));
        
        // 检查退出命令
        if (strcmp(input, "quit") == 0 || strcmp(input, "exit") == 0) {
            printf("感谢使用科学计算器！\n");
            break;
        }
        
        // 检查帮助命令
        if (strcmp(input, "help") == 0) {
            show_help();
            continue;
        }
        
        // 跳过空输入
        if (strlen(input) == 0) {
            continue;
        }
        
        // 初始化解析器
        Parser parser;
        init_parser(&parser, input);
        
        // 解析表达式
        ASTNode* ast = parse_expression(&parser);
        
        // 检查解析是否成功
        if (ast == NULL) {
            show_error("表达式解析失败");
            continue;
        }
        
        // 检查是否还有未处理的字符
        // 对于正确的表达式解析，这里应该没有未处理的字符
        // 添加调试信息
        if (parser.lexer.current_token.type != TOKEN_END) {
            free_ast(ast);
            show_error("表达式解析完成后仍有未处理的字符");
            continue;
        }
        
        // 初始化计算器
        Calculator calc;
        init_calculator(&calc);
        
        // 计算结果
        double result = evaluate(&calc, ast);
        
        // 释放AST内存
        free_ast(ast);
        
        // 检查计算是否有错误
        if (calc.error.message[0] != '\0') {
            show_error(calc.error.message);
            continue;
        }
        
        // 显示结果
        show_result(result);
    }
    
    return 0;
}