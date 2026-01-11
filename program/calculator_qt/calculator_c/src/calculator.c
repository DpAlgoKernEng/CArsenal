#include "calculator.h"
#include "functions.h"
#include "constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void init_calculator(Calculator* calc) {
    if (calc == NULL) {
        return;
    }
    
    calc->error.type = CALC_ERROR;
    calc->error.message[0] = '\0';
}

double evaluate(Calculator* calc, ASTNode* node) {
    if (calc == NULL || node == NULL) {
        if (calc != NULL) {
            init_error(&calc->error, EVALUATION_ERROR, "空节点");
        }
        return 0.0;
    }
    
    switch (node->type) {
        case NODE_NUMBER:
            return node->data.value;
            
        case NODE_CONSTANT: {
            if (!is_constant(node->data.constant_name)) {
                init_error(&calc->error, EVALUATION_ERROR, "未知常量");
                return 0.0;
            }
            return get_constant_value(node->data.constant_name);
        }
            
        case NODE_BINARY_OP: {
            double left = evaluate(calc, node->data.binary_op.left);
            if (calc->error.message[0] != '\0') {
                return 0.0;
            }
            
            double right = evaluate(calc, node->data.binary_op.right);
            if (calc->error.message[0] != '\0') {
                return 0.0;
            }
            
            return apply_operator(calc, node->data.binary_op.op, left, right);
        }
            
        case NODE_UNARY_OP: {
            double operand = evaluate(calc, node->data.unary_op.operand);
            if (calc->error.message[0] != '\0') {
                return 0.0;
            }
            
            return apply_unary_operator(calc, node->data.unary_op.op, operand);
        }
            
        case NODE_FUNCTION_CALL: {
            double* args = (double*)malloc(node->data.function_call.arg_count * sizeof(double));
            if (args == NULL) {
                init_error(&calc->error, EVALUATION_ERROR, "内存分配失败");
                return 0.0;
            }
            
            for (int i = 0; i < node->data.function_call.arg_count; i++) {
                args[i] = evaluate(calc, node->data.function_call.args[i]);
                if (calc->error.message[0] != '\0') {
                    free(args);
                    return 0.0;
                }
            }
            
            double result = apply_function(calc, node->data.function_call.name, args, node->data.function_call.arg_count);
            free(args);
            return result;
        }
            
        default:
            init_error(&calc->error, EVALUATION_ERROR, "未知节点类型");
            return 0.0;
    }
}

double apply_operator(Calculator* calc, char op, double left, double right) {
    if (calc == NULL) {
        return 0.0;
    }
    
    switch (op) {
        case '+':
            return left + right;
        case '-':
            return left - right;
        case '*':
            return left * right;
        case '/':
            if (right == 0) {
                init_error(&calc->error, EVALUATION_ERROR, "除零错误");
                return 0.0;
            }
            return left / right;
        case '^':
            return pow(left, right);
        default:
            init_error(&calc->error, EVALUATION_ERROR, "未知操作符");
            return 0.0;
    }
}

double apply_unary_operator(Calculator* calc, char op, double operand) {
    if (calc == NULL) {
        return 0.0;
    }
    
    switch (op) {
        case '+':
            return operand;
        case '-':
            return -operand;
        default:
            init_error(&calc->error, EVALUATION_ERROR, "未知一元操作符");
            return 0.0;
    }
}

double apply_function(Calculator* calc, const char* func_name, double* args, int arg_count) {
    if (calc == NULL || func_name == NULL || args == NULL) {
        return 0.0;
    }
    
    if (!is_function(func_name)) {
        init_error(&calc->error, EVALUATION_ERROR, "未知函数");
        return 0.0;
    }
    
    return evaluate_function(func_name, args, arg_count);
}