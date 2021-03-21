project(Google_tests)
add_subdirectory(lib)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

set(SOURCE_FILES testLexer.cpp)

# adding the Google_Tests_run target
add_executable(Google_Tests_run ${SOURCE_FILES})

target_link_libraries(Google_Tests_run compiler_lib)
target_link_libraries(Google_Tests_run gtest gtest_main)
