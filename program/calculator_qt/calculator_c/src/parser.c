#include "parser.h"
#include "constants.h"
#include "functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void init_parser(Parser* parser, const char* expression) {
    if (parser == NULL || expression == NULL) {
        return;
    }
    
    init_lexer(&parser->lexer, expression);
    parser->error.type = CALC_ERROR;
    parser->error.message[0] = '\0';
}

ASTNode* parse_expression(Parser* parser) {
    if (parser == NULL) {
        return NULL;
    }
    
    ASTNode* left = parse_term(parser);
    if (left == NULL) {
        return NULL;
    }
    
    while (parser->lexer.current_token.type == TOKEN_OPERATOR && 
           (parser->lexer.current_token.op == '+' || parser->lexer.current_token.op == '-')) {
        char op = parser->lexer.current_token.op;
        consume_token(&parser->lexer); // 消费操作符
        ASTNode* right = parse_term(parser);
        if (right == NULL) {
            free_ast(left);
            return NULL;
        }
        
        ASTNode* node = create_binary_op_node(op, left, right);
        if (node == NULL) {
            free_ast(left);
            free_ast(right);
            return NULL;
        }
        
        left = node;
    }
    
    return left;
}

ASTNode* parse_term(Parser* parser) {
    if (parser == NULL) {
        return NULL;
    }
    
    ASTNode* left = parse_factor(parser);
    if (left == NULL) {
        return NULL;
    }
    
    while (parser->lexer.current_token.type == TOKEN_OPERATOR && 
           (parser->lexer.current_token.op == '*' || 
            parser->lexer.current_token.op == '/' || 
            parser->lexer.current_token.op == '^')) {
        char op = parser->lexer.current_token.op;
        consume_token(&parser->lexer); // 消费操作符
        ASTNode* right = parse_factor(parser);
        if (right == NULL) {
            free_ast(left);
            return NULL;
        }
        
        ASTNode* node = create_binary_op_node(op, left, right);
        if (node == NULL) {
            free_ast(left);
            free_ast(right);
            return NULL;
        }
        
        left = node;
    }
    
    return left;
}

ASTNode* parse_factor(Parser* parser) {
    if (parser == NULL) {
        return NULL;
    }
    
    Token token = parser->lexer.current_token;
    
    // 处理数字
    if (token.type == TOKEN_NUMBER) {
        consume_token(&parser->lexer);
        return create_number_node(token.value);
    }
    
    // 处理常量
    if (token.type == TOKEN_CONSTANT) {
        consume_token(&parser->lexer);
        return create_constant_node(token.name);
    }
    
    // 处理函数调用
    if (token.type == TOKEN_FUNCTION) {
        char func_name[32];
        strcpy(func_name, token.name);
        consume_token(&parser->lexer); // 消费函数名
        
        if (parser->lexer.current_token.type != TOKEN_LPAREN) {
            // 设置错误信息
            return NULL;
        }
        consume_token(&parser->lexer); // 消费左括号
        
        // 解析参数列表
        ASTNode** args = NULL;
        int arg_count = 0;
        
        if (parser->lexer.current_token.type != TOKEN_RPAREN) {
            ASTNode* first_arg = parse_expression(parser);
            if (first_arg == NULL) {
                return NULL;
            }
            
            // 分配参数数组
            args = (ASTNode**)malloc(sizeof(ASTNode*));
            if (args == NULL) {
                free_ast(first_arg);
                return NULL;
            }
            
            args[0] = first_arg;
            arg_count = 1;
            
            while (parser->lexer.current_token.type == TOKEN_OPERATOR && 
                   parser->lexer.current_token.op == ',') {
                consume_token(&parser->lexer); // 消费逗号
                ASTNode* arg = parse_expression(parser);
                if (arg == NULL) {
                    // 释放已分配的参数
                    for (int i = 0; i < arg_count; i++) {
                        free_ast(args[i]);
                    }
                    free(args);
                    return NULL;
                }
                
                // 重新分配参数数组
                ASTNode** new_args = (ASTNode**)realloc(args, (arg_count + 1) * sizeof(ASTNode*));
                if (new_args == NULL) {
                    // 释放已分配的参数
                    for (int i = 0; i < arg_count; i++) {
                        free_ast(args[i]);
                    }
                    free(args);
                    free_ast(arg);
                    return NULL;
                }
                
                args = new_args;
                args[arg_count] = arg;
                arg_count++;
            }
        }
        
        if (parser->lexer.current_token.type != TOKEN_RPAREN) {
            // 释放参数
            if (args != NULL) {
                for (int i = 0; i < arg_count; i++) {
                    free_ast(args[i]);
                }
                free(args);
            }
            return NULL;
        }
        consume_token(&parser->lexer); // 消费右括号
        
        return create_function_call_node(func_name, args, arg_count);
    }
    
    // 处理一元操作符
    if (token.type == TOKEN_OPERATOR && 
        (token.op == '+' || token.op == '-')) {
        char op = token.op;
        consume_token(&parser->lexer); // 消费操作符
        ASTNode* operand = parse_factor(parser);
        if (operand == NULL) {
            return NULL;
        }
        return create_unary_op_node(op, operand);
    }
    
    // 处理括号表达式
    if (token.type == TOKEN_LPAREN) {
        consume_token(&parser->lexer); // 消费左括号
        ASTNode* expr = parse_expression(parser);
        if (expr == NULL) {
            return NULL;
        }
        
        if (parser->lexer.current_token.type != TOKEN_RPAREN) {
            free_ast(expr);
            return NULL;
        }
        consume_token(&parser->lexer); // 消费右括号
        return expr;
    }
    
    return NULL;
}

