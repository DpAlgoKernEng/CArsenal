#include "ui.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>


int get_user_input(char *buffer, size_t size) {
    if (buffer == NULL || size == 0) {
        return INPUT_ERROR;
    }

    printf(">>> ");
    fflush(stdout);

    if (size > (size_t)INT_MAX) return INPUT_ERROR;

    if (fgets(buffer, (int)size, stdin) == NULL) {
        buffer[0] = '\0';
        if (feof(stdin)) return INPUT_EOF;
        return INPUT_ERROR;
    }

    size_t len = strlen(buffer);
    int truncated = 0;

    // 如果最后没有换行，说明输入被截断，需丢弃剩余字符
    if (size > 1 && len == size - 1 && buffer[len-1] != '\n') {
        int c;
        truncated = 1;
        while ((c = getchar()) != '\n' && c != EOF) { /* 丢弃 */ }
    }

    // 去除末尾换行或回车
    len = strlen(buffer);
    if (len > 0 && buffer[len-1] == '\n') {
        buffer[len-1] = '\0';
        len--;
    }

    // 去除首尾空白
    size_t start = 0;
    while (start < len && isspace((unsigned char)buffer[start])) start++;
    size_t end = len;
    while (end > start && isspace((unsigned char)buffer[end-1])) end--;

    if (start > 0 || end < len) {
        size_t newlen = end - start;
        memmove(buffer, buffer + start, newlen);
        buffer[newlen] = '\0';
    }

    return truncated ? INPUT_TRUNC : INPUT_OK;
}

char *get_user_input_dynamic(void) {
    printf(">>> ");
    fflush(stdout);

    char *line = NULL;
    size_t len = 0;
    ssize_t nread = getline(&line, &len, stdin);
    if (nread == -1) {
        free(line);
        return NULL; // EOF 或错误
    }

    // 去掉末尾换行
    if (nread > 0 && line[nread-1] == '\n') {
        line[nread-1] = '\0';
        nread--;
    }

    // 裁剪首尾空白
    size_t start = 0;
    while (start < (size_t)nread && isspace((unsigned char)line[start])) start++;
    size_t end = nread;
    while (end > start && isspace((unsigned char)line[end-1])) end--;

    size_t newlen = end - start;
    if (start > 0 || newlen < (size_t)nread) {
        memmove(line, line + start, newlen);
        line[newlen] = '\0';
        // 可选：realloc 缩小内存
        char *shrunk = realloc(line, newlen + 1);
        if (shrunk) line = shrunk;
    }

    return line;
}

char *custom_getline(char **lineptr, size_t *n) {
    // 参数有效性检查
    if (lineptr == NULL || n == NULL) {
        return NULL;
    }

    size_t size = *n;
    size_t len = 0;
    int c;
    char *buffer = *lineptr;

    // 初始分配或检查现有缓冲区
    if (buffer == NULL || size < INITIAL_SIZE) {
        size = INITIAL_SIZE;
        buffer = (char*)malloc(size * sizeof(char));
        if (buffer == NULL) {
            return NULL;  // 分配失败
        }
    }

    // 逐字符读取输入
    while ((c = getchar()) != EOF && c != '\n') {
        // 检查缓冲区空间
        if (len + 1 >= size) {
            size *= 2;  // 双倍扩展策略
            char *new_buffer = (char*)realloc(buffer, size);
            if (new_buffer == NULL) {
                free(buffer);
                return NULL;  // 扩展失败
            }
            buffer = new_buffer;
        }
        buffer[len++] = (char)c;
    }

    // 处理EOF且未读取字符的情况
    if (c == EOF && len == 0) {
        free(buffer);
        return NULL;
    }

    // 添加终止符
    buffer[len] = '\0';

    // 更新输出参数
    *lineptr = buffer;
    *n = size;

    return buffer;  // 返回成功读取的行（包含换行符或终止符）
}