clear
# 1. 使用预设进行配置
cmake --preset=debug -DCMAKE_CXX_FLAGS="-g -O0 -fno-inline -D_DEBUG"

# 构建（指定构建目录）
cmake --build ./build/debug 

gdb ./build/debug/bin/main