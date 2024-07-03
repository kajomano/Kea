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

# TODO: Check if these work on windows
# Compilation options
# # Add warning flags (but remove some problematic ones)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wpedantic -Wextra")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-parameter -Wno-clobbered")

# # Limit max errors printed
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fmax-errors=6")

# # Release: Force optimization level
# set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

# # Debug: Remove optimization, add debug information
# set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g3 -ggdb")
