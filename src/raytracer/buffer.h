#pragma once

#include "vulkan_context.h"

class HostBuffer {
	vk::raii::Buffer in_buffer;
	vk::raii::Buffer out_buffer;
	uint32_t memory_type_index;
	vk::raii::DeviceMemory in_device_memory;
	vk::raii::DeviceMemory out_device_memory;

	vk::raii::Buffer createBuffer(uint32_t n_elements, const std::shared_ptr<VulkanContext> &vulkan_context) const;
	vk::raii::DeviceMemory allocateDeviceMemory(
		const vk::raii::Buffer &buffer, 
		const std::shared_ptr<VulkanContext> &vulkan_context
	) const;

	uint32_t findMemoryTypeIndex(const std::shared_ptr<VulkanContext> &vulkan_context) const;

public:	
	HostBuffer() = delete;
	HostBuffer(uint32_t n_elements, const std::shared_ptr<VulkanContext> &vulkan_context);
};