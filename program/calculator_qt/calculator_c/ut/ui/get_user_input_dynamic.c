// 单元测试：针对 char *get_user_input_dynamic(void) 的行为（getline 版）
#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "ui.h" /* 期望声明 get_user_input_dynamic() */

int main(void) {
#ifdef __linux__
#else
    /* getline/fmemopen 在非 GNU 平台可能不可用，此处仍尝试编译运行 */
#endif

#ifdef _MSC_VER
    fprintf(stderr, "跳过动态输入测试: Windows/MSVC 未启用 getline/fmemopen\n");
    return 2;
#endif

    /* 检查函数存在（弱方式：仅编译时依赖声明） */
    printf("开始动态读取单元测试...\n");

    /* 1) 普通输入 + 去首尾空白 */
    {
        char data[] = "   sin(pi/2)   \n";
        FILE *saved = stdin;
        FILE *mem = fmemopen(data, strlen(data) + 1, "r");
        stdin = mem;

        char *s = get_user_input_dynamic();
        fclose(mem);
        stdin = saved;

        if (s == NULL) {
            fprintf(stderr, "测试失败: get_user_input_dynamic 返回 NULL 期望字符串 \n");
            return 1;
        }
        if (strcmp(s, "sin(pi/2)") != 0) {
            fprintf(stderr, "测试失败：期望 'sin(pi/2)'，实际 '%s'\n", s);
            free(s);
            return 1;
        }
        free(s);
    }

    /* 2) EOF 情况 */
    {
        FILE *saved = stdin;
        FILE *mem = fopen("/dev/null", "r");
        stdin = mem;

        char *s = get_user_input_dynamic();
        fclose(mem);
        stdin = saved;

        if (s != NULL) {
            fprintf(stderr, "测试失败: EOF 情况期望返回 NULL, 实际 '%s'\n", s);
            free(s);
            return 1;
        }
    }

    printf("动态读取单元测试通过\n");
    return 0;
}