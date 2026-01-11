# CMakeLists.txt
cmake_minimum_required(VERSION 3.22.0)

project(carsenal VERSION 0.1.1 LANGUAGES ASM C CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

set(CPACK_CMAKE_GENERATOR "Unix Makefiles")
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type")
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "RelWithDebInfo" "MinSizeRel")

# CMake 中可以通过设置编译器标志来添加 -pg 和 -g 参数
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pg")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O1")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wunused-variable")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -finput-charset=UTF-8 -fexec-charset=UTF-8")

# include(cmake/get_boost.cmake)

# 三方库代码
add_subdirectory(third/log_format/log.c)           # 添加logc库的子目录
add_subdirectory(third/cmdline_parser/gflags)      # 添加gflags库的子目录
add_subdirectory(third/general_purpose/boost)      # 添加boost库的子目录

## 设置公共头文件路径
# set(PROJECT_PUBLIC_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/third)
## 添加第三方库路径
# list(APPEND third_INCLUDE_DIRS
#         ${CMAKE_SOURCE_DIR}/third/cmdline_parser
#         ${CMAKE_SOURCE_DIR}/third/log_format
#         ${CMAKE_SOURCE_DIR}/third/general_purpose/boost
# )
## 创建接口库 (统一访问点)
# add_library(third_warp INTERFACE)
# target_include_directories(third_warp INTERFACE
#         $<BUILD_INTERFACE:${PROJECT_PUBLIC_INCLUDE_DIR}>  # 项目公共头文件
#         $<BUILD_INTERFACE:${third_INCLUDE_DIRS}>          # 第三方头文件
# )

# 包含子目录
# 本项目库代码
add_subdirectory(library/colorfmt)                    # 添加库的子目录

# 本项目源代码
add_subdirectory(program/calculator_qt)               # 添加源代码的子目录
add_subdirectory(program/editor_tiny)                 # 添加源代码的子目录

# 本地样例代码
# add_subdirectory(tests/examples/knowledge_cpp)
# add_subdirectory(tests/examples/third)