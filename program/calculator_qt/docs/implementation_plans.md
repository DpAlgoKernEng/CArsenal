# QT C++17 计算器 - 子模块详细实施计划

## 1. 项目骨架与构建系统

### 目标
创建可编译的 Qt C++17 项目基础结构，配置 CMake 和依赖。

### 实施步骤
1. **创建目录结构**
   ```
   src/
   ├── ui/
   ├── core/
   ├── data/
   ├── utils/
   └── app/
   tests/
   resources/
   cmake/
   ```

2. **编写根 CMakeLists.txt**
   - 设置 CMake 最低版本 3.16
   - 定义项目名称、版本、C++17 标准
   - 查找 Qt 6.5 组件 (Core Quick Widgets)
   - 配置编译选项 (Warnings as errors, LTO 等)

3. **编写 src/CMakeLists.txt**
   - 添加子目录 (ui, core, data, utils, app)
   - 定义目标依赖关系

4. **编写依赖配置**
   - 使用 vcpkg 或 Conan 管理第三方库 (MPFR, GMP, SQLite, spdlog)
   - 创建 FindMPFR.cmake 等查找模块

5. **创建基础文件**
   - `src/app/main.cpp` - 简单 Qt 应用入口
   - `src/ui/main.qml` - 空 QML 文件
   - `resources/qml.qrc` - 资源文件

### 验收标准
- 项目能在三大平台 (Windows/macOS/Linux) 上成功配置和编译
- 运行后显示一个空白 Qt Quick 窗口

## 2. 数学表达式解析引擎

### 目标
实现词法分析、语法分析，将字符串表达式转换为 AST。

### 类设计
```cpp
// Token 类型
enum class TokenType {
    Number, Identifier, Operator, LeftParen, RightParen, Comma, End
};

struct Token {
    TokenType type;
    std::string value;
    double numericValue; // 如果是数字
};

// AST 节点类型
enum class NodeType {
    Number, Variable, BinaryOp, UnaryOp, FunctionCall
};

class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual double evaluate() const = 0;
    virtual std::string toString() const = 0;
};

class NumberNode : public ASTNode { ... };
class BinaryOpNode : public ASTNode { ... };
class FunctionCallNode : public ASTNode { ... };
```

### 实施步骤
1. **词法分析器 (Lexer)**
   - 实现 `Lexer` 类，输入字符串，输出 Token 流
   - 支持数字 (整数、小数、科学计数法)、标识符、运算符

2. **语法分析器 (Parser)**
   - 实现 `Parser` 类，使用调度场算法 (Shunting-yard)
   - 处理运算符优先级 (+, -, *, /, ^)
   - 支持括号和函数调用 (sin(pi/2))

3. **AST 构建**
   - 将 RPN 转换为 AST 节点树
   - 实现节点工厂模式

### 验收标准
- 能正确解析 "2+3*(4-1)" 并生成 AST
- 能处理函数调用 "sin(pi/2)"
- 单元测试覆盖所有运算符和边界情况

## 3. 表达式求值引擎

### 目标
遍历 AST 执行计算，返回数值结果。

### 实施步骤
1. **实现 ASTNode::evaluate()**
   - `NumberNode`: 返回存储的数值
   - `BinaryOpNode`: 递归求值左右子树，应用运算符
   - `FunctionCallNode`: 调用函数表计算

2. **创建函数表**
   - 内置函数映射: `sin`, `cos`, `tan`, `log`, `exp`, `sqrt`
   - 使用 `std::unordered_map<std::string, std::function<double(double)>>`

3. **错误处理**
   - 除零错误、未定义函数、无效参数
   - 抛出异常或返回 `std::optional<double>`

4. **精度控制**
   - 配置输出小数位数
   - 使用 `std::setprecision`

### 验收标准
- 计算 "2+3*4" 得到 14
- 计算 "sin(0)" 得到 0
- 处理错误输入 "1/0" 返回适当错误信息

## 4. 标准模式 UI (Qt Quick/QML)

### 目标
创建标准计算器界面，支持按钮输入和结果显示。

### QML 组件设计
```qml
// CalculatorButton.qml
Rectangle {
    property string text
    signal clicked
    // 样式和点击处理
}

// Display.qml
Text {
    property string expression
    property string result
    // 显示当前输入和结果
}

// StandardView.qml
Grid {
    // 4x5 按钮网格
    // 数字 0-9, 运算符 + - * /, 等号, 清除
}
```

