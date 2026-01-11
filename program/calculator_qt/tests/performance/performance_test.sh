#!/bin/bash

# 性能测试脚本 - 测试C语言和C++版本的计算器性能

echo "==========================================="
echo "科学计算器性能测试"
echo "==========================================="

# 导入共享测试工具函数库
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/../common/test_utils.sh"

# 性能测试函数
run_performance_test() {
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
    temp_input="/tmp/calculator_test_input.txt"
    for ((i=1; i<=iterations; i++)); do
        echo "$input" >> "$temp_input"
    done
    echo "quit" >> "$temp_input"
    
    # 运行性能测试
    echo "正在测试 '$test_name' ($lang_version) - $iterations 次迭代..."
    
    # 使用time命令测量执行时间
    result=$( { time -p timeout 30s $exe_path < "$temp_input" > /dev/null; } 2>&1 )
    
    # 检查是否超时
    if [ $? -eq 124 ]; then
        echo -e "${RED}测试 '$test_name' ($lang_version) 失败: 超时${NC}"
        rm -f "$temp_input"
        return 1
    fi
    
    # 提取执行时间
    real_time=$(echo "$result" | grep "real" | awk '{print $2}')
    user_time=$(echo "$result" | grep "user" | awk '{print $2}')
    sys_time=$(echo "$result" | grep "sys" | awk '{print $2}')
    
    echo "  实际时间: ${real_time}s"
    echo "  用户时间: ${user_time}s"
    echo "  系统时间: ${sys_time}s"
    
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
echo "性能测试 - C语言版本"
echo "-------------------------------------------"

# 构建C语言版本
build_project_wrapper "c"

# 基准测试
run_performance_test "简单算术运算" "2 + 3 * 4" 100 "c"
run_performance_test "科学函数计算" "sin(pi/2)" 100 "c"
run_performance_test "复合表达式" "sqrt(2^2 + 3^2)" 100 "c"

# 压力测试
run_performance_test "深度嵌套表达式" "((((((2+3)*4)-5)/6)^2)+1)" 50 "c"
run_performance_test "大量计算任务" "2 + 3 * 4" 1000 "c"

# 测试C++版本
echo ""
echo "-------------------------------------------"
echo "性能测试 - C++语言版本"
echo "-------------------------------------------"

# 构建C++语言版本
build_project_wrapper "cpp"

# 基准测试
run_performance_test "简单算术运算" "2 + 3 * 4" 100 "cpp"
run_performance_test "科学函数计算" "sin(pi/2)" 100 "cpp"
run_performance_test "复合表达式" "sqrt(2^2 + 3^2)" 100 "cpp"

# 压力测试
run_performance_test "深度嵌套表达式" "((((((2+3)*4)-5)/6)^2)+1)" 50 "cpp"
run_performance_test "大量计算任务" "2 + 3 * 4" 1000 "cpp"

echo ""
echo "==========================================="
echo "性能测试完成"
echo "==========================================="