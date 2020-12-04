cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g")
add_compile_options(-std=c++11 -pthread -g  -ggdb -O0 -Wall   -fstack-protector-all -march=native -fnon-call-exceptions)

project(E-metrics)

include_directories(.)
include_directories(base)
include_directories(metrics)

add_subdirectory(metrics)
add_subdirectory(test)
