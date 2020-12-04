cmake_minimum_required(VERSION 3.8)

aux_source_directory(./src metrics_src)

add_executable(metrics_test ${metrics_src})

add_dependencies(metrics_test metrics)
target_link_libraries(metrics_test PRIVATE metrics gtest gtest_main pthread)
