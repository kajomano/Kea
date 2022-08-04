# Installation (Windows):
- Vulkan:
  - Download and install: https://vulkan.lunarg.com/
- VCPKG:
  - git clone https://github.com/Microsoft/vcpkg.git --config "http.proxy=http://10.144.1.10:8080"
  - .\vcpkg\bootstrap-vcpkg.bat
  - .\vcpkg\vcpkg.exe --triplet x64-windows install glfw3 
  - .\vcpkg\vcpkg.exe --triplet x64-windows install glm
  NOTE: if it does not want to work, delet the build directory and try again


# Installation (Ubuntu):
- Vulkan:
  - sudo apt install vulkan-tools
  - sudo apt install libvulkan-dev
  - sudo apt install vulkan-validationlayers-dev spirv-tools
- GLFW:
  - sudo apt install libglfw3-dev
- GLM:
  - sudo apt install libglm-dev
- GLSLC:
  - Download: https://github.com/google/shaderc/blob/main/downloads.md
  - Copy the glslc binary to /usr/local/bin
