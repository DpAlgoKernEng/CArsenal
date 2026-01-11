# CArsenal 项目开发指南

本文档为AI智能编码代理提供在CArsenal项目中工作的完整指南。

## 构建系统

### 环境要求

- CMake 3.22+, vcpkg, Clang/GCC支持C++17和C11

### 构建命令

```bash
# 完整构建流程
cd /root/code/CArsenal
cmake -B build_cmake -S . \
  -DCMAKE_TOOLCHAIN_FILE=third/lib_manager/vcpkg/scripts/buildsystems/vcpkg.cmake \
  -DCMAKE_BUILD_TYPE=Debug
cmake --build build_cmake -j$(nproc)

# 运行主程序
./build_cmake/carsenal --help

# 清理构建
rm -rf build_cmake
```

### 构建配置

- `-DCMAKE_BUILD_TYPE`: Debug | Release | RelWithDebInfo | MinSizeRel
- `-DCMAKE_TOOLCHAIN_FILE`: 必须指定vcpkg工具链文件

## 测试系统

### 测试结构

```
test/
├── benchmark/     # 性能测试
├── example/       # 示例测试
└── integration/   # 集成测试
```

### 运行测试

```bash
# 编译运行单个测试文件
cd /root/code/CArsenal
gcc -o test_example test/example/carsenal/CArsenal.cpp \
  -Iinclude -Ibuild_cmake/vcpkg_installed/x64-linux/include
./test_example

# 运行主程序测试
./build_cmake/carsenal --name Test --verbose
```

## 代码风格规范

### 格式化与静态分析

```bash
# 格式化代码
clang-format -i path/to/file.cpp

# 格式化整个项目
find src library program -name "*.cpp" -o -name "*.c" -o -name "*.h" -o -name "*.hpp" | xargs clang-format -i

# 静态分析
clang-tidy -p build_cmake/compile_commands.json path/to/file.cpp
```

### 命名约定

1. **文件命名**: 小写加下划线 `colorfmt.c`, `cmdline_parser.h`
2. **函数命名**: 
   - C函数: 小写，单词间可加下划线 `printcolor()`
   - C++函数: 小写加下划线 `calculate_total()`
3. **变量命名**:
   - 局部变量: 小写加下划线 `user_name`
   - 成员变量: 小写加下划线，可选 `m_` 前缀 `m_data_size`
   - 常量: 全大写加下划线 `COLOR_RED`, `MAX_BUFFER_SIZE`
4. **类型命名**:
   - 结构体/类: 大驼峰 `ColorFormatter`, `CommandParser`
   - 枚举: 大驼峰 `ColorType`, 枚举值全大写 `COLOR_RED`
   - 类型别名: 小写加下划线 `error_code_t`, `buffer_ptr`
5. **宏命名**: 全大写加下划线 `PRINT_RED()`, `MAX_SIZE`

### 代码格式化规则

- **缩进**: 4空格，禁用Tab
- **指针**: 靠左 `int* ptr`
- **大括号**: Stroustrup风格（类/函数换行，控制语句不换行）
- **行宽**: 80字符
- **头文件顺序**: 相关头文件 → C系统头 → C++系统头 → 第三方库 → 项目其他头文件

### 类型与错误处理

**C代码**:

```c
#include <stdint.h>
#include <stdbool.h>

int32_t process_data(const uint8_t* buffer, size_t buffer_size);
bool validate_input(const char* input);

// 错误处理：返回错误码
typedef enum {
    ERROR_NONE = 0,
    ERROR_INVALID_ARG,
    ERROR_MEMORY,
    ERROR_IO
} error_code_t;
```

**C++代码**:

```cpp
#include <cstdint>
#include <memory>
#include <string_view>

// RAII资源管理
class FileHandler {
public:
    explicit FileHandler(const std::string& path);
    ~FileHandler();
    FileHandler(const FileHandler&) = delete;
    FileHandler& operator=(const FileHandler&) = delete;
    FileHandler(FileHandler&&) noexcept;
    FileHandler& operator=(FileHandler&&) noexcept;
};

// 错误处理：异常或std::optional
std::optional<int> safe_divide(int a, int b);
```

**Boost集成**:

```cpp
namespace po = boost::program_options;
namespace logging = boost::log;

BOOST_LOG_TRIVIAL(info) << "Operation completed";
BOOST_LOG_TRIVIAL(error) << "Failed to open file: " << filename;
```

### 注释规范

- **文件头注释**: Doxygen风格，包含@file, @brief, @author, @date, @version
- **函数注释**: Doxygen风格，说明参数、返回值、注意事项
- **行内注释**: 解释重要算法步骤或复杂逻辑

## 开发工作流

### 为新功能工作

1. 创建TODO列表
2. 检查现有代码模式
3. 实现功能（遵循代码规范）
4. 运行格式化 `clang-format`
5. 静态检查 `clang-tidy`
6. 验证构建通过
7. 测试功能

### 修改现有代码

1. 理解上下文
2. 保持风格一致
3. 避免破坏性更改
4. 更新相关文档

## 代理工具使用

### 常用代理

1. **explore代理**: 代码库模式搜索
2. **librarian代理**: 外部文档和示例搜索
3. **oracle代理**: 架构决策和复杂问题咨询
4. **frontend-ui-ux-engineer代理**: UI/UX相关工作
5. **document-writer代理**: 文档编写

### 工具调用规范

- 并行执行非依赖任务
- 提供清晰的需求和上下文
- 验证代理输出是否符合预期

## 注意事项

### 禁止行为

- 忽略编译器警告
- 破坏现有接口（除非明确要求）
- 提交未格式化的代码
- 忽略静态分析错误

### 必须行为

- 遵循现有代码风格和模式
- 添加适当的注释和文档
- 验证构建通过且无警告
- 更新相关文档
- 使用TODO列表跟踪复杂任务

---

*本文档最后更新: 2025年1月11日*
*维护者: CArsenal项目组*
*文档版本: 1.0*

## Active Technologies
- C++17 or later (modern C++ standards) + Standard library only; project has existing CLI11/gflags but new implementation will be independent (001-cmdline-parser)
- N/A (in-memory parsing only) (001-cmdline-parser)

## Recent Changes
- 001-cmdline-parser: Added C++17 or later (modern C++ standards) + Standard library only; project has existing CLI11/gflags but new implementation will be independent
