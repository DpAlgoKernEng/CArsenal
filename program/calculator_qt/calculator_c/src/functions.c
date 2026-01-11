#include "functions.h"
#include "error.h"
#include <math.h>
#include <string.h>
#include <stdio.h>

// 定义函数数组
static Function functions[] = {
    {"sin", func_sin, 1, 1},
    {"cos", func_cos, 1, 1},
    {"tan", func_tan, 1, 1},
    {"log", func_log, 1, 1},
    {"ln", func_ln, 1, 1},
    {"exp", func_exp, 1, 1},
    {"sqrt", func_sqrt, 1, 1},
    {"abs", func_abs, 1, 1}
};

// 函数数量
static int functions_count = sizeof(functions) / sizeof(Function);

int is_function(const char* name) {
    if (name == NULL) {
        return 0;
    }
    
    for (int i = 0; i < functions_count; i++) {
        if (strcmp(functions[i].name, name) == 0) {
            return 1;
        }
    }
    
    return 0;
}

FunctionPtr get_function(const char* name) {
    if (name == NULL) {
        return NULL;
    }
    
    for (int i = 0; i < functions_count; i++) {
        if (strcmp(functions[i].name, name) == 0) {
            return functions[i].func;
        }
    }
    
    return NULL;
}

int get_function_arg_count(const char* name) {
    if (name == NULL) {
        return 0;
    }
    
    for (int i = 0; i < functions_count; i++) {
        if (strcmp(functions[i].name, name) == 0) {
            return functions[i].min_args;
        }
    }
    
    return 0;
}

double evaluate_function(const char* name, double* args, int arg_count) {
    if (name == NULL || args == NULL) {
        return 0.0;
    }
    
    // 查找函数
    for (int i = 0; i < functions_count; i++) {
        if (strcmp(functions[i].name, name) == 0) {
            // 检查参数数量
            if (arg_count < functions[i].min_args || arg_count > functions[i].max_args) {
                return 0.0;
            }
            
            // 调用函数
            return functions[i].func(args, arg_count);
        }
    }
    
    return 0.0;
}

// 各种数学函数的实现
double func_sin(double* args, int arg_count) {
    return sin(args[0]);
}

double func_cos(double* args, int arg_count) {
    return cos(args[0]);
}

double func_tan(double* args, int arg_count) {
    return tan(args[0]);
}

double func_log(double* args, int arg_count) {
    if (args[0] <= 0) {
        return 0.0; // 错误情况，应该在调用前检查
    }
    return log10(args[0]);
}

double func_ln(double* args, int arg_count) {
    if (args[0] <= 0) {
        return 0.0; // 错误情况，应该在调用前检查
    }
    return log(args[0]);
}

double func_exp(double* args, int arg_count) {
    return exp(args[0]);
}

double func_sqrt(double* args, int arg_count) {
    if (args[0] < 0) {
        return 0.0; // 错误情况，应该在调用前检查
    }
    return sqrt(args[0]);
}

double func_abs(double* args, int arg_count) {
    return fabs(args[0]);
}