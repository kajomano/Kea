#pragma once

#include "vulkan_context.h"

class Pipeline {
	vk::raii::ShaderModule shader_module;

	vk::raii::ShaderModule createShaderModule(const std::string &file_name, const std::shared_ptr<VulkanContext> &vulkan_context) const;

public:	
	Pipeline() = delete;
	Pipeline(const std::shared_ptr<VulkanContext> &vulkan_context);
};