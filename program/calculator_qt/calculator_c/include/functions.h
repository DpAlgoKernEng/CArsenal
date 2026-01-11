#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#define MAX_FUNCTIONS 20

// 函数指针类型定义
typedef double (*FunctionPtr)(double*, int);

// 函数结构
typedef struct {
    char name[10];
    FunctionPtr func;
    int min_args;
    int max_args;
} Function;

// 函数声明
int is_function(const char* name);
FunctionPtr get_function(const char* name);
int get_function_arg_count(const char* name);
double evaluate_function(const char* name, double* args, int arg_count);

// 各种数学函数声明
double func_sin(double* args, int arg_count);
double func_cos(double* args, int arg_count);
double func_tan(double* args, int arg_count);
double func_log(double* args, int arg_count);
double func_ln(double* args, int arg_count);
double func_exp(double* args, int arg_count);
double func_sqrt(double* args, int arg_count);
double func_abs(double* args, int arg_count);

#endif // FUNCTIONS_H