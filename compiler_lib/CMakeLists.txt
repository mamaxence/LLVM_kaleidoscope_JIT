project(compiler_lib)

set(SOURCE_FILES src/lexer.cpp src/parser.cpp src/ppvisitor.cpp src/ast.cpp src/codegenvisitor.cpp)

# use fmt lib
set(FMT_SOURCE external/fmt-7.1.3/src/format.cc)

add_library(${PROJECT_NAME} ${SOURCE_FILES} ${FMT_SOURCE})

find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_INCLUDE_DIRS}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DEFINITIONS}")

target_include_directories(${PROJECT_NAME} PUBLIC ${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

llvm_map_components_to_libnames(llvm_libs support core irreader)
target_link_libraries(${PROJECT_NAME} ${llvm_libs})


target_include_directories(${PROJECT_NAME} PRIVATE external/fmt-7.1.3/include)
target_include_directories(${PROJECT_NAME} PUBLIC include)
