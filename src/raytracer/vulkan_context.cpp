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

// Vulkan extension list
const std::vector<const char*> EXTENSIONS = {};

/**
 * @brief Create a Vulkan instance
 * 
 * @return vk::raii::Instance
 */
vk::raii::Instance VulkanContext::createInstance() const {
	// Application info
	vk::ApplicationInfo app_info{
		.pApplicationName 	= "Kea",
		.applicationVersion = 1,
		.pEngineName        = "Stargazer",
		.engineVersion      = 1,
		.apiVersion         = vk::ApiVersion11
	};

	// Extensions
	auto extensions = getRequiredExtensions();

	// Instance info
	vk::InstanceCreateInfo instance_info{
		.pApplicationInfo 			= &app_info,
		.enabledExtensionCount 		= static_cast<uint32_t>(extensions.size()),
		.ppEnabledExtensionNames 	= extensions.data()
	};

	// Enable validation layers if needed
	if (ENABLE_VALIDATION_LAYERS) {
		// Validation layers
		instance_info.setEnabledLayerCount(static_cast<uint32_t>(VALIDATION_LAYERS.size()));
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
vk::DebugUtilsMessengerCreateInfoEXT VulkanContext::createDebugMessengerInfo() const {
	typedef vk::DebugUtilsMessageSeverityFlagBitsEXT severity;
	typedef vk::DebugUtilsMessageTypeFlagBitsEXT type;

	return vk::DebugUtilsMessengerCreateInfoEXT{
		.messageSeverity	= severity::eWarning | severity::eError,
		.messageType		= type::eGeneral | type::ePerformance |	type::eValidation,
		.pfnUserCallback	= &debugCallback
	};
}

/**
 * @brief Create a physical vulkan device
 * 
 * @return vk::raii::PhysicalDevice 
 */
vk::raii::PhysicalDevice VulkanContext::createPhysicalDevice() const {
	// TODO: Select a device that supports the requirements (if there are multiple devices)
	vk::raii::PhysicalDevice physical_device = instance.enumeratePhysicalDevices().front();

	// Stats
	vk::PhysicalDeviceProperties device_props = physical_device.getProperties();
	const uint32_t api_version = device_props.apiVersion;
	vk::PhysicalDeviceLimits device_limits = device_props.limits;

	// Print
	std::cout << "Device name               : " << device_props.deviceName << std::endl;
	std::cout << "Vulkan version            : " << vk::versionMajor(api_version) << "." << vk::versionMinor(api_version) << "." << vk::versionPatch(api_version) << std::endl;
	std::cout << "Max Compute Shared Memory : " << device_limits.maxComputeSharedMemorySize / 1024 << " KB" << std::endl;

	return physical_device;
}

/**
 * @brief Create logical vulkan device
 * 
 * @return vk::raii::Device 
 */
vk::raii::Device VulkanContext::createLogicalDevice() const {
	// Device queue info
	float q_prio = 1.0f;
	vk::DeviceQueueCreateInfo dq_info{
		.queueFamilyIndex 	= qf_index,
		.queueCount 		= 1,
		.pQueuePriorities 	= &q_prio
	};

	// Logical device info
	vk::DeviceCreateInfo device_info{
		.queueCreateInfoCount 	= 1,
		.pQueueCreateInfos 		= &dq_info
	};

	// Enable validation layers if needed
	if(ENABLE_VALIDATION_LAYERS) {
		device_info.setEnabledLayerCount(static_cast<uint32_t>(VALIDATION_LAYERS.size()));
		device_info.setPEnabledLayerNames(VALIDATION_LAYERS);
	}

	return physical_device.createDevice(device_info);
}

/**
 * @brief Find the queue family index that supports compute
 * 
 * @return uint32_t 
 */
uint32_t VulkanContext::findQueueFamilyIndex() const {
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

	// Return the queue family index
	return static_cast<uint32_t>(std::distance(qf_properties.begin(), prop_it));
}

/**
 * @brief Collect a list of required extensions
 * 
 * @return std::vector<const char*> 
 */
std::vector<const char*> VulkanContext::getRequiredExtensions() const {
	std::vector<const char*> extensions = EXTENSIONS;

	// Enable debug utils extensions if validation layers are used
    if (ENABLE_VALIDATION_LAYERS) extensions.push_back(vk::EXTDebugUtilsExtensionName);

    return extensions;
}

/**
 * @brief Construct a new Vulkan Context:: Vulkan Context object
 * 
 */
VulkanContext::VulkanContext()
	: context{}
	, instance{createInstance()}
	, debug_messenger{vk::raii::DebugUtilsMessengerEXT(instance, createDebugMessengerInfo())}
	, physical_device{createPhysicalDevice()}
	, qf_index{findQueueFamilyIndex()}
	, logical_device{createLogicalDevice()}
{}
