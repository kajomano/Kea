cmake_minimum_required(VERSION 3.29)

# Set the project name and version
project(Kea VERSION 0.1)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# ON if you want to see the commands invoked
set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_FIND_DEBUG_MODE 0)

# Print build type debug/release
message(STATUS ${CMAKE_BUILD_TYPE})

# Add subdirectories
add_subdirectory(bindings)
add_subdirectory(raytracer)

# Compilation options
add_compile_options(/W4)
