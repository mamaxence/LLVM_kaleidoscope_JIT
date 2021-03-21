cmake_minimum_required(VERSION 3.17)
project(llvm_kaleidoscope)

enable_testing()

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} main.cpp)

add_subdirectory(compiler_lib)
target_link_libraries(${PROJECT_NAME} compiler_lib)

add_subdirectory(tests)

# flags
list(APPEND flags "-fPIC" "-Wall" "-Wextra" "-Wpedantic")

if (CMAKE_BUILD_TYPE MATCHES Debug)
    list(APPEND flags "-g")
elseif(CMAKE_BUILD_TYPE MATCHES Release)
    list(APPEND flags "-O2")
endif ()

message("use flags : ${flags}")
target_compile_options(${PROJECT_NAME} PRIVATE ${flags})
