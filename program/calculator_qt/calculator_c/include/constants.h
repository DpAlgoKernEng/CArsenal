#ifndef CONSTANTS_H
#define CONSTANTS_H

#define MAX_CONSTANTS 10

// 常量结构
typedef struct {
    char name[10];
    double value;
} Constant;

// 函数声明
int is_constant(const char* name);
double get_constant_value(const char* name);
int get_constants_count();
Constant* get_constant_at(int index);

#endif // CONSTANTS_H