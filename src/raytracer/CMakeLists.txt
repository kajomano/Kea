# Since we are compiling libraries, turn on position independent code
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Define library
add_library(Raytracer)

# Add sources
target_sources(Raytracer
  PUBLIC
  	buffer.h
	pipeline.h
  	vulkan_context.h

  	shader_test.h

  PRIVATE
  	buffer.cpp
	pipeline.cpp
	vulkan_context.cpp	

  	shader_test.cpp
)

# Expose the headers for whoever links against this lib
target_include_directories(Raytracer PUBLIC ${CMAKE_CURRENT_LIST_DIR})

# Vulkan
# Disable validation layers if in release mode
target_compile_definitions(Raytracer PRIVATE $<$<CONFIG:RELEASE>:DISABLE_VALIDATION_LAYERS>)

# Find package
find_package(Vulkan REQUIRED)
target_include_directories(Raytracer PRIVATE ${Vulkan_INCLUDE_DIRS})
target_link_libraries(Raytracer PRIVATE ${Vulkan_LIBRARIES})

# Compile compute shaders (GLSL)
set(SHADERS_IN_DIR ${CMAKE_CURRENT_LIST_DIR}/shaders)
set(SHADERS_OUT_DIR ${CMAKE_BINARY_DIR}/shaders)

file(GLOB SHADERS ${SHADERS_IN_DIR}/*.comp)
file(MAKE_DIRECTORY ${SHADERS_OUT_DIR})

foreach(SHADER ${SHADERS})
	get_filename_component(SHADER_NAME ${SHADER} NAME_WLE)
	set(SHADER_OUT_NAME ${SHADERS_OUT_DIR}/${SHADER_NAME}.spv)
	list(APPEND SHADER_OUT_NAMES ${SHADER_OUT_NAME})
	add_custom_command(
		MAIN_DEPENDENCY ${SHADER}
		OUTPUT ${SHADER_OUT_NAME}
		COMMAND ${Vulkan_GLSLC_EXECUTABLE} ${SHADER} "-o" ${SHADER_OUT_NAME}
		VERBATIM)
endforeach()

add_custom_target(BuildShaders DEPENDS ${SHADER_OUT_NAMES})
add_dependencies(Raytracer BuildShaders)