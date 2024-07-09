#include "buffer.h"
#include "pipeline.h"
#include "vulkan_context.h"

#include "shader_test.h"

int shaderTest() {
	auto vulkan_context = std::make_shared<VulkanContext>();

	HostBuffer{10, vulkan_context};
	Pipeline{vulkan_context};

    return EXIT_SUCCESS;
}