### 实施步骤
1. **创建 QML 组件**
   - `CalculatorButton.qml` - 可重用按钮
   - `Display.qml` - 显示屏
   - `StandardView.qml` - 主界面

2. **实现 C++ 后端绑定**
   - 创建 `CalculatorEngine` QObject 类，暴露给 QML
   - 槽函数: `inputDigit`, `inputOperator`, `calculate`, `clear`

3. **连接 UI 与引擎**
   - 按钮点击调用 `CalculatorEngine` 方法
   - 引擎计算后更新显示

4. **样式设计**
   - 使用 Qt Quick Controls 2 或自定义样式
   - 响应式布局，适应窗口大小

### 验收标准
- 界面美观，按钮布局合理
- 能通过按钮输入表达式并显示结果
- 支持连续计算和清除功能

## 5. 应用入口与模式控制器

### 目标
实现应用主窗口和模式切换功能。

### 类设计
```cpp
class Application : public QObject {
    Q_OBJECT
public:
    enum class Mode { Standard, Scientific, Programmer };
    
    Mode currentMode() const;
    void switchMode(Mode mode);
    
private:
    Mode m_mode;
    CalculatorEngine* m_engine;
};

class ModeController : public QObject {
    Q_OBJECT
    Q_PROPERTY(Mode currentMode READ currentMode NOTIFY modeChanged)
    // ...
};
```

### 实施步骤
1. **完善 main.cpp**
   - 注册 QML 类型 (`CalculatorEngine`, `ModeController`)
   - 加载主 QML 文件

2. **实现模式控制器**
   - 管理当前模式状态
   - 提供 QML 属性绑定

3. **创建主 QML 文件**
   - 使用 `StackView` 或 `Loader` 切换不同模式视图
   - 添加模式选择按钮或菜单

4. **添加应用配置**
   - 窗口大小、位置持久化
   - 应用图标和标题

### 验收标准
- 应用启动显示标准模式
- 能通过 UI 切换到科学模式（即使科学模式尚未实现）
- 窗口关闭后重新打开保持上次大小和位置

## 6. 科学计算器模式

### 目标
扩展表达式引擎和 UI，支持科学计算功能。

### 实施步骤
1. **扩展表达式引擎**
   - 添加更多内置函数: `asin`, `acos`, `atan`, `log10`, `pow`, `factorial`
   - 支持常数: `pi`, `e`
   - 支持角度/弧度模式切换

2. **创建科学模式 QML 界面**
   - 扩展按钮网格，添加科学函数按钮
   - 添加角度/弧度切换按钮
   - 添加矩阵和单位换算入口按钮

3. **实现复数支持**
   - 扩展 `ASTNode` 支持复数运算
   - 添加 `i` 常数和复数运算符

4. **UI 优化**
   - 使用选项卡或折叠面板组织大量按钮
   - 提供函数说明提示

### 验收标准
- 能计算 "sin(pi/2)" 得到 1
- 能计算 "2^10" 得到 1024
- 角度/弧度切换正确影响三角函数结果

## 7. 程序员模式

### 目标
实现进制转换和位运算功能。

### 实施步骤
1. **扩展表达式引擎**
   - 添加位运算符: `&`, `|`, `^`, `~`, `<<`, `>>`
   - 支持不同进制数字解析 (0b1010, 0o777, 0xFF)

2. **创建程序员模式 QML 界面**
   - 添加进制选择按钮 (Bin, Oct, Dec, Hex)
   - 添加位运算按钮
   - 显示当前值的不同进制表示

3. **实现进制转换**
   - `QString::number(value, base)` 进行转换
   - 实时更新所有进制显示

4. **位运算可视化**
   - 显示二进制位图
   - 支持位翻转动画

### 验收标准
- 能计算 "0b1010 & 0b1100" 得到 0b1000
- 十进制 255 能正确显示为 FF (十六进制)
- 支持大数位运算 (64 位)

## 8. 历史记录系统

### 目标
记录计算历史，支持查看和重用。

### 实施步骤
1. **设计数据库表**
   ```sql
   CREATE TABLE history (
       id INTEGER PRIMARY KEY,
       timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,
       expression TEXT NOT NULL,
       result TEXT NOT NULL,
       mode INTEGER
   );
   ```

2. **实现 HistoryManager 类**
   - 使用 SQLite 数据库
   - 方法: `addEntry`, `getEntries`, `clearHistory`, `exportToCSV`

3. **创建历史面板 QML 组件**
   - `ListView` 显示历史条目
   - 点击条目可重新输入表达式
   - 支持删除和导出

