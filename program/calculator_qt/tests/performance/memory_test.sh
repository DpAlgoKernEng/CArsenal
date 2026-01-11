#!/bin/bash

# 内存测试脚本 - 测试C语言和C++版本的计算器内存使用情况

echo "==========================================="
echo "科学计算器内存测试"
echo "==========================================="

# 导入共享测试工具函数库
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/../common/test_utils.sh"

# 检查是否安装了必要的工具
check_tools() {
    local missing_tools=()
    
    if ! command -v valgrind &> /dev/null; then
        missing_tools+=("valgrind")
    fi
    
    if ! command -v ps &> /dev/null; then
        missing_tools+=("ps")
    fi
    
    if [ ${#missing_tools[@]} -ne 0 ]; then
        echo -e "${YELLOW}警告: 以下工具未安装: ${missing_tools[*]}${NC}"
        echo -e "${YELLOW}部分测试将被跳过${NC}"
        return 1
    fi
    
    return 0
}

# 内存使用监控函数
monitor_memory_usage() {
    local lang_version="$1"
    local test_name="$2"
    local input="$3"
    local iterations="$4"
    
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
    
    echo "正在监控 $lang_version 版本的内存使用情况 - $test_name..."
    
    # 创建临时输入文件
    temp_input="/tmp/calculator_memory_input.txt"
    for ((i=1; i<=iterations; i++)); do
        echo "$input" >> "$temp_input"
    done
    echo "quit" >> "$temp_input"
    
    # 使用Valgrind进行内存分析
    if command -v valgrind &> /dev/null; then
        echo -e "${BLUE}使用Valgrind进行详细内存分析...${NC}"
        
        valgrind_output=$(timeout 60s valgrind --tool=massif --time-unit=B --massif-out-file=/tmp/massif.out $exe_path < "$temp_input" 2>&1)
        valgrind_result=$?
        
        if [ $valgrind_result -eq 0 ]; then
            # 分析Massif输出
            if [ -f /tmp/massif.out ]; then
                peak_memory=$(ms_print /tmp/massif.out | grep -E "^[0-9]+[[:space:]]+[0-9]+" | head -10 | awk '{print $2}' | sort -nr | head -1)
                if [ ! -z "$peak_memory" ]; then
                    echo -e "${GREEN}峰值内存使用: $peak_memory 字节${NC}"
                fi
                rm -f /tmp/massif.out
            fi
            echo -e "${GREEN}Valgrind内存分析完成${NC}"
        else
            echo -e "${RED}Valgrind分析失败${NC}"
        fi
    else
        echo -e "${YELLOW}跳过Valgrind分析 (未安装)${NC}"
    fi
    
    # 清理临时文件
    rm -f "$temp_input"
    
    return 0
}

# 内存泄漏测试函数
test_memory_leaks() {
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
    
    echo "正在测试 $lang_version 版本的内存泄漏..."
    
    # 创建多种输入测试用例
    temp_input="/tmp/calculator_leak_input.txt"
    
    # 基本算术运算
    for i in {1..100}; do
        echo "$i + $((i+1)) * $((i+2))" >> "$temp_input"
    done
    
    # 科学函数
    echo "sin(pi/2)" >> "$temp_input"
    echo "cos(pi/4)" >> "$temp_input"
    echo "tan(pi/6)" >> "$temp_input"
    echo "ln(e)" >> "$temp_input"
    echo "log(100)" >> "$temp_input"
    echo "sqrt(16)" >> "$temp_input"
    echo "abs(-5)" >> "$temp_input"
    
    # 复合表达式
    for i in {1..50}; do
        echo "sqrt($i^2 + $((i+1))^2)" >> "$temp_input"
    done
    
    # 错误处理测试
    echo "5 / 0" >> "$temp_input"
    echo "sqrt(-1)" >> "$temp_input"
    echo "log(-1)" >> "$temp_input"
    
    echo "quit" >> "$temp_input"
    
    # 使用Valgrind检查内存泄漏
    if command -v valgrind &> /dev/null; then
        echo -e "${BLUE}使用Valgrind检查内存泄漏...${NC}"
        
        valgrind_output=$(timeout 60s valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose $exe_path < "$temp_input" 2>&1)
        valgrind_result=$?
        
        # 检查是否有内存泄漏
        if echo "$valgrind_output" | grep -q "no leaks are possible"; then
            echo -e "${GREEN}未发现内存泄漏${NC}"
        elif echo "$valgrind_output" | grep -q "definitely lost:"; then
            lost_bytes=$(echo "$valgrind_output" | grep "definitely lost:" | awk '{print $4}')
            if [ "$lost_bytes" = "0" ] || [ "$lost_bytes" = "0," ]; then
                echo -e "${GREEN}未发现明确的内存泄漏${NC}"
            else
                echo -e "${RED}发现内存泄漏: $lost_bytes 字节${NC}"
                echo "$valgrind_output" | grep -A 5 -B 5 "definitely lost:"
            fi
        else
            echo -e "${YELLOW}无法确定内存泄漏情况${NC}"
        fi
        
        # 检查是否有未初始化的值使用
        if echo "$valgrind_output" | grep -q "Conditional jump or move depends on uninitialised value"; then
            echo -e "${RED}发现未初始化值的使用${NC}"
        fi
    else
        echo -e "${YELLOW}跳过内存泄漏检查 (未安装Valgrind)${NC}"
    fi
    
    # 清理临时文件
    rm -f "$temp_input"
    
    return 0
}

# 构建项目函数
build_project_wrapper() {
    local lang_version="$1"
    build_project "$lang_version" "$SCRIPT_DIR"
}

# 检查必要工具
check_tools

# 测试C语言版本
echo ""
echo "-------------------------------------------"
echo "内存测试 - C语言版本"
echo "-------------------------------------------"

# 构建C语言版本
build_project_wrapper "c"

# 监控内存使用
monitor_memory_usage "c" "基本算术运算" "2 + 3 * 4" 1000
monitor_memory_usage "c" "科学函数计算" "sin(pi/2) + cos(pi/4)" 500
monitor_memory_usage "c" "复合表达式计算" "sqrt(2^2 + 3^2) * abs(-5)" 500
monitor_memory_usage "c" "深度嵌套表达式" "((((((2+3)*4)-5)/6)^2)+1)" 100
monitor_memory_usage "c" "大量计算任务" "2 + 3 * 4" 5000

# 内存泄漏测试
test_memory_leaks "c"

# 测试C++版本
echo ""
echo "-------------------------------------------"
echo "内存测试 - C++语言版本"
echo "-------------------------------------------"

# 构建C++语言版本
build_project_wrapper "cpp"

# 监控内存使用
monitor_memory_usage "cpp" "基本算术运算" "2 + 3 * 4" 1000
monitor_memory_usage "cpp" "科学函数计算" "sin(pi/2) + cos(pi/4)" 500
monitor_memory_usage "cpp" "复合表达式计算" "sqrt(2^2 + 3^2) * abs(-5)" 500
monitor_memory_usage "cpp" "深度嵌套表达式" "((((((2+3)*4)-5)/6)^2)+1)" 100
monitor_memory_usage "cpp" "大量计算任务" "2 + 3 * 4" 5000

# 内存泄漏测试
test_memory_leaks "cpp"

echo ""
echo "==========================================="
echo "内存测试完成"
echo "==========================================="