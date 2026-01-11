add_library(colorfmt STATIC colorfmt.c)  # 创建一个名为MathLib的静态库

# 为目标添加头文件包含路径
target_include_directories(colorfmt PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})