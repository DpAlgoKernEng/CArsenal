# CMakeLists.txt
cmake_minimum_required(VERSION 3.22.0)

# 设置 vcpkg 工具链（必须在 project() 之前）
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/third/lib_manager/vcpkg/scripts/buildsystems/vcpkg.cmake
    CACHE STRING "Vcpkg toolchain file")

project(carsenal VERSION 1.0.0 LANGUAGES ASM C CXX)

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
# add_subdirectory(third/general_purpose/boost)      # 添加boost库的子目录

# vcpkg 方式

set(Boost_DEBUG ON)  # 启用调试信息，查看查找过程
set(Boost_USE_STATIC_LIBS OFF)  # 或 ON，根据你的需求
# 查找 Boost 库
find_package(Boost 1.90.0 REQUIRED COMPONENTS
    program_options  # 命令解析
    log              # 日志主库
    log_setup        # 日志配置
    thread           # 多线程
    filesystem       # 文件系统（日志需要）
    system           # 系统支持
    date_time        # 时间戳
    chrono           # 时间功能
)
# 打印找到的 Boost 信息
if(Boost_FOUND)
    message(STATUS "Boost found!")
    message(STATUS "  Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "  Boost_LIBRARY_DIRS: ${Boost_LIBRARY_DIRS}")
    message(STATUS "  Boost_VERSION: ${Boost_VERSION}")
    message(STATUS "  Boost_LIBRARIES: ${Boost_LIBRARIES}")
endif()

# 主程序
add_executable(carsenal src/CArsenal.cpp)
# 链接 Boost 库
target_link_libraries(carsenal PRIVATE
    Boost::program_options
    Boost::log
    Boost::log_setup
    Boost::thread
    Boost::filesystem
    Boost::system
    Boost::date_time
    Boost::chrono
)

# 包含子目录
# 本项目库代码
add_subdirectory(library/colorfmt)                    # 添加库的子目录

# 本项目源代码
add_subdirectory(program/calculator_qt)               # 添加源代码的子目录
add_subdirectory(program/editor_tiny)                 # 添加源代码的子目录

# 本地样例代码
# add_subdirectory(tests/examples/knowledge_cpp)
# add_subdirectory(tests/examples/third)