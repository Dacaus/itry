clear
# 1. 使用预设进行配置
cmake --preset=debug

# 构建（指定构建目录）
cmake --build ./build/debug

gdb ./build/debug/bin/main