#!/bin/bash

# 测试报告生成脚本 - 收集并生成所有测试的综合报告

echo "==========================================="
echo "科学计算器测试报告生成器"
echo "==========================================="

# 设置颜色输出
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# 报告目录
REPORT_DIR="./reports"
mkdir -p "$REPORT_DIR"

# 当前日期时间
TIMESTAMP=$(date +"%Y%m%d_%H%M%S")

# 报告文件
REPORT_FILE="$REPORT_DIR/test_report_$TIMESTAMP.md"
SUMMARY_FILE="$REPORT_DIR/summary_$TIMESTAMP.txt"

# 初始化报告文件
init_report() {
    echo "# 科学计算器测试报告" > "$REPORT_FILE"
    echo "生成时间: $(date)" >> "$REPORT_FILE"
    echo "" >> "$REPORT_FILE"
    echo "## 测试概述" >> "$REPORT_FILE"
    echo "" >> "$REPORT_FILE"
    
    # 初始化摘要文件
    echo "科学计算器测试摘要" > "$SUMMARY_FILE"
    echo "生成时间: $(date)" >> "$SUMMARY_FILE"
    echo "===========================================" >> "$SUMMARY_FILE"
}

# 添加功能测试结果到报告
add_functional_test_results() {
    echo "## 功能测试结果" >> "$REPORT_FILE"
    echo "" >> "$REPORT_FILE"
    
    # 运行功能测试并将输出保存到文件
    echo "正在运行功能测试..."
    functional_output="$REPORT_DIR/functional_test_output.txt"
    
    if [ -f "./functional/functional_test.sh" ]; then
        timeout 120s ./functional/functional_test.sh > "$functional_output" 2>&1
        functional_result=$?
        
        # 将功能测试输出添加到报告
        echo '```bash' >> "$REPORT_FILE"
        cat "$functional_output" >> "$REPORT_FILE"
        echo '```' >> "$REPORT_FILE"
        echo "" >> "$REPORT_FILE"
        
        # 分析结果
        if [ $functional_result -eq 0 ]; then
            echo "### 功能测试状态: 通过" >> "$REPORT_FILE"
            echo "功能测试: 通过" >> "$SUMMARY_FILE"
        else
            echo "### 功能测试状态: 失败" >> "$REPORT_FILE"
            echo "功能测试: 失败" >> "$SUMMARY_FILE"
        fi
    else
        echo "### 功能测试状态: 未找到测试脚本" >> "$REPORT_FILE"
        echo "功能测试: 未找到测试脚本" >> "$SUMMARY_FILE"
    fi
    
    echo "" >> "$REPORT_FILE"
}

# 添加性能测试结果到报告
add_performance_test_results() {
    echo "## 性能测试结果" >> "$REPORT_FILE"
    echo "" >> "$REPORT_FILE"
    
    # 运行性能测试并将输出保存到文件
    echo "正在运行性能测试..."
    performance_output="$REPORT_DIR/performance_test_output.txt"
    
    if [ -f "./performance/performance_test.sh" ]; then
        timeout 120s ./performance/performance_test.sh > "$performance_output" 2>&1
        performance_result=$?
        
        # 将性能测试输出添加到报告
        echo '```bash' >> "$REPORT_FILE"
        cat "$performance_output" >> "$REPORT_FILE"
        echo '```' >> "$REPORT_FILE"
        echo "" >> "$REPORT_FILE"
        
        # 分析结果
        if [ $performance_result -eq 0 ]; then
            echo "### 性能测试状态: 通过" >> "$REPORT_FILE"
            echo "性能测试: 通过" >> "$SUMMARY_FILE"
        else
            echo "### 性能测试状态: 失败" >> "$REPORT_FILE"
            echo "性能测试: 失败" >> "$SUMMARY_FILE"
        fi
    else
        echo "### 性能测试状态: 未找到测试脚本" >> "$REPORT_FILE"
        echo "性能测试: 未找到测试脚本" >> "$SUMMARY_FILE"
    fi
    
    echo "" >> "$REPORT_FILE"
}

# 添加压力测试结果到报告
add_stress_test_results() {
    echo "## 压力测试结果" >> "$REPORT_FILE"
    echo "" >> "$REPORT_FILE"
    
    # 运行压力测试并将输出保存到文件
    echo "正在运行压力测试..."
    stress_output="$REPORT_DIR/stress_test_output.txt"
    
    if [ -f "./performance/stress_test.sh" ]; then
        timeout 180s ./performance/stress_test.sh > "$stress_output" 2>&1
        stress_result=$?
        
        # 将压力测试输出添加到报告
        echo '```bash' >> "$REPORT_FILE"
        cat "$stress_output" >> "$REPORT_FILE"
        echo '```' >> "$REPORT_FILE"
        echo "" >> "$REPORT_FILE"
        
        # 分析结果
        if [ $stress_result -eq 0 ]; then
            echo "### 压力测试状态: 通过" >> "$REPORT_FILE"
            echo "压力测试: 通过" >> "$SUMMARY_FILE"
        else
            echo "### 压力测试状态: 失败" >> "$REPORT_FILE"
            echo "压力测试: 失败" >> "$SUMMARY_FILE"
        fi
    else
        echo "### 压力测试状态: 未找到测试脚本" >> "$REPORT_FILE"
        echo "压力测试: 未找到测试脚本" >> "$SUMMARY_FILE"
    fi
    
    echo "" >> "$REPORT_FILE"
}

