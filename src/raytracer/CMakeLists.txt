# Since we are compiling libraries, turn on position independent code
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Define library
add_library(raytracer)

# Add sources
target_sources(raytracer
  PUBLIC
	shader_test.h

  PRIVATE
    shader_test.cpp
)

target_include_directories(raytracer PUBLIC ${CMAKE_CURRENT_LIST_DIR})

# External dependencies
# # Disable validation layers if in release mode
# target_compile_definitions(kea PRIVATE $<$<CONFIG:RELEASE>:DISABLE_VALIDATION_LAYERS>)

# # Vulkan
# find_package(Vulkan REQUIRED)
# target_include_directories(kea PRIVATE ${Vulkan_INCLUDE_DIRS})
# target_link_libraries(kea PRIVATE ${Vulkan_LIBRARIES})



