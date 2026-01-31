cmake_minimum_required(VERSION 3.31)
project(dtoy VERSION 1.0.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/libs)

find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# 添加LLVM包含目录
include_directories(${LLVM_INCLUDE_DIRS})

# 处理LLVM定义
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

# # 精简的LLVM组件列表（只包含必要的）
# set(LLVM_COMPONENTS
#     core          # IR 数据结构：Module, Function, BasicBlock, Instruction
#     native
#     support
#     target
#       executionengine
#   orcjit
#   jitlink
#   object
# )

# # 使用llvm_map_components_to_libnames
# llvm_map_components_to_libnames(llvm_libs ${LLVM_COMPONENTS})

# # 打印调试信息
# message(STATUS "LLVM components: ${LLVM_COMPONENTS}")
# message(STATUS "LLVM libraries found: ${llvm_libs}")


# 关键：用 llvm-config 决定链接库
execute_process(
  COMMAND llvm-config --libs --system-libs
          core orcjit executionengine jitlink native support
  OUTPUT_VARIABLE LLVM_LIBS
)
string(STRIP "${LLVM_LIBS}" LLVM_LIBS)


# 检查关键库是否存在
foreach(lib ${LLVM_LIBS})
    message(STATUS "  - ${lib}")
endforeach()
set(SRC_LISTS 
    src/main.cpp
    src/token.cpp
    src/ast.cpp
    src/parser.cpp
    src/itryType.cpp
)

add_executable(main ${SRC_LISTS})
target_include_directories(main PUBLIC include/)
target_link_libraries(main ${LLVM_LIBS})

