cmake_minimum_required(VERSION 3.7)
project(template_crush_saga)

SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++1z" )

set(SOURCE_FILES main.cpp constexpr_string.hpp board.hpp utils.hpp)
add_executable(template_crush_saga ${SOURCE_FILES})