#pragma once

#include <iostream>

#define VULKAN_HPP_NO_CONSTRUCTORS // For designated initializers
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_raii.hpp>

class VulkanContext {
	vk::raii::Context context;
	vk::raii::Instance instance;
	vk::raii::DebugUtilsMessengerEXT debug_messenger;
	vk::raii::PhysicalDevice physical_device;
	uint32_t qf_index;
	vk::raii::Device logical_device;	

	vk::raii::Instance createInstance() const;
	vk::DebugUtilsMessengerCreateInfoEXT createDebugMessengerInfo() const;
	vk::raii::PhysicalDevice createPhysicalDevice() const;
	vk::raii::Device createLogicalDevice() const;

	uint32_t findQueueFamilyIndex() const;
	std::vector<const char*> getRequiredExtensions() const;

	/**
	 * @brief Callback function for debug prints
	 * 
	 * @param messageSeverity 
	 * @param messageTypes 
	 * @param pCallbackData 
	 * @param pUserData 
	 * @return VKAPI_ATTR 
	 */
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageTypes,
		VkDebugUtilsMessengerCallbackDataEXT const* pCallbackData,
		void* pUserData
	) {
        std::cerr << "Vulkan validation layer: " << pCallbackData->pMessage << std::endl;
        return VK_FALSE;
    }

public:
	VulkanContext();

	uint32_t getQueueFamilyIndex() {return qf_index;};
	vk::raii::PhysicalDevice& getPhysicalDevice() {return physical_device;};
	vk::raii::Device& getLogicalDevice() {return logical_device;};
};