ASTNode* create_number_node(double value) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if (node == NULL) {
        return NULL;
    }
    
    node->type = NODE_NUMBER;
    node->data.value = value;
    return node;
}

ASTNode* create_binary_op_node(char op, ASTNode* left, ASTNode* right) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if (node == NULL) {
        return NULL;
    }
    
    node->type = NODE_BINARY_OP;
    node->data.binary_op.op = op;
    node->data.binary_op.left = left;
    node->data.binary_op.right = right;
    return node;
}

ASTNode* create_unary_op_node(char op, ASTNode* operand) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if (node == NULL) {
        return NULL;
    }
    
    node->type = NODE_UNARY_OP;
    node->data.unary_op.op = op;
    node->data.unary_op.operand = operand;
    return node;
}

ASTNode* create_function_call_node(const char* name, ASTNode** args, int arg_count) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if (node == NULL) {
        return NULL;
    }
    
    node->type = NODE_FUNCTION_CALL;
    strncpy(node->data.function_call.name, name, sizeof(node->data.function_call.name) - 1);
    node->data.function_call.name[sizeof(node->data.function_call.name) - 1] = '\0';
    node->data.function_call.args = args;
    node->data.function_call.arg_count = arg_count;
    return node;
}

ASTNode* create_constant_node(const char* name) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if (node == NULL) {
        return NULL;
    }
    
    node->type = NODE_CONSTANT;
    strncpy(node->data.constant_name, name, sizeof(node->data.constant_name) - 1);
    node->data.constant_name[sizeof(node->data.constant_name) - 1] = '\0';
    return node;
}

void free_ast(ASTNode* node) {
    if (node == NULL) {
        return;
    }
    
    switch (node->type) {
        case NODE_BINARY_OP:
            free_ast(node->data.binary_op.left);
            free_ast(node->data.binary_op.right);
            break;
        case NODE_UNARY_OP:
            free_ast(node->data.unary_op.operand);
            break;
        case NODE_FUNCTION_CALL:
            for (int i = 0; i < node->data.function_call.arg_count; i++) {
                free_ast(node->data.function_call.args[i]);
            }
            free(node->data.function_call.args);
            break;
        default:
            // 其他节点类型不需要特殊处理
            break;
    }
    
    free(node);
}

int get_operator_precedence(char op) {
    switch (op) {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
            return 2;
        case '^':
            return 3;
        default:
            return 0;
    }
}