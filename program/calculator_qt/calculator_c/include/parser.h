#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "error.h"
#include <stdlib.h>

// AST节点类型枚举
typedef enum {
    NODE_NUMBER,
    NODE_BINARY_OP,
    NODE_UNARY_OP,
    NODE_FUNCTION_CALL,
    NODE_CONSTANT
} NodeType;

// AST节点结构
typedef struct ASTNode {
    NodeType type;
    union {
        double value;  // 当type为NODE_NUMBER时使用
        struct {
            char op;
            struct ASTNode* left;
            struct ASTNode* right;
        } binary_op;  // 当type为NODE_BINARY_OP时使用
        struct {
            char op;
            struct ASTNode* operand;
        } unary_op;  // 当type为NODE_UNARY_OP时使用
        struct {
            char name[32];
            struct ASTNode** args;
            int arg_count;
        } function_call;  // 当type为NODE_FUNCTION_CALL时使用
        char constant_name[32];  // 当type为NODE_CONSTANT时使用
    } data;
} ASTNode;

// 解析器结构
typedef struct {
    Lexer lexer;
    CalcError error;
} Parser;

// 函数声明
void init_parser(Parser* parser, const char* expression);
ASTNode* parse_expression(Parser* parser);
ASTNode* parse_term(Parser* parser);
ASTNode* parse_factor(Parser* parser);
ASTNode* create_number_node(double value);
ASTNode* create_binary_op_node(char op, ASTNode* left, ASTNode* right);
ASTNode* create_unary_op_node(char op, ASTNode* operand);
ASTNode* create_function_call_node(const char* name, ASTNode** args, int arg_count);
ASTNode* create_constant_node(const char* name);
void free_ast(ASTNode* node);
int get_operator_precedence(char op);

#endif // PARSER_H