cmake_minimum_required(VERSION 3.13)
project(task1)

set(CMAKE_CXX_STANDARD 20)

add_executable(main main.cpp includes/lexer.cpp includes/parser.cpp includes/ASTNode.cpp)