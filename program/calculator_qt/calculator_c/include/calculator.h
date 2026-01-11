#ifndef CALCULATOR_H
#define CALCULATOR_H

#include "parser.h"
#include "error.h"

// 计算器结构
typedef struct {
    CalcError error;
} Calculator;

// 函数声明
void init_calculator(Calculator* calc);
double evaluate(Calculator* calc, ASTNode* node);
double apply_operator(Calculator* calc, char op, double left, double right);
double apply_unary_operator(Calculator* calc, char op, double operand);
double apply_function(Calculator* calc, const char* func_name, double* args, int arg_count);

#endif // CALCULATOR_H