4. **集成到主界面**
   - 添加历史面板切换按钮
   - 每次计算后自动记录

### 验收标准
- 计算记录自动保存到数据库
- 历史面板能显示、筛选、删除记录
- 点击历史条目能重新计算

## 9. 变量管理系统

### 目标
支持变量定义和在表达式中使用。

### 实施步骤
1. **实现 VariableManager 类**
   - `std::unordered_map<std::string, double>` 存储变量
   - 方法: `define`, `get`, `list`, `saveToFile`, `loadFromFile`

2. **扩展表达式解析器**
   - 标识符解析为变量引用
   - 支持赋值表达式 "x = 5"

3. **创建变量面板 QML 组件**
   - 显示当前定义的所有变量
   - 支持修改变量值
   - 变量导入/导出

4. **集成到表达式引擎**
   - 求值时替换变量值为实际数值
   - 处理未定义变量错误

### 验收标准
- 能定义变量 "x = 10"，然后在表达式中使用 "x*2" 得到 20
- 变量持久化，重启应用后仍然存在
- 变量面板能显示和修改变量

## 10. 单位换算器

### 目标
实现物理单位之间的转换。

### 实施步骤
1. **设计单位数据库**
   ```cpp
   struct Unit {
       std::string name;
       std::string category; // "length", "mass", "temperature"
       double toBaseFactor;  // 转换为基准单位的因子
       double fromBaseOffset; // 温度等需要偏移
   };
   ```

2. **实现 UnitConverter 类**
   - 加载预定义单位 (米、英尺、千克、磅、摄氏度、华氏度)
   - 方法: `convert(value, fromUnit, toUnit)`

3. **创建单位换算 QML 界面**
   - 类别选择下拉框
   - 源单位和目标单位选择
   - 实时转换显示

4. **集成到科学模式**
   - 在表达式引擎中支持单位语法 "5m to ft"
   - 添加单位换算按钮

### 验收标准
- 能正确转换 "5 meters to feet" 得到约 16.404 英尺
- 支持温度转换 (摄氏度↔华氏度)
- 单位数据库可扩展

## 11. 矩阵运算引擎

### 目标
支持矩阵创建和基本运算。

### 实施步骤
1. **实现 Matrix 类**
   - 使用 `std::vector<std::vector<double>>` 存储数据
   - 方法: `rows`, `cols`, `at`, `determinant`, `inverse`, `transpose`

2. **扩展表达式引擎支持矩阵**
   - 添加矩阵字面量语法 "[[1,2],[3,4]]"
   - 支持矩阵运算符 "+", "-", "*" (矩阵乘法)
   - 添加矩阵函数 "det()", "inv()", "transpose()"

3. **创建矩阵输入 QML 界面**
   - 矩阵编辑器 (表格形式)
   - 可视化矩阵运算结果

4. **性能优化**
   - 使用 Eigen 库作为可选后端
   - 缓存矩阵运算结果

### 验收标准
- 能计算 "[[1,2],[3,4]] * [[5,6],[7,8]]" 得到正确结果
- 能计算 3x3 矩阵的行列式
- 支持矩阵求逆和转置

## 12. 自定义函数支持

### 目标
允许用户定义自己的函数。

### 实施步骤
1. **设计函数定义格式**
   ```cpp
   struct UserFunction {
       std::string name;
       std::vector<std::string> parameters;
       std::string expression; // 函数体表达式
   };
   ```

2. **实现 FunctionManager 类**
   - 存储用户函数
   - 方法: `define`, `call`, `list`, `save`, `load`

3. **扩展表达式引擎**
   - 解析函数定义 "f(x) = x^2 + sin(x)"
   - 调用时替换参数并求值函数体

4. **创建函数管理 QML 界面**
   - 函数定义对话框
   - 函数列表和编辑功能

### 验收标准
- 能定义函数 "f(x)=x^2"，然后调用 "f(5)" 得到 25
- 支持多参数函数 "g(x,y)=x+y"
- 函数定义持久化存储

## 13. 插件系统框架

### 目标
提供插件 API，允许第三方扩展。

### 实施步骤
1. **设计插件接口**
   ```cpp
   class ICalculatorPlugin {
   public:
       virtual ~ICalculatorPlugin() = default;
       virtual QString name() const = 0;
       virtual void initialize(CalculatorEngine* engine) = 0;
       virtual QObject* createUI() = 0;
   };
   ```

2. **实现 PluginManager 类**
   - 动态加载共享库 (.dll/.so/.dylib)
   - 插件发现、加载、卸载
   - 插件间通信机制

