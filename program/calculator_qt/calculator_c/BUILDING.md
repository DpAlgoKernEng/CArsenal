# 构建说明

本项目现在使用CMake来生成构建系统，替代了原来的Makefile。

## 构建步骤

1. 创建构建目录:
   ```
   mkdir build_cmake
   ```

2. 进入构建目录并运行CMake:
   ```
   cd build_cmake
   cmake ..
   ```

3. 编译项目:
   ```
   make
   ```

4. 运行程序:
   ```
   ./scientific_calculator
   ```

## 安装

要安装程序，可以使用:
```
sudo make install
```

这将把可执行文件安装到 `/usr/local/bin/` 目录下。

## 清理

要清理构建文件:
```
cd build_cmake
make clean
```

或者删除整个构建目录:
```
rm -rf build_cmake