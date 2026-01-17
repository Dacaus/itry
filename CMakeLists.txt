cmake_minimum_required(VERSION 3.31)
project(dtoy VERSION 1.0.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/libs)

# # 方法1：让 CMake 自动查找（在系统默认路径中）
# find_package(LLVM REQUIRED CONFIG)

# # 关键变量（find_package 后自动设置）
# message(STATUS "LLVM found: ${LLVM_PACKAGE_VERSION}")
# message(STATUS "LLVM include dirs: ${LLVM_INCLUDE_DIRS}")
# message(STATUS "LLVM library dir: ${LLVM_LIBRARY_DIRS}")
# message(STATUS "LLVM definitions: ${LLVM_DEFINITIONS}")
# message(STATUS "LLVM tools dir: ${LLVM_TOOLS_BINARY_DIR}")



find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})
set(LLVM_COMPONENTS
    core          # IR 数据结构：Module, Function, BasicBlock, Instruction
    support       # 工具类：字符串、文件、命令行、错误处理
    
    irreader      # 读取 LLVM IR（如果需要从文件加载）
    irstrip       # IR 精简工具
    
    codegen       # 代码生成基础设施
    mc            # 机器码层
    mcparser      # 机器码解析
    mcdisassembler # 机器码反汇编
    object        # 目标文件处理
    target        # 目标平台抽象
    
    executionengine  # JIT 执行引擎基础
    orcjit           # ORC JIT 框架（现代，推荐）
    runtimeDyld      # 运行时动态链接
    orcshared       # ORC 共享库支持
    orctargetprocess # ORC 目标进程支持
    
    analysis       # 分析 Passes
    transformsutils # 转换工具
    passes         # Pass 管理器
    scalaropts     # 标量优化
    vectorize      # 向量化
    ipo            # 过程间优化
    
    native         # 本机目标
)

llvm_map_components_to_libnames(llvm_libs ${LLVM_COMPONENTS})

message(STATUS "Using LLVM components: ${LLVM_COMPONENTS}")
message(STATUS "LLVM libraries: ${llvm_libs}")