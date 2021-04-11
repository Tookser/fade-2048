cmake_minimum_required(VERSION 3.5)

add_executable(2048_ut 2048_ut.cpp)

target_link_libraries(2048_ut engine_lib display_lib gtest gtest_main gmock gmock_main)

add_test(NAME 2048_tests COMMAND 2048_ut)
