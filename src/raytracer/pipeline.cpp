#include <fstream>
#include <filesystem>

#include "pipeline.h"

/**
 * @brief Create a Shader Module object
 * 
 * @param vulkan_context 
 * @return vk::raii::ShaderModule 
 */
vk::raii::ShaderModule Pipeline::createShaderModule(const std::string &file_name, const std::shared_ptr<VulkanContext> &vulkan_context) const {
	auto &logical_device = vulkan_context->getLogicalDevice();

	// Read the SPIR-V file contents
	auto shader_file_path = std::filesystem::path(__FILE__).parent_path().parent_path().parent_path() / "build" / "shaders" / file_name;
	std::vector<char> shader_contents;
	
	if(std::ifstream shader_file{shader_file_path, std::ios::binary | std::ios::ate}) {
		const size_t file_size = shader_file.tellg();
		shader_file.seekg(0);
		shader_contents.resize(file_size, '\0');
		shader_file.read(shader_contents.data(), file_size);
	} else {
		throw std::runtime_error("Couldn't read shader file!"); 
	}

	// Create shader module info
	vk::ShaderModuleCreateInfo shader_module_info{
		.codeSize 	= shader_contents.size(),
		.pCode		= reinterpret_cast<const uint32_t*>(shader_contents.data())
	};

	// Create shader module and return
	return logical_device.createShaderModule(shader_module_info);
}

/**
 * @brief Construct a new Pipeline:: Pipeline object
 * 
 * @param vulkan_context 
 */
Pipeline::Pipeline(const std::shared_ptr<VulkanContext> &vulkan_context)
	: shader_module{createShaderModule("square.spv", vulkan_context)}
{}