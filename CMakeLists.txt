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

# 添加LLVM包含目录
include_directories(${LLVM_INCLUDE_DIRS})

# 处理LLVM定义
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

# 精简的LLVM组件列表（只包含必要的）
set(LLVM_COMPONENTS
    core          # IR 数据结构：Module, Function, BasicBlock, Instruction
)

# 使用llvm_map_components_to_libnames
llvm_map_components_to_libnames(llvm_libs ${LLVM_COMPONENTS})

# 打印调试信息
message(STATUS "LLVM components: ${LLVM_COMPONENTS}")
message(STATUS "LLVM libraries found: ${llvm_libs}")

# 检查关键库是否存在
foreach(lib ${llvm_libs})
    message(STATUS "  - ${lib}")
endforeach()
set(SRC_LISTS 
    src/main.cpp
    src/token.cpp
    src/ast.cpp
    src/parser.cpp
)

add_executable(main ${SRC_LISTS})
target_include_directories(main PUBLIC include/)
target_link_libraries(main ${llvm_libs})