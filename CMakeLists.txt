cmake_minimum_required(VERSION 3.17)
project(llvm_kaleidoscope)

enable_testing()

set(CMAKE_CXX_STANDARD 20)

find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_INCLUDE_DIRS}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DEFINITIONS}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

add_executable(${PROJECT_NAME} main.cpp compiler_lib/include/program.h)

llvm_map_components_to_libnames(llvm_libs support core irreader)
target_link_libraries(${PROJECT_NAME} ${llvm_libs})

add_subdirectory(compiler_lib)
target_link_libraries(${PROJECT_NAME} compiler_lib)

add_subdirectory(tests)

# flags
list(APPEND flags "-fPIC" "-Wall" "-Wextra" "-Wpedantic" "-rdynamic")

if (CMAKE_BUILD_TYPE MATCHES Debug)
    list(APPEND flags "-g")
elseif(CMAKE_BUILD_TYPE MATCHES Release)
    list(APPEND flags "-O2")
endif ()

message("use flags : ${flags}")
target_compile_options(${PROJECT_NAME} PRIVATE ${flags})
