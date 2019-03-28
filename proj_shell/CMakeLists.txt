cmake_minimum_required(VERSION 3.13)

project(shell)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin)
add_executable(shell src/shell.c)