#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>

const uint32_t WIDTH  = 800;
const uint32_t HEIGHT = 600;

class Kea {
public:
    void run() {
        initWindow();
        initVulkan();
        mainLoop();
        cleanup();
    }

private:
    GLFWwindow* m_window;
    VkInstance  m_vk_instance;

    void initWindow() {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // No OpenGL context
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);   // No resizing (TODO: change this later!)

        m_window = glfwCreateWindow(WIDTH, HEIGHT, "Kea", nullptr, nullptr);
    }

    void initVulkan() {
        // Create the info struct for the instance
        VkInstanceCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

        // glfw helper that determines the extensions needed for Vulkan to talk
        // to the os' window system
        create_info.ppEnabledExtensionNames = glfwGetRequiredInstanceExtensions(&create_info.enabledExtensionCount);

        // Validation layers
        create_info.enabledLayerCount = 0;

        // Create the actual instance
        if (vkCreateInstance(&create_info, nullptr, &m_vk_instance) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create Vulkan instance!");
        }
    }

    void mainLoop() {
        // Main event loop to keep the window open
        while (!glfwWindowShouldClose(m_window)) {
            glfwPollEvents();
        }
    }

    void cleanup() {
        vkDestroyInstance(m_vk_instance, nullptr);

        glfwDestroyWindow(m_window);

        glfwTerminate();
    }

};

int main() {
    Kea kea;

    try {
        kea.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}