#!/bin/bash

# 共享测试工具函数库

# 设置颜色输出
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# 日志函数
log_info() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

log_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

log_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

log_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# 构建项目函数
build_project() {
    local lang_version="$1"
    local script_dir="$2"
    
    log_info "正在构建 $lang_version 版本..."
    
    if [ "$lang_version" = "c" ]; then
        cd "$script_dir/../calculator_c"
        mkdir -p build_cmake
        cd build_cmake
        cmake .. >/dev/null 2>&1
        make >/dev/null 2>&1
        build_result=$?
        cd "$script_dir"
    else
        cd "$script_dir/../calculator_cpp"
        mkdir -p build_cmake
        cd build_cmake
        cmake .. >/dev/null 2>&1
        make >/dev/null 2>&1
        build_result=$?
        cd "$script_dir"
    fi
    
    if [ $build_result -ne 0 ]; then
        log_error "构建 $lang_version 版本失败"
        return 1
    else
        log_success "构建 $lang_version 版本成功"
        return 0
    fi
}

# 检查可执行文件是否存在
check_executable() {
    local lang_version="$1"
    local script_dir="$2"
    
    if [ "$lang_version" = "c" ]; then
        exe_path="$script_dir/../calculator_c/build_cmake/scientific_calculator"
    else
        exe_path="$script_dir/../calculator_cpp/build_cmake/scientific_calculator"
    fi
    
    if [ ! -f "$exe_path" ]; then
        log_error "找不到可执行文件 $exe_path"
        return 1
    fi
    
    return 0
}

# 获取可执行文件路径
get_executable_path() {
    local lang_version="$1"
    local script_dir="$2"
    
    if [ "$lang_version" = "c" ]; then
        echo "$script_dir/../calculator_c/build_cmake/scientific_calculator"
    else
        echo "$script_dir/../calculator_cpp/build_cmake/scientific_calculator"
    fi
}