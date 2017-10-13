cmake_minimum_required(VERSION 3.8)
project(ISA)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES isamon.c isamon.h)
add_executable(ISA ${SOURCE_FILES})