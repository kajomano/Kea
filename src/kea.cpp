#include <iostream>
#include <stdexcept>

#include "kea.h"

// Callback function to Vulkan debug messages
static VKAPI_ATTR VkBool32 VKAPI_CALL vulkanDebugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
    VkDebugUtilsMessageTypeFlagsEXT message_type,
    const VkDebugUtilsMessengerCallbackDataEXT* p_callback_data,
    void* p_user_data) {

    std::cerr << "Validation layer: " << p_callback_data->pMessage << std::endl;

    return VK_FALSE;
}

// Proxy function to create debug messenger
VkResult createDebugUtilsMessengerEXT(
    VkInstance instance,
    const VkDebugUtilsMessengerCreateInfoEXT* p_create_info,
    const VkAllocationCallbacks* p_allocator,
    VkDebugUtilsMessengerEXT* p_debug_messenger
) {
    // Get the address of the vkCreateDebugUtilsMessengerEXT function
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, p_create_info, p_allocator, p_debug_messenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

// Proxy function to destroy debug messenger
void destroyDebugUtilsMessengerEXT(
    VkInstance instance, 
    VkDebugUtilsMessengerEXT p_debug_messenger,
    const VkAllocationCallbacks* p_allocator
) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, p_debug_messenger, p_allocator);
    }
}

// Main function
void Kea::run() {
    initWindow();
    initVulkan();
    mainLoop();
    cleanup();
}

// Initialize the window with glfw
void Kea::initWindow() {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // No OpenGL context
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);   // No resizing (TODO: change this later!)

    m_window = glfwCreateWindow(WIDTH, HEIGHT, "Kea", nullptr, nullptr);
}

void Kea::initVulkan() {
    createVulkanInstance();
    setupDebugMessenger();
}

void Kea::mainLoop() {
    // Main event loop to keep the window open
    while (!glfwWindowShouldClose(m_window)) {
        glfwPollEvents();
    }
}

void Kea::cleanup() {
    if (ENABLE_VALIDATION_LAYERS) {
        destroyDebugUtilsMessengerEXT(m_vk_instance, m_debug_messenger, nullptr);
    }
    vkDestroyInstance(m_vk_instance, nullptr);

    glfwDestroyWindow(m_window);
    glfwTerminate();
}

// Check if the set validation layers are supported
bool Kea::checkValidationLayerSupport() {
    uint32_t layer_cnt;
    vkEnumerateInstanceLayerProperties(&layer_cnt, nullptr);

    std::vector<VkLayerProperties> layers(layer_cnt);
    vkEnumerateInstanceLayerProperties(&layer_cnt, layers.data());

    for (const char* layer_name : VALIDATION_LAYERS) {
        bool layer_found = false;

        for (const auto& layer_props : layers) {
            if (strcmp(layer_name, layer_props.layerName) == 0) {
                layer_found = true;
                break;
            }
        }

        if (!layer_found) {
            return false;
        }
    }

    return true;
}

// Collect the required extension names
std::vector<const char*> Kea::getRequiredExtensions() {
    // GLFW extensions are mandatory for window creation
    uint32_t glfw_ext_cnt = 0;
    const char** glfw_exts;
    glfw_exts = glfwGetRequiredInstanceExtensions(&glfw_ext_cnt);

    // Large extensions vector
    std::vector<const char*> exts(glfw_exts, glfw_exts + glfw_ext_cnt);

    // If validation layers are also enabled, need debug utils
    if (ENABLE_VALIDATION_LAYERS) {
        exts.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return exts;
}

// Create VUlkan instance
void Kea::createVulkanInstance() {
    // Create the info struct for the instance
    VkInstanceCreateInfo create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

    // Set required extensions
    auto exts = getRequiredExtensions();
    create_info.enabledExtensionCount = static_cast<uint32_t>(exts.size());
    create_info.ppEnabledExtensionNames = exts.data();

    // Validation layers
    VkDebugUtilsMessengerCreateInfoEXT debug_create_info{};
    if (ENABLE_VALIDATION_LAYERS) {
        std::cerr << "Checking" << std::endl;

        // Check if validation layers exist
        if (!checkValidationLayerSupport()) {
            throw std::runtime_error("Validation layers unavailable!");
        }

        // Add validation layers
        create_info.enabledLayerCount = static_cast<uint32_t>(VALIDATION_LAYERS.size());
        create_info.ppEnabledLayerNames = VALIDATION_LAYERS.data();

        populateDebugMessengerCreateInfo(debug_create_info);
        create_info.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debug_create_info;
    } else {
        create_info.enabledLayerCount = 0;
        create_info.pNext = nullptr;
    }

    // Create the actual instance
    if (vkCreateInstance(&create_info, nullptr, &m_vk_instance) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create Vulkan instance!");
    }
}

void Kea::setupDebugMessenger() {
    if (!ENABLE_VALIDATION_LAYERS) return;

    VkDebugUtilsMessengerCreateInfoEXT create_info{};
    populateDebugMessengerCreateInfo(create_info);


    if (createDebugUtilsMessengerEXT(m_vk_instance, &create_info, nullptr, &m_debug_messenger) != VK_SUCCESS) {
        throw std::runtime_error("Failed to set up debug messenger!");
    }
}

void Kea::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& create_info) {
    create_info.sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    // All severities to react to with the callback
    create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    // All types to react to with the callback
    create_info.messageType     = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    create_info.pfnUserCallback = vulkanDebugCallback;
    create_info.pUserData       = nullptr;
}