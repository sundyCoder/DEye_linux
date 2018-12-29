
cmake_minimum_required(VERSION 3.7)
project(tf_detector_example)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-O3 -std=c++11 -pthread -lprotobuf -DNDEBUG")

# Find source files
file(GLOB SOURCES src/*.cpp src/*.c)

# Include header files
include_directories(src/)
include_directories(src/include)
include_directories(src/internal)

# Create shared library
add_library(DEye SHARED ${SOURCES})

# OpenCV libs
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
#target_link_libraries(tf_detector_example ${OpenCV_LIBS})


# ==================== PATHS TO SPECIFY! ==================== #
# Eigen lib headers
include_directories("/media/jlchen/C/3rd_party/eigen3/include/eigen3")
include_directories("/media/jlchen/C/3rd_party/eigen3/include/eigen3")


# TensorFlow headers
include_directories("/usr/local/include/tf/")
include_directories("/usr/local/include/tf/bazel-genfiles/")
include_directories("/usr/local/include/tf/tensorflow/")
include_directories("/usr/local/include/tf/third-party/")
include_directories("/usr/local/include/tf/tensorflow/contrib/makefile/downloads/absl/")

# Link TensorFlow libs
target_link_libraries(DEye "/usr/local/lib/libCore.so")
target_link_libraries(DEye "/usr/local/lib/libprotobuf.so")
