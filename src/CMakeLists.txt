cmake_minimum_required(VERSION 3.16)

# Set the project name and version
project(Kea VERSION 0.1)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the executable as target
add_executable(kea main.cpp kea.cpp)

# Dsiable validation layers if in release mode
target_compile_definitions(kea PRIVATE $<$<CONFIG:RELEASE>:DISABLE_VALIDATION_LAYERS>)

# Vulkan
find_package(Vulkan REQUIRED)
target_include_directories(kea PRIVATE ${Vulkan_INCLUDE_DIRS})
target_link_libraries(kea PRIVATE ${Vulkan_LIBRARIES})

# GLFW
find_package(glfw3 REQUIRED)
target_link_libraries(kea PRIVATE glfw)

# # GLM
# find_package(glm CONFIG REQUIRED)
# target_link_libraries(main PRIVATE glm::glm)
