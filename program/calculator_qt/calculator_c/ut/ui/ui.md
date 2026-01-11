# UI 模块代码分析审计指南

## 1、列出目标文件

```bash
# 在工作区根目录运行
# filepath: /home/britghtl/code/tour_cpp
find ./calculator/calculator_c/ut/ui -type f \( -name "*.c" -o -name "*.h" \) -maxdepth 3 -print
```

## 2、静态分析（cppcheck & clang-tidy）

```bash
# 安装 cppcheck / clang-tidy 后运行
cppcheck --enable=all --inconclusive --std=c11 --suppress=missingIncludeSystem /home/britghtl/code/tour_cpp/calculator/calculator_c/ut/ui 2> ~/ui_cppcheck.txt

# 或（若有 compile_commands.json）
clang-tidy /home/britghtl/code/tour_cpp/calculator/calculator_c/ut/ui/*.c -- -I/home/britghtl/code/tour_cpp/include 2> ~/ui_clang_tidy.txt
```

## 3、动态检查（AddressSanitizer / UndefinedBehaviorSanitizer）

```bash
# 编译并运行你的测试/示例二进制（用你项目的编译命令替换下面）
gcc -g -O0 -fsanitize=address,undefined -fno-omit-frame-pointer $(find /home/britghtl/code/tour_cpp/calculator/calculator_c/ut/ui -name '*.c' -print) -I/home/britghtl/code/tour_cpp/include -o /tmp/ui_test || true
/tmp/ui_test 2>&1 | tee ~/ui_asan_run.txt
```

## 4、使用 valgrind 检查内存泄漏（若不是 ASan）

```bash
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./your_test_binary 2>&1 | tee ~/ui_valgrind.txt
```

## 5、快速人工检查清单（请针对每个 .c/.h 检查）

- malloc/realloc/free 对称性：malloc 后是否有对应 free；realloc 失败时是否使用旧指针导致泄漏。
- 文件句柄：fopen 后是否有 fclose；早期返回时是否关闭资源。
- 指针空值检查：函数返回指针/库调用后是否检查 NULL。
- 缓冲区边界：strcpy/strcat 是否被安全替代（strncpy/strlcpy/ snprintf）；数组越界与 off-by-one。
- 未初始化变量：局部变量在使用前是否赋值。
- 格式字符串安全：printf 家族是否使用外部字符串作为格式。
- 并发问题（若有多线程）：共享资源的同步。
- 整数溢出/下溢：索引/大小计算时检查边界。
- 资源泄漏（FILE*, DIR*, sockets, locks）。
- 错误处理路径：出错分支是否漏释放资源。
