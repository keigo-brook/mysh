cmake_minimum_required(VERSION 3.3)
project(mysh)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -o ./mysh")

set(SOURCE_FILES main.c mysub.c mysub.h my_path.c main.h)
add_executable(mysh ${SOURCE_FILES})