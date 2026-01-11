#!/bin/bash
set -e

PROJECT_ROOT="/root/code/CArsenal"
VCPKG_DIR="$PROJECT_ROOT/third/lib_manager/vcpkg"

echo "1. 更新 vcpkg..."
cd $VCPKG_DIR
git pull
./bootstrap-vcpkg.sh

echo "2. 安装 Boost..."
./vcpkg install boost-filesystem boost-system boost-thread boost-program-options boost-regex

echo "3. 清理构建目录..."
cd $PROJECT_ROOT
rm -rf build_cmake

echo "4. 配置项目..."
cmake -B build_cmake -S . \
  -DCMAKE_TOOLCHAIN_FILE=$VCPKG_DIR/scripts/buildsystems/vcpkg.cmake \
  -DCMAKE_BUILD_TYPE=Release

echo "5. 构建..."
cmake --build build_cmake --config Release

echo "完成！"