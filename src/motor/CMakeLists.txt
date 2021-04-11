cmake_minimum_required(VERSION 3.5)

add_library(motor_lib motor.cpp)

target_link_libraries(motor_lib display_lib engine_lib)
