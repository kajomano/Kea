#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>

const uint32_t WIDTH  = 800;
const uint32_t HEIGHT = 600;

const std::vector<const char*> VALIDATION_LAYERS = {
    "VK_LAYER_KHRONOS_validation"
};

#ifdef DISABLE_VALIDATION_LAYERS
    const bool ENABLE_VALIDATION_LAYERS = false;
#else
    const bool ENABLE_VALIDATION_LAYERS = true;
#endif

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

    bool checkValidationLayerSupport() {
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

    void initVulkan() {
        // Create the info struct for the instance
        VkInstanceCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

        // glfw helper that determines the extensions needed for Vulkan to talk
        // to the os' window system
        create_info.ppEnabledExtensionNames = glfwGetRequiredInstanceExtensions(&create_info.enabledExtensionCount);

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