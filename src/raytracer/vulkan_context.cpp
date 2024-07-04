#include "vulkan_context.h"

// Validation layer switch
#ifdef DISABLE_VALIDATION_LAYERS
	const bool ENABLE_VALIDATION_LAYERS = false;
#else
	const bool ENABLE_VALIDATION_LAYERS = true;
#endif

// Vulkan validation layer list
const std::vector<const char*> VALIDATION_LAYERS = {
    "VK_LAYER_KHRONOS_validation"
};

// Vulkan debug extension list
const std::vector<const char*> EXTENSIONS = {
    vk::EXTDebugUtilsExtensionName
};

/**
 * @brief Create a Vulkan instance
 * 
 * @return vk::raii::Instance 
 */
vk::raii::Instance VulkanContext::createInstance() {
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

	vk::InstanceCreateInfo instance_info{
		.pApplicationInfo = &app_info
	};

	if (ENABLE_VALIDATION_LAYERS) {
		// Extensions
		instance_info.setEnabledExtensionCount(1);
		instance_info.setPEnabledExtensionNames(EXTENSIONS);

		// Validation layers
		instance_info.setEnabledLayerCount(1);
		instance_info.setPEnabledLayerNames(VALIDATION_LAYERS);

		// Add debug messenger info for debug prints during instance creation
		auto debug_messenger_info = createDebugMessengerInfo();
		instance_info.pNext = &debug_messenger_info;
	}

	return vk::raii::Instance(context, instance_info);
}

/**
 * @brief Create a debug messenger config, which is used in 2 places:
 * 1) To create the debug messenger
 * 2) To pass to instance info so there are debug messages during instance creation too
 * 
 */
vk::DebugUtilsMessengerCreateInfoEXT VulkanContext::createDebugMessengerInfo() {
	typedef vk::DebugUtilsMessageSeverityFlagBitsEXT severity;
	typedef vk::DebugUtilsMessageTypeFlagBitsEXT type;

	return vk::DebugUtilsMessengerCreateInfoEXT{
		.messageSeverity	= severity::eWarning | severity::eError,
		.messageType		= type::eGeneral | type::ePerformance |	type::eValidation,
		.pfnUserCallback	= &debugCallback
	};
}

/**
 * @brief Check if the validation layers listed in VALIDATION_LAYERS are supported
 * 
 * @return true 
 * @return false 
 */
bool VulkanContext::checkValidationLayerSupport() {
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

/**
 * @brief Create logical vulkan device
 * 
 * @return vk::raii::Device 
 */
vk::raii::Device VulkanContext::createLogicalDevice() {
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
		device_info.setEnabledLayerCount(1);
		device_info.setPEnabledLayerNames(VALIDATION_LAYERS);
	}

	return physical_device.createDevice(device_info);
}

/**
 * @brief Construct a new Vulkan Context:: Vulkan Context object
 * 
 */
VulkanContext::VulkanContext()
	: context{}
	, instance{createInstance()}
	, debug_messenger{vk::raii::DebugUtilsMessengerEXT(instance, createDebugMessengerInfo())}
	, physical_device{instance.enumeratePhysicalDevices().front()}
	, logical_device{createLogicalDevice()}
{};
