#!/bin/bash

# 完整测试执行脚本 - 运行所有测试并生成综合报告

echo "==========================================="
echo "科学计算器完整测试套件"
echo "==========================================="

# 设置颜色输出
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# 脚本目录
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# 测试标志
RUN_FUNCTIONAL=true
RUN_PERFORMANCE=true
RUN_STRESS=true
RUN_SECURITY=true
RUN_MEMORY=true
RUN_VALGRIND=true
GENERATE_REPORT=true

# 显示帮助信息
show_help() {
    echo "用法: $0 [选项]"
    echo ""
    echo "选项:"
    echo "  --functional-only     仅运行功能测试"
    echo "  --performance-only    仅运行性能测试"
    echo "  --stress-only         仅运行压力测试"
    echo "  --security-only       仅运行安全测试"
    echo "  --memory-only         仅运行内存测试"
    echo "  --valgrind-only       仅运行Valgrind测试"
    echo "  --no-functional       跳过功能测试"
    echo "  --no-performance      跳过性能测试"
    echo "  --no-stress           跳过压力测试"
    echo "  --no-security         跳过安全测试"
    echo "  --no-memory           跳过内存测试"
    echo "  --no-valgrind         跳过Valgrind测试"
    echo "  --no-report           不生成测试报告"
    echo "  --help, -h            显示此帮助信息"
    echo ""
}

# 解析命令行参数
parse_arguments() {
    while [[ $# -gt 0 ]]; do
        case $1 in
            --functional-only)
                RUN_FUNCTIONAL=true
                RUN_PERFORMANCE=false
                RUN_STRESS=false
                RUN_SECURITY=false
                RUN_MEMORY=false
                RUN_VALGRIND=false
                shift
                ;;
            --performance-only)
                RUN_FUNCTIONAL=false
                RUN_PERFORMANCE=true
                RUN_STRESS=false
                RUN_SECURITY=false
                RUN_MEMORY=false
                RUN_VALGRIND=false
                shift
                ;;
            --stress-only)
                RUN_FUNCTIONAL=false
                RUN_PERFORMANCE=false
                RUN_STRESS=true
                RUN_SECURITY=false
                RUN_MEMORY=false
                RUN_VALGRIND=false
                shift
                ;;
            --security-only)
                RUN_FUNCTIONAL=false
                RUN_PERFORMANCE=false
                RUN_STRESS=false
                RUN_SECURITY=true
                RUN_MEMORY=false
                RUN_VALGRIND=false
                shift
                ;;
            --memory-only)
                RUN_FUNCTIONAL=false
                RUN_PERFORMANCE=false
                RUN_STRESS=false
                RUN_SECURITY=false
                RUN_MEMORY=true
                RUN_VALGRIND=false
                shift
                ;;
            --valgrind-only)
                RUN_FUNCTIONAL=false
                RUN_PERFORMANCE=false
                RUN_STRESS=false
                RUN_SECURITY=false
                RUN_MEMORY=false
                RUN_VALGRIND=true
                shift
                ;;
            --no-functional)
                RUN_FUNCTIONAL=false
                shift
                ;;
            --no-performance)
                RUN_PERFORMANCE=false
                shift
                ;;
            --no-stress)
                RUN_STRESS=false
                shift
                ;;
            --no-security)
                RUN_SECURITY=false
                shift
                ;;
            --no-memory)
                RUN_MEMORY=false
                shift
                ;;
            --no-valgrind)
                RUN_VALGRIND=false
                shift
                ;;
            --no-report)
                GENERATE_REPORT=false
                shift
                ;;
            --help|-h)
                show_help
                exit 0
                ;;
            *)
                echo "未知参数: $1"
                show_help
                exit 1
                ;;
        esac
    done
}