3. **创建插件示例**
   - 统计计算插件 (平均值、标准差)
   - 绘图插件 (函数图像)
   - 单位数据库扩展插件

4. **插件配置界面**
   - 插件启用/禁用
   - 插件设置对话框

### 验收标准
- 能加载示例插件并在 UI 中显示其功能
- 插件崩溃不影响主应用
- 提供完整的插件开发文档

## 14. 高精度数学库集成

### 目标
使用 MPFR/GMP 提供任意精度计算。

### 实施步骤
1. **封装 MPFR 类型**
   ```cpp
   class HighPrecisionNumber {
   private:
       mpfr_t value;
       // ...
   };
   ```

2. **修改表达式引擎**
   - 可选使用高精度数字类型
   - 配置计算精度 (小数位数)

3. **性能权衡**
   - 默认使用 double 快速计算
   - 需要高精度时切换到 MPFR

4. **UI 精度设置**
   - 添加精度设置对话框
   - 实时切换精度模式

### 验收标准
- 能计算 1/3 到 100 位小数
- 高精度模式与普通模式结果一致
- 性能下降在可接受范围内

## 15. 缓存管理器

### 目标
缓存表达式结果，提升重复计算性能。

### 实施步骤
1. **设计缓存键**
   - 使用表达式字符串的哈希
   - 考虑变量值和精度设置

2. **实现 CacheManager 类**
   - LRU 缓存策略
   - 可配置缓存大小
   - 统计命中率

3. **集成到表达式引擎**
   - 求值前检查缓存
   - 求值后存储结果

4. **缓存监控界面**
   - 显示缓存命中率
   - 手动清除缓存

### 验收标准
- 重复计算相同表达式时命中缓存
- 缓存大小限制有效
- 变量变化后缓存自动失效

## 16. 主题管理器与国际化

### 目标
支持主题切换和多语言。

### 实施步骤
1. **实现 ThemeManager 类**
   - 定义颜色方案 (亮色、暗色、自定义)
   - QML 属性绑定主题颜色

2. **国际化支持**
   - 使用 Qt Linguist 翻译所有字符串
   - 支持动态语言切换

3. **创建设置对话框**
   - 主题选择
   - 语言选择
   - 其他外观设置

4. **资源管理**
   - 主题相关图片资源
   - 字体资源

### 验收标准
- 能切换亮色/暗色主题
- 能切换中英文界面
- 所有 UI 元素正确应用主题

## 17. 错误处理与日志系统

### 目标
统一的错误处理和日志记录。

### 实施步骤
1. **集成 spdlog**
   - 配置文件和控制台输出
   - 不同日志级别 (debug, info, warning, error)

2. **实现 ErrorHandler 类**
   - 统一异常捕获
   - 用户友好的错误提示
   - 错误代码和帮助链接

3. **日志界面**
   - 内置日志查看器
   - 日志过滤和搜索

4. **崩溃报告**
   - 生成崩溃转储
   - 错误报告对话框

### 验收标准
- 所有错误都有日志记录
- 用户看到友好的错误提示而不是崩溃
- 日志文件可帮助调试问题

## 18. 性能优化

### 目标
提升表达式求值和 UI 响应速度。

### 实施步骤
1. **并行求值**
   - 识别独立子表达式
   - 使用 `std::async` 并行计算

2. **内存池**
   - AST 节点对象池
   - 减少动态内存分配

3. **编译器优化**
   - CMake 配置 LTO (链接时优化)
   - PGO (配置文件引导优化) 构建

4. **性能分析**
   - 集成性能测试
   - 瓶颈识别和优化

### 验收标准
- 复杂表达式求值速度提升 30% 以上
- 内存使用量减少
- UI 响应流畅

## 实施时间表

| 阶段 | 模块 | 预估工时 | 累计工时 |
|------|------|----------|----------|
| 第一阶段 | 1-5 | 26 小时 | 26 小时 |
| 第二阶段 | 6-9 | 30 小时 | 56 小时 |
| 第三阶段 | 10-13 | 38 小时 | 94 小时 |
| 第四阶段 | 14-18 | 28 小时 | 122 小时 |

**总计**: 约 122 小时 (3 周全职开发)

## 质量保证

每个模块实施后必须:
1. 通过单元测试 (Google Test)
2. 通过集成测试 (模拟用户操作)
3. 代码审查 (遵循编码规范)
4. 性能基准测试
5. 跨平台验证

---
*文档版本: 1.0*
*最后更新: 2026-01-12*