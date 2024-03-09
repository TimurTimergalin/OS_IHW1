cmake_minimum_required(VERSION 3.28)
project(os_ihw1 C)

set(CMAKE_C_STANDARD 11)

add_executable(4 4/main.c common.c common.h)

add_executable(5 5/main.c common.c common.h)

add_executable(6 6/main.c common.c common.h)

add_executable(7 7/main.c common.c common.h)

add_executable(8.1 common.c common.h 8/io_worker.c)
add_executable(8.2 common.c common.h 8/logic_worker.c)

add_executable(9.1 common.c common.h 9/io_worker.c)
add_executable(9.2 common.c common.h 9/logic_worker.c)

