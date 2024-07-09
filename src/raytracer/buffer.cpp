#include "buffer.h"

/**
 * @brief Create a buffer of specific size
 * 
 * @param n_elements 
 * @param vulkan_context 
 * @return vk::raii::Buffer 
 */
vk::raii::Buffer HostBuffer::createBuffer(uint32_t n_elements, const std::shared_ptr<VulkanContext> &vulkan_context) const {
	uint32_t qf_index = vulkan_context->getQueueFamilyIndex();
	auto &logical_device = vulkan_context->getLogicalDevice();

	// Create the buffer info
	vk::BufferCreateInfo buffer_info{
		.size 					= n_elements * sizeof(int32_t),
		.usage 					= vk::BufferUsageFlagBits::eStorageBuffer,
		.sharingMode 			= vk::SharingMode::eExclusive,
		.queueFamilyIndexCount 	= 1,
		.pQueueFamilyIndices 	= &qf_index
	};

	// Create the buffer and return
	return logical_device.createBuffer(buffer_info);
}

/**
 * @brief 
 * 
 * @param vulkan_context 
 * @return vk::raii::DeviceMemory 
 */
vk::raii::DeviceMemory HostBuffer::allocateDeviceMemory(
	const vk::raii::Buffer &buffer, 
	const std::shared_ptr<VulkanContext> &vulkan_context
) const {
	auto &logical_device = vulkan_context->getLogicalDevice();

	// Create memory allocation info
	vk::MemoryAllocateInfo mem_alloc_info{
		.allocationSize 	= buffer.getMemoryRequirements().size,
		.memoryTypeIndex 	= memory_type_index
	};

	// Allocate memory and return
	return logical_device.allocateMemory(mem_alloc_info);
}

/**
 * @brief Find the memroy type index for the allocated memory
 * 
 * @param vulkan_context 
 * @return uint32_t 
 */
uint32_t HostBuffer::findMemoryTypeIndex(const std::shared_ptr<VulkanContext> &vulkan_context) const {
	auto &physical_device = vulkan_context->getPhysicalDevice();

	// Query the memory types and select a fitting one
	vk::PhysicalDeviceMemoryProperties mem_properties = physical_device.getMemoryProperties();

	// Iterate over all memory types
	uint32_t mem_type_id;
	vk::DeviceSize mem_heap_size;
	for(mem_type_id = 0; mem_type_id <= mem_properties.memoryTypeCount; ++mem_type_id) {
		// Throw exception if none of the memory types were selected
		if(mem_type_id == mem_properties.memoryTypeCount) throw std::runtime_error("Couldn't find fitting memory type!");

		vk::MemoryType mem_type = mem_properties.memoryTypes[mem_type_id];

		// If the memory is visible to the host and coherent, select it
		if(
			(vk::MemoryPropertyFlagBits::eHostVisible & mem_type.propertyFlags) &&
			(vk::MemoryPropertyFlagBits::eHostCoherent & mem_type.propertyFlags)
		) {
			mem_heap_size = mem_properties.memoryHeaps[mem_type.heapIndex].size;
			break;
		}
	}

	// Print
	std::cout << "Memory Type Index         : " << mem_type_id << std::endl;
	std::cout << "Memory Heap Size          : " << mem_heap_size / 1024 / 1024 / 1024 << " GB" << std::endl;

	// Return the memory type id
	return mem_type_id;
}

/**
 * @brief Construct a new Host Buffer:: Host Buffer object
 * 
 * @param n_elements 
 * @param vulkan_context 
 */
HostBuffer::HostBuffer(uint32_t n_elements, const std::shared_ptr<VulkanContext> &vulkan_context)
	: in_buffer{createBuffer(n_elements, vulkan_context)}
	, out_buffer{createBuffer(n_elements, vulkan_context)}
	, memory_type_index{findMemoryTypeIndex(vulkan_context)}
	, in_device_memory{allocateDeviceMemory(in_buffer, vulkan_context)}
	, out_device_memory{allocateDeviceMemory(out_buffer, vulkan_context)}
{
	// ITT
	// TODO: look up what this is
	// // Get a mapped pointer with which to copy data from host to device
	// int32_t* InBufferPtr = static_cast<int32_t*>(Device.mapMemory(InBufferMemory, 0, BufferSize));
	// for (int32_t I = 0; I < NumElements; ++I)
	// {
	// 	InBufferPtr[I] = I;
	// }
	// Device.unmapMemory(InBufferMemory);

	// Bind the buffers to the deived memories
	in_buffer.bindMemory(in_device_memory, 0);
	out_buffer.bindMemory(out_device_memory, 0);
}