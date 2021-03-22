project(compiler_lib)

set(SOURCE_FILES src/lexer.cpp src/parser.cpp src/visitor.cpp src/ast.cpp)

# use fmt lib
set(FMT_SOURCE external/fmt-7.1.3/src/format.cc)

add_library(${PROJECT_NAME} ${SOURCE_FILES} ${FMT_SOURCE})

target_include_directories(${PROJECT_NAME} PRIVATE external/fmt-7.1.3/include)
target_include_directories(${PROJECT_NAME} PUBLIC include)