# 添加安全测试结果到报告
add_security_test_results() {
    echo "## 安全测试结果" >> "$REPORT_FILE"
    echo "" >> "$REPORT_FILE"
    
    # 运行安全测试并将输出保存到文件
    echo "正在运行安全测试..."
    security_output="$REPORT_DIR/security_test_output.txt"
    
    if [ -f "./security/security_test.sh" ]; then
        timeout 180s ./security/security_test.sh > "$security_output" 2>&1
        security_result=$?
        
        # 将安全测试输出添加到报告
        echo '```bash' >> "$REPORT_FILE"
        cat "$security_output" >> "$REPORT_FILE"
        echo '```' >> "$REPORT_FILE"
        echo "" >> "$REPORT_FILE"
        
        # 分析结果
        if [ $security_result -eq 0 ]; then
            echo "### 安全测试状态: 通过" >> "$REPORT_FILE"
            echo "安全测试: 通过" >> "$SUMMARY_FILE"
        else
            echo "### 安全测试状态: 失败" >> "$REPORT_FILE"
            echo "安全测试: 失败" >> "$SUMMARY_FILE"
        fi
    else
        echo "### 安全测试状态: 未找到测试脚本" >> "$REPORT_FILE"
        echo "安全测试: 未找到测试脚本" >> "$SUMMARY_FILE"
    fi
    
    echo "" >> "$REPORT_FILE"
}

# 添加内存测试结果到报告
add_memory_test_results() {
    echo "## 内存测试结果" >> "$REPORT_FILE"
    echo "" >> "$REPORT_FILE"
    
    # 运行内存测试并将输出保存到文件
    echo "正在运行内存测试..."
    memory_output="$REPORT_DIR/memory_test_output.txt"
    
    if [ -f "./performance/memory_test.sh" ]; then
        timeout 180s ./performance/memory_test.sh > "$memory_output" 2>&1
        memory_result=$?
        
        # 将内存测试输出添加到报告
        echo '```bash' >> "$REPORT_FILE"
        cat "$memory_output" >> "$REPORT_FILE"
        echo '```' >> "$REPORT_FILE"
        echo "" >> "$REPORT_FILE"
        
        # 分析结果
        if [ $memory_result -eq 0 ]; then
            echo "### 内存测试状态: 通过" >> "$REPORT_FILE"
            echo "内存测试: 通过" >> "$SUMMARY_FILE"
        else
            echo "### 内存测试状态: 失败" >> "$REPORT_FILE"
            echo "内存测试: 失败" >> "$SUMMARY_FILE"
        fi
    else
        echo "### 内存测试状态: 未找到测试脚本" >> "$REPORT_FILE"
        echo "内存测试: 未找到测试脚本" >> "$SUMMARY_FILE"
    fi
    
    echo "" >> "$REPORT_FILE"
}

# 添加Valgrind测试结果到报告
add_valgrind_test_results() {
    echo "## Valgrind测试结果" >> "$REPORT_FILE"
    echo "" >> "$REPORT_FILE"
    
    # 检查Valgrind是否安装
    if ! command -v valgrind &> /dev/null; then
        echo "### Valgrind测试状态: 跳过 (未安装Valgrind)" >> "$REPORT_FILE"
        echo "Valgrind测试: 跳过 (未安装Valgrind)" >> "$SUMMARY_FILE"
        echo "" >> "$REPORT_FILE"
        return
    fi
    
    # 运行Valgrind测试并将输出保存到文件
    echo "正在运行Valgrind测试..."
    valgrind_output="$REPORT_DIR/valgrind_test_output.txt"
    
    if [ -f "./performance/valgrind_test.sh" ]; then
        timeout 240s ./performance/valgrind_test.sh > "$valgrind_output" 2>&1
        valgrind_result=$?
        
        # 将Valgrind测试输出添加到报告
        echo '```bash' >> "$REPORT_FILE"
        cat "$valgrind_output" >> "$REPORT_FILE"
        echo '```' >> "$REPORT_FILE"
        echo "" >> "$REPORT_FILE"
        
        # 分析结果
        if [ $valgrind_result -eq 0 ]; then
            echo "### Valgrind测试状态: 通过" >> "$REPORT_FILE"
            echo "Valgrind测试: 通过" >> "$SUMMARY_FILE"
        else
            echo "### Valgrind测试状态: 失败" >> "$REPORT_FILE"
            echo "Valgrind测试: 失败" >> "$SUMMARY_FILE"
        fi
    else
        echo "### Valgrind测试状态: 未找到测试脚本" >> "$REPORT_FILE"
        echo "Valgrind测试: 未找到测试脚本" >> "$SUMMARY_FILE"
    fi
    
    echo "" >> "$REPORT_FILE"
}

# 生成测试摘要
generate_summary() {
    echo "" >> "$REPORT_FILE"
    echo "## 测试摘要" >> "$REPORT_FILE"
    echo "" >> "$REPORT_FILE"
    
    # 将摘要文件内容添加到报告
    echo '```' >> "$REPORT_FILE"
    cat "$SUMMARY_FILE" >> "$REPORT_FILE"
    echo '```' >> "$REPORT_FILE"
    
    echo "" >> "$REPORT_FILE"
    echo "报告已生成: $REPORT_FILE" >> "$REPORT_FILE"
    echo "摘要已生成: $SUMMARY_FILE" >> "$REPORT_FILE"
}

# 显示测试结果摘要
display_summary() {
    echo ""
    echo "==========================================="
    echo "测试结果摘要"
    echo "==========================================="
    cat "$SUMMARY_FILE"
    echo ""
    echo "完整报告已生成: $REPORT_FILE"
}

# 主函数
main() {
    # 初始化报告
    init_report
    
    # 添加各类测试结果
    add_functional_test_results
    add_performance_test_results
    add_stress_test_results
    add_security_test_results
    add_memory_test_results
    add_valgrind_test_results
    
    # 生成摘要
    generate_summary
    
    # 显示摘要
    display_summary
}

# 运行主函数
main