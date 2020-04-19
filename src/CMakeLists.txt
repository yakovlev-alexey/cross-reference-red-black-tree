 
cmake_minimum_required(VERSION 3.10)

project(cross-reference-red-black-tree)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -Wextra -Werror -Wno-missing-field-initializers -Wold-style-cast")
