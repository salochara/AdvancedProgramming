cmake_minimum_required(VERSION 3.14)
project(AdvancedProgramming C)

set(CMAKE_C_STANDARD 11)

include_directories(DataTypes )

add_executable(AdvancedProgramming
    DataTypes/datatypes.c
    DataTypes/datatypes.h
    DataTypes/main.c)

add_executable(basics basics.c)

add_executable(AdvancedProgramminghomework-1-arrays homework-1-arrays/hw.c)


