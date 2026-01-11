#include "constants.h"
#include <math.h>
#include <string.h>

// 定义常量数组
static Constant constants[] = {
    {"pi", M_PI},
    {"e", M_E}
};

// 常量数量
static int constants_count = sizeof(constants) / sizeof(Constant);

int is_constant(const char* name) {
    if (name == NULL) {
        return 0;
    }
    
    for (int i = 0; i < constants_count; i++) {
        if (strcmp(constants[i].name, name) == 0) {
            return 1;
        }
    }
    
    return 0;
}

double get_constant_value(const char* name) {
    if (name == NULL) {
        return 0.0;
    }
    
    for (int i = 0; i < constants_count; i++) {
        if (strcmp(constants[i].name, name) == 0) {
            return constants[i].value;
        }
    }
    
    return 0.0;
}

int get_constants_count() {
    return constants_count;
}

Constant* get_constant_at(int index) {
    if (index < 0 || index >= constants_count) {
        return NULL;
    }
    
    return &constants[index];
}