project(compiler_lib)

set(SOURCE_FILES src/lexer.cpp)

add_library(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC include)