#!/bin/bash

# Valgrind测试脚本 - 使用Valgrind工具深入分析C语言和C++版本的计算器

echo "==========================================="
echo "科学计算器Valgrind测试"
echo "==========================================="

# 导入共享测试工具函数库
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/../common/test_utils.sh"

# 检查是否安装了Valgrind
check_valgrind() {
    if ! command -v valgrind &> /dev/null; then
        echo -e "${RED}错误: 未找到 Valgrind 工具，请先安装 Valgrind${NC}"
        return 1
    fi
    return 0
}

# Valgrind内存泄漏详细测试
valgrind_leak_test() {
    local lang_version="$1"
    local test_case="$2"
    local input="$3"
    
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
    
    echo "使用Valgrind测试 $lang_version 版本 - $test_case..."
    
    # 创建临时输入文件
    temp_input="/tmp/calculator_valgrind_input.txt"
    echo "$input" > "$temp_input"
    echo "quit" >> "$temp_input"
    
    # 使用Valgrind进行详细内存泄漏检查
    valgrind_output=$(timeout 30s valgrind \
        --leak-check=full \
        --show-leak-kinds=all \
        --track-origins=yes \
        --verbose \
        --log-file=/tmp/valgrind_$lang_version.log \
        $exe_path < "$temp_input" 2>&1)
    
    valgrind_result=$?
    
    # 分析结果
    if [ -f /tmp/valgrind_$lang_version.log ]; then
        echo -e "${BLUE}详细日志已保存到 /tmp/valgrind_$lang_version.log${NC}"
        
        # 检查错误摘要
        error_summary=$(grep "ERROR SUMMARY:" /tmp/valgrind_$lang_version.log)
        if [ ! -z "$error_summary" ]; then
            echo "$error_summary"
            error_count=$(echo "$error_summary" | awk '{print $4}')
            if [ "$error_count" = "0" ]; then
                echo -e "${GREEN}无内存错误${NC}"
            else
                echo -e "${RED}发现 $error_count 个内存错误${NC}"
            fi
        fi
        
        # 检查内存泄漏
        leak_summary=$(grep "definitely lost:" /tmp/valgrind_$lang_version.log)
        if [ ! -z "$leak_summary" ]; then
            echo "$leak_summary"
            leak_bytes=$(echo "$leak_summary" | awk '{print $4}')
            if [ "$leak_bytes" = "0" ] || [ "$leak_bytes" = "0," ]; then
                echo -e "${GREEN}无明确内存泄漏${NC}"
            else
                echo -e "${RED}发现内存泄漏: $leak_bytes 字节${NC}"
            fi
        fi
        
        # 检查间接丢失
        indirect_summary=$(grep "indirectly lost:" /tmp/valgrind_$lang_version.log)
        if [ ! -z "$indirect_summary" ]; then
            echo "$indirect_summary"
            indirect_bytes=$(echo "$indirect_summary" | awk '{print $4}')
            if [ "$indirect_bytes" != "0" ] && [ "$indirect_bytes" != "0," ]; then
                echo -e "${YELLOW}发现间接内存泄漏: $indirect_bytes 字节${NC}"
            fi
        fi
        
        # 检查可能丢失
        possibly_lost_summary=$(grep "possibly lost:" /tmp/valgrind_$lang_version.log)
        if [ ! -z "$possibly_lost_summary" ]; then
            echo "$possibly_lost_summary"
            possibly_lost_bytes=$(echo "$possibly_lost_summary" | awk '{print $4}')
            if [ "$possibly_lost_bytes" != "0" ] && [ "$possibly_lost_bytes" != "0," ]; then
                echo -e "${YELLOW}发现可能的内存泄漏: $possibly_lost_bytes 字节${NC}"
            fi
        fi
    fi
    
    # 清理临时文件
    rm -f "$temp_input" /tmp/valgrind_$lang_version.log
    
    return $valgrind_result
}

# Valgrind数组边界检查
valgrind_array_bounds_test() {
    local lang_version="$1"
    
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
    
    echo "使用Valgrind进行数组边界检查 ($lang_version 版本)..."
    
    # 创建可能导致数组越界的输入
    temp_input="/tmp/calculator_bounds_input.txt"
    
    # 正常长度的表达式
    echo "2 + 3 * 4" >> "$temp_input"
    
    # 较长的表达式
    long_expr=""
    for i in {1..50}; do
        if [ $i -eq 1 ]; then
            long_expr="($long_expr$i+$((i+1)))"
        else
            long_expr="($long_expr*$i)"
        fi
    done
    echo "$long_expr" >> "$temp_input"
    
    # 非常长的表达式（可能触发边界问题）
    very_long_expr=""
    for i in {1..100}; do
        if [ $i -eq 1 ]; then
            very_long_expr="$i"
        else
            very_long_expr="$very_long_expr+$i"
        fi
    done
    echo "$very_long_expr" >> "$temp_input"
    
    echo "quit" >> "$temp_input"
    
    # 使用Valgrind进行数组边界检查
    valgrind_output=$(timeout 60s valgrind \
        --tool=memcheck \
        --leak-check=no \
        --undef-value-errors=no \
        $exe_path < "$temp_input" 2>&1)
    
    valgrind_result=$?
    
    # 检查是否发现数组边界错误
    if echo "$valgrind_output" | grep -q "Invalid read\|Invalid write"; then
        echo -e "${RED}发现数组边界错误${NC}"
        echo "$valgrind_output" | grep -A 3 -B 3 "Invalid read\|Invalid write"
    else
        echo -e "${GREEN}未发现数组边界错误${NC}"
    fi
    
    # 清理临时文件
    rm -f "$temp_input"
    
    return $valgrind_result
}

# 构建项目函数
build_project_wrapper() {
    local lang_version="$1"
    build_project "$lang_version" "$SCRIPT_DIR"
}

# 检查Valgrind是否可用
if ! check_valgrind; then
    exit 1
fi

# 测试C语言版本
echo ""
echo "-------------------------------------------"
echo "Valgrind测试 - C语言版本"
echo "-------------------------------------------"

# 构建C语言版本
build_project_wrapper "c"

# 内存泄漏测试
valgrind_leak_test "c" "基本算术运算" "2 + 3 * 4"
valgrind_leak_test "c" "科学函数计算" "sin(pi/2) + cos(pi/4)"
valgrind_leak_test "c" "复合表达式" "sqrt(2^2 + 3^2)"
valgrind_leak_test "c" "错误处理" "5 / 0"

# 数组边界检查
valgrind_array_bounds_test "c"

# 测试C++版本
echo ""
echo "-------------------------------------------"
echo "Valgrind测试 - C++语言版本"
echo "-------------------------------------------"

# 构建C++语言版本
build_project_wrapper "cpp"

# 内存泄漏测试
valgrind_leak_test "cpp" "基本算术运算" "2 + 3 * 4"
valgrind_leak_test "cpp" "科学函数计算" "sin(pi/2) + cos(pi/4)"
valgrind_leak_test "cpp" "复合表达式" "sqrt(2^2 + 3^2)"
valgrind_leak_test "cpp" "错误处理" "5 / 0"

# 数组边界检查
valgrind_array_bounds_test "cpp"

echo ""
echo "==========================================="
echo "Valgrind测试完成"
echo "==========================================="