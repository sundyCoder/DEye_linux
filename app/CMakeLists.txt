
cmake_minimum_required(VERSION 3.7)
project(demo)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-O3 -std=c++11 -pthread -fopenmp -DNDEBUG")

set(SOURCE_FILES_OO demo.cpp DEye.h)
add_executable(demo ${SOURCE_FILES_OO})

# OpenCV libs
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(demo ${OpenCV_LIBS})
target_link_libraries(demo "/usr/local/lib/libCore.so")
target_link_libraries(demo "/usr/local/lib/libDEye.so")
