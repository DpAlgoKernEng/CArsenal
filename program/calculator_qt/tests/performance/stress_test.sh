#!/bin/bash

# 压力测试脚本 - 对C语言和C++版本的计算器进行高负载测试

echo "==========================================="
echo "科学计算器压力测试"
echo "==========================================="

# 导入共享测试工具函数库
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/../common/test_utils.sh"

# 压力测试函数
run_stress_test() {
    local test_name="$1"
    local input="$2"
    local iterations="$3"
    local lang_version="$4"
    
    # 根据语言版本选择可执行文件
    if [ "$lang_version" = "c" ]; then
        exe_path="../../calculator_c/build_cmake/scientific_calculator"
    else
        exe_path="../../calculator_cpp/build_cmake/scientific_calculator"
    fi
    
    # 检查可执行文件是否存在
    if [ ! -f "$exe_path" ]; then
        echo -e "${RED}错误: 找不到可执行文件 $exe_path${NC}"
        return 1
    fi
    
    # 创建临时输入文件
    temp_input="/tmp/calculator_stress_input.txt"
    for ((i=1; i<=iterations; i++)); do
        echo "$input" >> "$temp_input"
    done
    echo "quit" >> "$temp_input"
    
    # 运行压力测试
    echo "正在运行压力测试 '$test_name' ($lang_version) - $iterations 次迭代..."
    
    # 记录开始时间
    start_time=$(date +%s)
    
    # 运行测试并重定向输出到/dev/null以提高性能
    timeout 60s $exe_path < "$temp_input" > /dev/null 2>&1
    test_result=$?
    
    # 记录结束时间
    end_time=$(date +%s)
    duration=$((end_time - start_time))
    
    # 检查是否超时
    if [ $test_result -eq 124 ]; then
        echo -e "${RED}压力测试 '$test_name' ($lang_version) 失败: 超时${NC}"
        rm -f "$temp_input"
        return 1
    fi
    
    # 检查是否有其他错误
    if [ $test_result -ne 0 ]; then
        echo -e "${RED}压力测试 '$test_name' ($lang_version) 失败: 程序异常退出 (退出码: $test_result)${NC}"
        rm -f "$temp_input"
        return 1
    fi
    
    echo -e "${GREEN}压力测试 '$test_name' ($lang_version) 通过${NC}"
    echo "  执行时间: ${duration}秒"
    echo "  平均每次计算: $(echo "scale=6; $duration / $iterations" | bc -l)秒"
    
    # 清理临时文件
    rm -f "$temp_input"
    
    return 0
}

# 构建项目函数
build_project_wrapper() {
    local lang_version="$1"
    build_project "$lang_version" "$SCRIPT_DIR"
}

# 测试C语言版本
echo ""
echo "-------------------------------------------"
echo "压力测试 - C语言版本"
echo "-------------------------------------------"

# 构建C语言版本
build_project_wrapper "c"

# 高负载测试
run_stress_test "大量简单计算" "2 + 3 * 4" 5000 "c"
run_stress_test "大量科学函数计算" "sin(pi/2) + cos(pi/4)" 2000 "c"
run_stress_test "深度嵌套表达式" "((((((2+3)*4)-5)/6)^2)+1)" 1000 "c"

# 长时间运行测试
echo ""
echo "正在进行长时间运行测试..."
run_stress_test "长时间运行测试" "sqrt(2^2 + 3^2) * sin(pi/4)" 10000 "c"

# 测试C++版本
echo ""
echo "-------------------------------------------"
echo "压力测试 - C++语言版本"
echo "-------------------------------------------"

# 构建C++语言版本
build_project_wrapper "cpp"

# 高负载测试
run_stress_test "大量简单计算" "2 + 3 * 4" 5000 "cpp"
run_stress_test "大量科学函数计算" "sin(pi/2) + cos(pi/4)" 2000 "cpp"
run_stress_test "深度嵌套表达式" "((((((2+3)*4)-5)/6)^2)+1)" 1000 "cpp"

# 长时间运行测试
echo ""
echo "正在进行长时间运行测试..."
run_stress_test "长时间运行测试" "sqrt(2^2 + 3^2) * sin(pi/4)" 10000 "cpp"

echo ""
echo "==========================================="
echo "压力测试完成"
echo "==========================================="