# QT C++17 计算器项目架构设计

## 1. 概述

本项目旨在构建一个跨平台、高性能、企业级可生产的桌面计算器应用程序，采用 Qt C++17 技术栈。应用支持三种模式（标准、科学、程序员），具备基本数学运算、函数运算、复杂数学表达式解析、自定义函数等核心功能，并扩展了矩阵运算、单位换算、历史记录、变量存储和插件系统等高级特性。

## 2. 设计目标

- **跨平台**: 支持 Windows、macOS、Linux 桌面环境。
- **高性能**: 表达式求值引擎采用优化算法，支持并行计算和缓存。
- **可维护性**: 模块化设计，遵循 SOLID 原则，便于扩展和测试。
- **用户体验**: 现代化自定义 UI（Qt Quick/QML），响应式交互。
- **企业级可生产性**: 包含完整的测试覆盖、CI/CD 流水线、文档和错误处理。

## 3. 高层次架构组件图

```mermaid
graph TB
    subgraph "UI Layer (Qt Quick/QML)"
        UI[QML UI]
        StandardView[标准模式视图]
        ScientificView[科学计算器视图]
        ProgrammerView[程序员模式视图]
        HistoryPanel[历史记录面板]
        VariablePanel[变量管理面板]
        SettingsDialog[设置对话框]
    end

    subgraph "Business Logic Layer"
        ExpressionParser[表达式解析器]
        ExpressionEvaluator[表达式求值器]
        FunctionManager[函数管理器]
        VariableManager[变量管理器]
        UnitConverter[单位换算器]
        MatrixEngine[矩阵运算引擎]
        HistoryManager[历史管理器]
        ModeController[模式控制器]
        PluginManager[插件管理器]
    end

    subgraph "Core Engine Layer"
        MathLibrary[高精度数学库]
        PluginSystem[插件系统接口]
        CacheManager[缓存管理器]
    end

    subgraph "Data Persistence Layer"
        SettingsStorage[设置存储]
        HistoryStorage[历史存储]
        PluginStorage[插件配置存储]
    end

    subgraph "Utilities Layer"
        Logger[日志系统]
        I18n[国际化]
        ThemeManager[主题管理器]
        ErrorHandler[错误处理器]
    end

    UI --> Business Logic Layer
    Business Logic Layer --> Core Engine Layer
    Business Logic Layer --> Data Persistence Layer
    Business Logic Layer --> Utilities Layer
```

## 4. 组件详细说明

### 4.1 UI 层
- **Qt Quick/QML**: 用于构建现代化、响应式用户界面。
- **视图分离**: 三种模式各自独立 QML 文件，通过模式控制器切换。
- **可重用组件**: 按钮网格、显示屏、历史列表等封装为自定义 QML 组件。
- **主题支持**: 通过 ThemeManager 动态切换亮色/暗色主题。

### 4.2 业务逻辑层
- **表达式解析器**: 将用户输入的字符串转换为抽象语法树 (AST)，支持运算符优先级、括号、函数调用。
- **表达式求值器**: 遍历 AST 执行计算，支持实数、复数、矩阵等多种数据类型。
- **函数管理器**: 管理内置函数 (sin, cos, log, sqrt) 和用户自定义函数。
- **变量管理器**: 提供变量的定义、赋值、引用和持久化存储。
- **单位换算器**: 实现物理单位之间的转换 (长度、质量、温度等)。
- **矩阵运算引擎**: 提供矩阵加法、乘法、求逆、行列式等操作。
- **历史管理器**: 记录每次计算表达式与结果，支持撤销/重做。
- **模式控制器**: 根据当前模式调整可用运算符、函数和显示格式。
- **插件管理器**: 加载、卸载第三方插件，扩展计算器功能。

### 4.3 核心引擎层
- **高精度数学库**: 集成 MPFR/GMP 库提供任意精度算术，确保科学计算的准确性。
- **插件系统接口**: 定义插件 API，允许第三方开发者贡献新功能。
- **缓存管理器**: 缓存常用表达式结果，提升重复计算性能。

### 4.4 数据持久层
- **设置存储**: 使用 QSettings 保存用户偏好 (窗口大小、主题、小数点位数)。
- **历史存储**: SQLite 数据库存储历史记录，支持查询和导出。
- **插件配置存储**: JSON 文件保存插件启用状态和配置。

### 4.5 工具层
- **日志系统**: 基于 spdlog 记录运行时信息，便于问题排查。
- **国际化**: 使用 Qt Linguist 实现多语言支持。
- **主题管理器**: 管理颜色、字体等视觉资源。
- **错误处理器**: 统一处理异常，提供友好的错误提示。

## 5. 模块结构

