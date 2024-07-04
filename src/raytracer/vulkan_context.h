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
	bool checkValidationLayerSupport();
	// std::vector<const char*> getRequiredExtensions();
	vk::raii::Device createLogicalDevice();

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