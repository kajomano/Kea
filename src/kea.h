#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "consts.h"

class Kea {
public:
    void run();

private:
    // Main functions (called by run)
    void initWindow();
    void initVulkan();
    void mainLoop();
    void cleanup();

    // Helper functions
    bool checkValidationLayerSupport();
    std::vector<const char*> getRequiredExtensions();
    void createVulkanInstance();
    void setupDebugMessenger();
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& create_info);

    GLFWwindow* m_window;
    VkInstance  m_vk_instance;
    VkDebugUtilsMessengerEXT m_debug_messenger;
};