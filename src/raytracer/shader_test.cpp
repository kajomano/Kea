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

class RayTracer {
	vk::raii::Context context;
	vk::raii::Instance instance;
	vk::raii::DebugUtilsMessengerEXT debug_messenger;
	vk::raii::PhysicalDevice physical_device;
	vk::raii::Device logical_device;

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

	vk::raii::Device createLogicalDevice() {
		// Find a queue that supports compute
		std::vector<vk::QueueFamilyProperties> qf_properties = physical_device.getQueueFamilyProperties();
		auto prop_it = std::find_if(qf_properties.begin(), qf_properties.end(), 
			[](const vk::QueueFamilyProperties& properties) {
				return properties.queueFlags & vk::QueueFlagBits::eCompute;
			}
		);
		if (prop_it == qf_properties.end()) {
			throw std::runtime_error("Couldn't find queue family that supports compute!");
		}

		float q_prio = 1.0f;
		vk::DeviceQueueCreateInfo dq_info{
			.queueFamilyIndex = static_cast<uint32_t>(std::distance(qf_properties.begin(), prop_it)),
			.queueCount = 1,
			.pQueuePriorities = &q_prio
		};

		vk::DeviceCreateInfo device_info{
			.queueCreateInfoCount = 1,
			.pQueueCreateInfos = &dq_info
		};

		if(ENABLE_VALIDATION_LAYERS) {
			device_info.setEnabledLayerCount(static_cast<uint32_t>(VALIDATION_LAYERS.size()));
			device_info.setPEnabledLayerNames(VALIDATION_LAYERS);
		}

		return physical_device.createDevice(device_info);
    }

public:
	RayTracer()
		: context{}
		, instance{createInstance()}
		, debug_messenger{vk::raii::DebugUtilsMessengerEXT(instance, createDebugMessengerInfo())}
		, physical_device{instance.enumeratePhysicalDevices().front()}
		, logical_device{createLogicalDevice()}
	{};

    void run() {

    }
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