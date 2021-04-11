cmake_minimum_required(VERSION 3.5)

set(CMAKE_BUILD_TYPE Debug)

project(2048IB)

include(CTest)

include_directories(${CMAKE_SOURCE_DIR})

set(CMAKE_CXX_STANDARD 17)

# for gmock on cygwin on windows 10
add_definitions( -DGTEST_HAS_PTHREAD=1 )

add_subdirectory(bin)
add_subdirectory(motor)
add_subdirectory(googletest)
add_subdirectory(display)
add_subdirectory(engine)

add_subdirectory(ut)

IF(WIN32)
    add_subdirectory(glfw-3.2.1)
ENDIF()
