#include <iostream>
#include <stdexcept>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <optional>

#define VULKAN_HPP_NO_CONSTRUCTORS // For designated initializers
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_raii.hpp>

#include "shader_test.h"

// https://bakedbits.dev/posts/vulkan-compute-example/
// https://github.com/KhronosGroup/Vulkan-Hpp
// https://gist.github.com/PolarNick239/d4e8cf6645ccb84804afe8d864be3f22

#ifdef DISABLE_VALIDATION_LAYERS
	const bool ENABLE_VALIDATION_LAYERS = false;
#else
	const bool ENABLE_VALIDATION_LAYERS = true;
#endif

const std::vector<const char*> VALIDATION_LAYERS = {
    "VK_LAYER_KHRONOS_validation"
};

// struct QueueFamilyIndices {
//     std::optional<uint32_t> graphicsFamily;

//     bool isComplete() {
//         return graphicsFamily.has_value();
//     }
// };

class RayTracer {
	vk::raii::Context context;
	vk::raii::Instance instance;
	vk::raii::DebugUtilsMessengerEXT debug_messenger;

	vk::raii::Instance createInstance() {
        if(ENABLE_VALIDATION_LAYERS && !checkValidationLayerSupport()) {
            throw std::runtime_error("Validation layers requested, but not available!");
        }

		vk::ApplicationInfo app_info{
			.pApplicationName 	= "Kea",
			.applicationVersion = 1,
			.pEngineName        = "Stargazer",
			.engineVersion      = 1,
			.apiVersion         = vk::ApiVersion10
		};

		auto extensions = getRequiredExtensions();

		vk::InstanceCreateInfo instance_info{
			.pApplicationInfo = &app_info,

			// Extensions
			.enabledExtensionCount = static_cast<uint32_t>(extensions.size()),
			.ppEnabledExtensionNames = extensions.data()
		};

		// Validation layers
		if (ENABLE_VALIDATION_LAYERS) {
			instance_info.setEnabledLayerCount(static_cast<uint32_t>(VALIDATION_LAYERS.size()));
			instance_info.setPEnabledLayerNames(VALIDATION_LAYERS);

			// Add debug messenger info for debug prints during instance creation
			auto debug_messenger_info = createDebugMessengerInfo();
			instance_info.pNext = &debug_messenger_info;
		}

		return vk::raii::Instance(context, instance_info);
    }

	// Create a debug messenger config, which is used in 2 places:
	// 1) To create the debug messenger
	// 2) To pass to instance info so there are debug messages during instance creation too
	vk::DebugUtilsMessengerCreateInfoEXT createDebugMessengerInfo() {
		typedef vk::DebugUtilsMessageSeverityFlagBitsEXT severity;
		typedef vk::DebugUtilsMessageTypeFlagBitsEXT type;

		return vk::DebugUtilsMessengerCreateInfoEXT{
			.messageSeverity	= severity::eWarning | severity::eError,
			.messageType		= type::eGeneral | type::ePerformance |	type::eValidation,
			.pfnUserCallback	= &debugCallback
		};
	}

	// Callback function to print debug messages
	// NOTE: C-style because I think the signature has to be correct
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT       messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT              messageTypes,
		VkDebugUtilsMessengerCallbackDataEXT const * pCallbackData,
		void* pUserData
	) {
        std::cerr << "Vulkan validation layer: " << pCallbackData->pMessage << std::endl;
        return VK_FALSE;
    }

	// Check if the validation layers listed in VALIDATION_LAYERS are supported
	bool checkValidationLayerSupport() {
        std::vector<vk::LayerProperties> available_layers = vk::enumerateInstanceLayerProperties();

        for(const char* layer_name : VALIDATION_LAYERS) {
            bool layer_found = false;

            for(const auto &layer_properties : available_layers) {
                if(strcmp(layer_name, layer_properties.layerName) == 0) {
                    layer_found = true;
                    break;
                }
            }

            if (!layer_found) return false;
        }

        return true;
    }

	std::vector<const char*> getRequiredExtensions() {
        std::vector<const char*> extensions;

        if (ENABLE_VALIDATION_LAYERS) {
            extensions.push_back(vk::EXTDebugUtilsExtensionName);
        }

        return extensions;
    }

public:
	RayTracer()
		: context{}
		, instance{createInstance()}
		, debug_messenger{vk::raii::DebugUtilsMessengerEXT(instance, createDebugMessengerInfo())}
	{};

    void run() {

    }

private:
    // VkInstance instance;
	// VkDebugUtilsMessengerEXT debugMessenger;
	// VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	// VkDevice device;
    // VkQueue graphicsQueue;

    void initVulkan() {
        createInstance();
		// setupDebugMessenger();
		// pickPhysicalDevice();
		// createLogicalDevice();
    }

    // void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
    //     createInfo = {};
    //     createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    //     createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    //     createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    //     createInfo.pfnUserCallback = debugCallback;
    // }

    // void setupDebugMessenger() {
    //     if (!enableValidationLayers) return;

    //     VkDebugUtilsMessengerCreateInfoEXT createInfo;
    //     populateDebugMessengerCreateInfo(createInfo);

    //     if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
    //         throw std::runtime_error("Failed to set up debug messenger!");
    //     }
    // }

	// void pickPhysicalDevice() {
    //     uint32_t deviceCount = 0;
    //     vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    //     if (deviceCount == 0) {
    //         throw std::runtime_error("Failed to find GPUs with Vulkan support!");
    //     }

    //     std::vector<VkPhysicalDevice> devices(deviceCount);
    //     vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    //     for (const auto& device : devices) {
    //         if (isDeviceSuitable(device)) {
    //             physicalDevice = device;
    //             break;
    //         }
    //     }

    //     if (physicalDevice == VK_NULL_HANDLE) {
    //         throw std::runtime_error("Failed to find a suitable GPU!");
    //     }
    // }

   	// void createLogicalDevice() {
    //     QueueFamilyIndices indices = findQueueFamilies(physicalDevice);

    //     VkDeviceQueueCreateInfo queueCreateInfo{};
    //     queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    //     queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
    //     queueCreateInfo.queueCount = 1;

    //     float queuePriority = 1.0f;
    //     queueCreateInfo.pQueuePriorities = &queuePriority;

    //     VkPhysicalDeviceFeatures deviceFeatures{};

    //     VkDeviceCreateInfo createInfo{};
    //     createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    //     createInfo.pQueueCreateInfos = &queueCreateInfo;
    //     createInfo.queueCreateInfoCount = 1;

    //     createInfo.pEnabledFeatures = &deviceFeatures;

    //     createInfo.enabledExtensionCount = 0;

    //     if (enableValidationLayers) {
    //         createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
    //         createInfo.ppEnabledLayerNames = validationLayers.data();
    //     } else {
    //         createInfo.enabledLayerCount = 0;
    //     }

    //     if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
    //         throw std::runtime_error("failed to create logical device!");
    //     }

    //     vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
    // }

    // bool isDeviceSuitable(VkPhysicalDevice device) {
    //     QueueFamilyIndices indices = findQueueFamilies(device);

    //     return indices.isComplete();
    // }

    // QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) {
    //     QueueFamilyIndices indices;

    //     uint32_t queueFamilyCount = 0;
    //     vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    //     std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    //     vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    //     int i = 0;
    //     for (const auto& queueFamily : queueFamilies) {
    //         if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
    //             indices.graphicsFamily = i;
    //         }

    //         if (indices.isComplete()) {
    //             break;
    //         }

    //         i++;
    //     }

    //     return indices;
    // }

};

int shaderTest() {
	RayTracer app;

    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}