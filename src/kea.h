#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "consts.h"

class Kea {
public:
    void run();

private:
    GLFWwindow* m_window;
    VkInstance  m_vk_instance;

    // Helper functions
    bool checkValidationLayerSupport();
    std::vector<const char*> getRequiredExtensions();

    // Main functions (called by run)
    void initWindow();
    void initVulkan();
    void mainLoop();
    void cleanup();
};