#include <iostream>
#include <stdexcept>

#include "kea.h"

void Kea::run() {
    initWindow();
    initVulkan();
    mainLoop();
    cleanup();
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

// Initialize the window with glfw
void Kea::initWindow() {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // No OpenGL context
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);   // No resizing (TODO: change this later!)

    m_window = glfwCreateWindow(WIDTH, HEIGHT, "Kea", nullptr, nullptr);
}


// Initialize Vulkan
void Kea::initVulkan() {
    // Create the info struct for the instance
    VkInstanceCreateInfo create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

    // Set required extensions
    auto exts = getRequiredExtensions();
    create_info.enabledExtensionCount = static_cast<uint32_t>(exts.size());
    create_info.ppEnabledExtensionNames = exts.data();

    // Validation layers
    if (ENABLE_VALIDATION_LAYERS) {
        std::cerr << "Checking" << std::endl;

        // Check if validation layers exist
        if (!checkValidationLayerSupport()) {
            throw std::runtime_error("Validation layers unavailable!");
        }

        // Add validation layers
        create_info.enabledLayerCount = static_cast<uint32_t>(VALIDATION_LAYERS.size());
        create_info.ppEnabledLayerNames = VALIDATION_LAYERS.data();
    } else {
        create_info.enabledLayerCount = 0;
    }

    // Create the actual instance
    if (vkCreateInstance(&create_info, nullptr, &m_vk_instance) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create Vulkan instance!");
    }
}

void Kea::mainLoop() {
    // Main event loop to keep the window open
    while (!glfwWindowShouldClose(m_window)) {
        glfwPollEvents();
    }
}

void Kea::cleanup() {
    vkDestroyInstance(m_vk_instance, nullptr);

    glfwDestroyWindow(m_window);

    glfwTerminate();
}