# 检查依赖工具
check_dependencies() {
    echo "检查依赖工具..."
    
    local missing_tools=()
    
    # 检查基本工具
    if ! command -v cmake &> /dev/null; then
        missing_tools+=("cmake")
    fi
    
    if ! command -v make &> /dev/null; then
        missing_tools+=("make")
    fi
    
    # 如果需要运行Valgrind测试，检查Valgrind
    if [ "$RUN_VALGRIND" = true ] || [ "$RUN_MEMORY" = true ]; then
        if ! command -v valgrind &> /dev/null; then
            echo -e "${YELLOW}警告: 未找到 Valgrind，相关测试将被跳过${NC}"
            RUN_VALGRIND=false
        fi
    fi
    
    # 如果有任何缺失的工具，显示警告
    if [ ${#missing_tools[@]} -ne 0 ]; then
        echo -e "${RED}错误: 缺少以下工具: ${missing_tools[*]}${NC}"
        echo "请安装这些工具后再运行测试。"
        return 1
    fi
    
    echo -e "${GREEN}所有必需工具都已安装${NC}"
    return 0
}

# 构建项目
build_projects() {
    echo ""
    echo "==========================================="
    echo "构建项目"
    echo "==========================================="
    
    local c_build_success=true
    local cpp_build_success=true
    
    # 构建C语言版本
    echo "正在构建C语言版本..."
    cd "$SCRIPT_DIR/../calculator_c"
    mkdir -p build_cmake
    cd build_cmake
    cmake .. >/dev/null 2>&1
    make clean >/dev/null 2>&1
    if make >/dev/null 2>&1; then
        echo -e "${GREEN}C语言版本构建成功${NC}"
    else
        echo -e "${RED}C语言版本构建失败${NC}"
        c_build_success=false
    fi
    cd "$SCRIPT_DIR"
    
    # 构建C++语言版本
    echo "正在构建C++语言版本..."
    cd "$SCRIPT_DIR/../calculator_cpp"
    mkdir -p build_cmake
    cd build_cmake
    cmake .. >/dev/null 2>&1
    make clean >/dev/null 2>&1
    if make >/dev/null 2>&1; then
        echo -e "${GREEN}C++语言版本构建成功${NC}"
    else
        echo -e "${RED}C++语言版本构建失败${NC}"
        cpp_build_success=false
    fi
    cd "$SCRIPT_DIR"
    
    # 如果任何一个构建失败，返回错误
    if [ "$c_build_success" = false ] || [ "$cpp_build_success" = false ]; then
        return 1
    fi
    
    return 0
}

# 运行功能测试
run_functional_tests() {
    if [ "$RUN_FUNCTIONAL" = false ]; then
        echo -e "${YELLOW}跳过功能测试${NC}"
        return 0
    fi
    
    echo ""
    echo "==========================================="
    echo "运行功能测试"
    echo "==========================================="
    
    if [ -f "./functional/functional_test.sh" ]; then
        chmod +x ./functional/functional_test.sh
        timeout 120s ./functional/functional_test.sh
        return $?
    else
        echo -e "${RED}错误: 找不到功能测试脚本${NC}"
        return 1
    fi
}

# 运行性能测试
run_performance_tests() {
    if [ "$RUN_PERFORMANCE" = false ]; then
        echo -e "${YELLOW}跳过性能测试${NC}"
        return 0
    fi
    
    echo ""
    echo "==========================================="
    echo "运行性能测试"
    echo "==========================================="
    
    if [ -f "./performance/performance_test.sh" ]; then
        chmod +x ./performance/performance_test.sh
        timeout 120s ./performance/performance_test.sh
        return $?
    else
        echo -e "${RED}错误: 找不到性能测试脚本${NC}"
        return 1
    fi
}

# 运行压力测试
run_stress_tests() {
    if [ "$RUN_STRESS" = false ]; then
        echo -e "${YELLOW}跳过压力测试${NC}"
        return 0
    fi
    
    echo ""
    echo "==========================================="
    echo "运行压力测试"
    echo "==========================================="
    
    if [ -f "./performance/stress_test.sh" ]; then
        chmod +x ./performance/stress_test.sh
        timeout 180s ./performance/stress_test.sh
        return $?
    else
        echo -e "${RED}错误: 找不到压力测试脚本${NC}"
        return 1
    fi
}

# 运行安全测试
run_security_tests() {
    if [ "$RUN_SECURITY" = false ]; then
        echo -e "${YELLOW}跳过安全测试${NC}"
        return 0
    fi
    
    echo ""
    echo "==========================================="
    echo "运行安全测试"
    echo "==========================================="
    
    if [ -f "./security/security_test.sh" ]; then
        chmod +x ./security/security_test.sh
        timeout 180s ./security/security_test.sh
        return $?
    else
        echo -e "${RED}错误: 找不到安全测试脚本${NC}"
        return 1
    fi
}

# 运行内存测试
run_memory_tests() {
    if [ "$RUN_MEMORY" = false ]; then
        echo -e "${YELLOW}跳过内存测试${NC}"
        return 0
    fi
    
    echo ""
    echo "==========================================="
    echo "运行内存测试"
    echo "==========================================="
    
    if [ -f "./performance/memory_test.sh" ]; then
        chmod +x ./performance/memory_test.sh
        timeout 180s ./performance/memory_test.sh
        return $?
    else
        echo -e "${RED}错误: 找不到内存测试脚本${NC}"
        return 1
    fi
}

# 运行Valgrind测试
run_valgrind_tests() {
    if [ "$RUN_VALGRIND" = false ]; then
        echo -e "${YELLOW}跳过Valgrind测试${NC}"
        return 0
    fi
    
    # 检查Valgrind是否安装
    if ! command -v valgrind &> /dev/null; then
        echo -e "${YELLOW}跳过Valgrind测试 (未安装Valgrind)${NC}"
        return 0
    fi
    
    echo ""
    echo "==========================================="
    echo "运行Valgrind测试"
    echo "==========================================="
    
    if [ -f "./performance/valgrind_test.sh" ]; then
        chmod +x ./performance/valgrind_test.sh
        timeout 240s ./performance/valgrind_test.sh
        return $?
    else
        echo -e "${RED}错误: 找不到Valgrind测试脚本${NC}"
        return 1
    fi
}

# 生成测试报告
generate_test_report() {
    if [ "$GENERATE_REPORT" = false ]; then
        echo -e "${YELLOW}跳过测试报告生成${NC}"
        return 0
    fi
    
    echo ""
    echo "==========================================="
    echo "生成测试报告"
    echo "==========================================="
    
    if [ -f "./generate_report.sh" ]; then
        chmod +x ./generate_report.sh
        timeout 300s ./generate_report.sh
        return $?
    else
        echo -e "${RED}错误: 找不到测试报告生成脚本${NC}"
        return 1
    fi
}

# 显示测试执行摘要
show_execution_summary() {
    echo ""
    echo "==========================================="
    echo "测试执行摘要"
    echo "==========================================="
    
    echo "测试类型              状态"
    echo "-------------------------------------------"
    printf "%-20s %s\n" "功能测试" $([ "$RUN_FUNCTIONAL" = true ] && echo "已执行" || echo "已跳过")
    printf "%-20s %s\n" "性能测试" $([ "$RUN_PERFORMANCE" = true ] && echo "已执行" || echo "已跳过")
    printf "%-20s %s\n" "压力测试" $([ "$RUN_STRESS" = true ] && echo "已执行" || echo "已跳过")
    printf "%-20s %s\n" "安全测试" $([ "$RUN_SECURITY" = true ] && echo "已执行" || echo "已跳过")
    printf "%-20s %s\n" "内存测试" $([ "$RUN_MEMORY" = true ] && echo "已执行" || echo "已跳过")
    printf "%-20s %s\n" "Valgrind测试" $([ "$RUN_VALGRIND" = true ] && echo "已执行" || echo "已跳过")
    printf "%-20s %s\n" "报告生成" $([ "$GENERATE_REPORT" = true ] && echo "已执行" || echo "已跳过")
}

# 主函数
main() {
    # 解析命令行参数
    parse_arguments "$@"
    
    # 检查依赖
    if ! check_dependencies; then
        exit 1
    fi
    
    # 构建项目
    if ! build_projects; then
        echo -e "${RED}构建失败，退出测试${NC}"
        exit 1
    fi
    
    # 运行各类测试
    local functional_result=0
    local performance_result=0
    local stress_result=0
    local security_result=0
    local memory_result=0
    local valgrind_result=0
    local report_result=0
    
    run_functional_tests
    functional_result=$?
    
    run_performance_tests
    performance_result=$?
    
    run_stress_tests
    stress_result=$?
    
    run_security_tests
    security_result=$?
    
    run_memory_tests
    memory_result=$?
    
    run_valgrind_tests
    valgrind_result=$?
    
    # 生成测试报告
    generate_test_report
    report_result=$?
    
    # 显示执行摘要
    show_execution_summary
    
    # 计算总体结果
    local total_failures=0
    [ $functional_result -ne 0 ] && ((total_failures++))
    [ $performance_result -ne 0 ] && ((total_failures++))
    [ $stress_result -ne 0 ] && ((total_failures++))
    [ $security_result -ne 0 ] && ((total_failures++))
    [ $memory_result -ne 0 ] && ((total_failures++))
    [ $valgrind_result -ne 0 ] && ((total_failures++))
    
    echo ""
    echo "==========================================="
    echo "最终结果"
    echo "==========================================="
    
    if [ $total_failures -eq 0 ]; then
        echo -e "${GREEN}所有测试已成功执行!${NC}"
        exit 0
    else
        echo -e "${RED}有 $total_failures 个测试组件失败${NC}"
        exit 1
    fi
}

# 运行主函数
main "$@"