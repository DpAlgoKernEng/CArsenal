cmake_minimum_required(VERSION 3.22.0)

# 项目名称和版本
project(scientific_calculator VERSION 1.0 LANGUAGES ASM C CXX)

# 设置C标准
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 生成 build/compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# 设置编译选项
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -O2")

# 设置编译选项
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O2")

# 设置字符集为 UTF-8
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -finput-charset=UTF-8 -fexec-charset=UTF-8")

# CMake 中可以通过设置编译器标志来添加 -pg 和 -g 参数
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pg")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")

# 启用编译器警告，捕获潜在的内存问题
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")

# 包含目录 
# include(cmake/get_boost.cmake)

# 本项目库代码
add_subdirectory(calculator_c)
add_subdirectory(calculator_cpp)
