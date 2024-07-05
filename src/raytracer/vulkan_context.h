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
	vk::raii::Device logical_device;

	vk::raii::Instance createInstance();
	vk::DebugUtilsMessengerCreateInfoEXT createDebugMessengerInfo();
	vk::raii::Device createLogicalDevice();

	std::vector<const char*> getRequiredExtensions();

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
		VkDebugUtilsMessageSeverityFlagBitsEXT       messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT              messageTypes,
		VkDebugUtilsMessengerCallbackDataEXT const * pCallbackData,
		void* pUserData
	) {
        std::cerr << "Vulkan validation layer: " << pCallbackData->pMessage << std::endl;
        return VK_FALSE;
    }

public:
	VulkanContext();
};