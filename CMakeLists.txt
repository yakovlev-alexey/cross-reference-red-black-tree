cmake_minimum_required(VERSION 3.10)
project(cross_reference_red_black_tree)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall -Wextra -Werror -Wno-missing-field-initializers -Wold-style-cast)

set(ANALYZER_SOURCES src/map.hpp src/list.hpp src/text-analyzer.hpp src/text-analyzer.cpp)

add_executable(ConsoleTextAnalyzer src/main.cpp ${ANALYZER_SOURCES})

add_executable(TestTextAnalyzer tests/test-main.cpp ${ANALYZER_SOURCES})
