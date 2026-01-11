add_library(logc STATIC src/log.c)  # 创建一个名为logc的静态库

# 为目标添加头文件包含路径
target_include_directories(logc PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})