# Since we are compiling libraries, turn on position independent code
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Define library
add_library(raytracer)

# Add sources
target_sources(raytracer
  PUBLIC
  	vulkan_context.h
	buffer.h

  	shader_test.h

  PRIVATE
	vulkan_context.cpp
	buffer.cpp

  	shader_test.cpp
)

# Expose the headers for whoever links against this lib
target_include_directories(raytracer PUBLIC ${CMAKE_CURRENT_LIST_DIR})

# Vulkan
# Disable validation layers if in release mode
target_compile_definitions(raytracer PRIVATE $<$<CONFIG:RELEASE>:DISABLE_VALIDATION_LAYERS>)

# Find package
find_package(Vulkan REQUIRED)
target_include_directories(raytracer PRIVATE ${Vulkan_INCLUDE_DIRS})
target_link_libraries(raytracer PRIVATE ${Vulkan_LIBRARIES})