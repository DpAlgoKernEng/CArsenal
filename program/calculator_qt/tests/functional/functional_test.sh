#!/bin/bash

# 功能测试脚本 - 测试C语言和C++版本的计算器

echo "==========================================="
echo "科学计算器功能测试"
echo "==========================================="

# 导入共享测试工具函数库
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/../common/test_utils.sh"

# 测试计数器
total_tests=0
passed_tests=0
failed_tests=0

# 测试函数
run_test() {
    local test_name="$1"
    local input="$2"
    local expected="$3"
    local lang_version="$4"
    
    total_tests=$((total_tests + 1))
    
    # 获取可执行文件路径
    exe_path=$(get_executable_path "$lang_version" "$SCRIPT_DIR")
    
    # 检查可执行文件是否存在
    if ! check_executable "$lang_version" "$SCRIPT_DIR"; then
        failed_tests=$((failed_tests + 1))
        return 1
    fi
    
    # 运行测试
    result=$(echo "$input" | timeout 5s $exe_path 2>/dev/null | tail -n 1)
    
    # 检查是否超时
    if [ $? -eq 124 ]; then
        echo -e "${RED}测试 '$test_name' ($lang_version) 失败: 超时${NC}"
        failed_tests=$((failed_tests + 1))
        return 1
    fi
    
    # 检查结果
    if [[ "$result" =~ $expected ]]; then
        echo -e "${GREEN}测试 '$test_name' ($lang_version) 通过${NC}"
        passed_tests=$((passed_tests + 1))
        return 0
    else
        echo -e "${RED}测试 '$test_name' ($lang_version) 失败: 期望 '$expected', 得到 '$result'${NC}"
        failed_tests=$((failed_tests + 1))
        return 1
    fi
}

# 构建项目函数
build_project_wrapper() {
    local lang_version="$1"
    build_project "$lang_version" "$SCRIPT_DIR"
}

# 测试C语言版本
echo ""
echo "-------------------------------------------"
echo "测试C语言版本"
echo "-------------------------------------------"

# 构建C语言版本
build_project_wrapper "c"

# 基本算术运算测试
run_test "基本加法" "2 + 3" "5" "c"
run_test "乘法优先级" "2 + 3 * 4" "14" "c"
run_test "括号优先级" "(2 + 3) * 4" "20" "c"
run_test "减法和除法" "10 - 6 / 2" "7" "c"
run_test "幂运算" "2^3" "8" "c"

# 科学函数测试
run_test "正弦函数" "sin(0)" "0" "c"
run_test "余弦函数" "cos(0)" "1" "c"
run_test "正切函数" "tan(0)" "0" "c"
run_test "自然对数" "ln(e)" "1" "c"
run_test "常用对数" "log(10)" "1" "c"
run_test "平方根" "sqrt(16)" "4" "c"
run_test "绝对值" "abs(-5)" "5" "c"

# 常量测试
run_test "圆周率" "pi" "3.14" "c"
run_test "自然常数" "e" "2.71" "c"

# 复合表达式测试
run_test "复合表达式1" "sin(pi/2)" "1" "c"
run_test "复合表达式2" "2 * pi" "6.28" "c"
run_test "复合表达式3" "sqrt(2^2 + 3^2)" "3.60" "c"

# 错误处理测试
run_test "除零错误" "5 / 0" "错误" "c"
run_test "负数开方" "sqrt(-1)" "错误" "c"
run_test "对数负数" "log(-1)" "错误" "c"

# 新增测试用例
run_test "复杂表达式1" "(2 + 3) * (4 - 1) / (5 + 1)" "2.5" "c"
run_test "复杂表达式2" "2^(3+1) - sqrt(16) * 2" "8" "c"
run_test "三角函数组合" "sin(pi/2) + cos(0)" "2" "c"
run_test "指数和对数" "exp(ln(5))" "5" "c"
run_test "混合函数" "abs(-3) + sqrt(9) - 2^2" "2" "c"

# 测试C++版本
echo ""
echo "-------------------------------------------"
echo "测试C++语言版本"
echo "-------------------------------------------"

# 构建C++语言版本
build_project_wrapper "cpp"

# 基本算术运算测试
run_test "基本加法" "2 + 3" "5" "cpp"
run_test "乘法优先级" "2 + 3 * 4" "14" "cpp"
run_test "括号优先级" "(2 + 3) * 4" "20" "cpp"
run_test "减法和除法" "10 - 6 / 2" "7" "cpp"
run_test "幂运算" "2^3" "8" "cpp"

# 科学函数测试
run_test "正弦函数" "sin(0)" "0" "cpp"
run_test "余弦函数" "cos(0)" "1" "cpp"
run_test "正切函数" "tan(0)" "0" "cpp"
run_test "自然对数" "ln(e)" "1" "cpp"
run_test "常用对数" "log(10)" "1" "cpp"
run_test "平方根" "sqrt(16)" "4" "cpp"
run_test "绝对值" "abs(-5)" "5" "cpp"

# 常量测试
run_test "圆周率" "pi" "3.14" "cpp"
run_test "自然常数" "e" "2.71" "cpp"

# 复合表达式测试
run_test "复合表达式1" "sin(pi/2)" "1" "cpp"
run_test "复合表达式2" "2 * pi" "6.28" "cpp"
run_test "复合表达式3" "sqrt(2^2 + 3^2)" "3.60" "cpp"

# 错误处理测试
run_test "除零错误" "5 / 0" "错误" "cpp"
run_test "负数开方" "sqrt(-1)" "错误" "cpp"
run_test "对数负数" "log(-1)" "错误" "cpp"

# 新增测试用例
run_test "复杂表达式1" "(2 + 3) * (4 - 1) / (5 + 1)" "2.5" "cpp"
run_test "复杂表达式2" "2^(3+1) - sqrt(16) * 2" "8" "cpp"
run_test "三角函数组合" "sin(pi/2) + cos(0)" "2" "cpp"
run_test "指数和对数" "exp(ln(5))" "5" "cpp"
run_test "混合函数" "abs(-3) + sqrt(9) - 2^2" "2" "cpp"

# 输出测试结果摘要
echo ""
echo "==========================================="
echo "测试结果摘要"
echo "==========================================="
echo "总测试数: $total_tests"
echo -e "${GREEN}通过测试: $passed_tests${NC}"
echo -e "${RED}失败测试: $failed_tests${NC}"

if [ $failed_tests -eq 0 ]; then
    echo -e "${GREEN}所有测试通过!${NC}"
    exit 0
else
    echo -e "${RED}有 $failed_tests 个测试失败${NC}"
    exit 1
fi