```
src/
├── ui/                          # QML 界面文件
│   ├── StandardView.qml
│   ├── ScientificView.qml
│   ├── ProgrammerView.qml
│   ├── components/              # 可重用 QML 组件
│   └── resources/               # 图片、字体等资源
├── core/                        # 核心引擎
│   ├── math/                    # 数学库封装
│   ├── expression/              # 表达式解析与求值
│   ├── function/                # 函数管理
│   ├── variable/                # 变量管理
│   ├── unit/                    # 单位换算
│   ├── matrix/                  # 矩阵运算
│   ├── history/                 # 历史管理
│   ├── plugin/                  # 插件系统
│   └── cache/                   # 缓存管理
├── data/                        # 数据持久化
│   ├── settings/
│   ├── history/
│   └── plugin/
├── utils/                       # 工具类
│   ├── logging/
│   ├── i18n/
│   ├── theme/
│   └── error/
└── app/                         # 应用入口与全局管理
    ├── main.cpp
    ├── Application.cpp
    └── ModeController.cpp
```

## 6. 构建系统与工具链

- **构建系统**: CMake (最低版本 3.16)
- **Qt 版本**: Qt 6.5 或更高 (Qt Quick 3.0)
- **C++ 标准**: C++17
- **依赖管理**: vcpkg 或 Conan 管理第三方库 (MPFR, GMP, SQLite, spdlog)
- **编译器**: MSVC (Windows), Clang (macOS), GCC (Linux)
- **CI/CD**: GitHub Actions 或 GitLab CI，自动化构建、测试和打包
- **包格式**: Windows (MSI/NSIS), macOS (.dmg), Linux (AppImage/DEB/RPM)

## 7. 表达式解析与求值引擎设计

### 7.1 解析流程
1. 词法分析：将输入字符串拆分为令牌 (数字、运算符、函数名、括号等)。
2. 语法分析：使用调度场算法 (Shunting‑yard) 将中缀表达式转换为后缀表达式 (RPN)。
3. 抽象语法树构建：从 RPN 生成 AST，便于后续优化和求值。

### 7.2 求值流程
1. 类型检查：确保操作数类型匹配 (标量、矩阵、单位量)。
2. 递归求值：深度优先遍历 AST，调用相应运算符或函数。
3. 精度控制：根据用户设置保留小数位数或使用科学计数法。
4. 结果缓存：将 AST 哈希后缓存结果，加速重复计算。

### 7.3 自定义函数支持
- 用户可通过 GUI 或脚本定义新函数 (例如 `f(x)=x^2+sin(x)`)。
- 函数定义存储在 SQLite 中，重启后仍然有效。

## 8. 三种模式设计

### 8.1 标准模式
- 基本四则运算、百分比、平方根、倒数。
- 界面简洁，适合日常计算。

### 8.2 科学计算器模式
- 三角函数、指数、对数、阶乘、常数 (π, e)。
- 支持复数、角度/弧度切换。
- 矩阵运算和单位换算入口。

### 8.3 程序员模式
- 二进制、八进制、十进制、十六进制转换。
- 位运算 (AND, OR, XOR, NOT, 移位)。
- 字节、字、双字显示。

## 9. 性能优化策略

- **表达式缓存**: 缓存最近使用的表达式及其结果，减少重复计算。
- **并行求值**: 对独立子表达式使用多线程求值。
- **内存池**: 为频繁创建的 AST 节点使用对象池，降低内存分配开销。
- **懒加载**: 插件和大型数学库按需加载。
- **编译器优化**: 启用 LTO、PGO 等优化选项。

## 10. 跨平台支持方案

- **平台抽象层**: 使用 Qt 原生 API 处理文件路径、系统对话框等差异。
- **条件编译**: 针对不同平台的特有功能使用 `#ifdef`。
- **测试矩阵**: CI 中涵盖三大操作系统，确保兼容性。

## 11. 测试策略与质量保证

- **单元测试**: Google Test 覆盖核心算法 (解析、求值、矩阵运算)。
- **集成测试**: 模拟用户操作，验证 UI 与逻辑交互。
- **性能测试**: 基准测试框架评估表达式求值速度。
- **静态分析**: Clang‑Tidy、Cppcheck 检查代码质量。
- **覆盖率**: 目标 90% 以上行覆盖。

## 12. 后续扩展方向

- **云端同步**: 用户历史与自定义函数跨设备同步。
- **脚本引擎**: 集成 Lua/Python 实现高级编程计算。
- **图形绘制**: 绘制函数图像、数据图表。
- **语音输入**: 支持语音识别输入表达式。
- **协作功能**: 多人实时共享计算会话。

---
*文档版本: 1.0*
*最后更新: 2026-01-12*