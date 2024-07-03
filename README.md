[To view this document in a nicely rendered format, shift+ctrl+v in VS Code]::
[Syntax guide at: https://www.markdownguide.org/basic-syntax/]::

# KEA repository

## Build environment setup (Windows)

- CMAKE
	- Download installer from [here](https://cmake.org/)
	- Add CMAKE to the PATH when installing!

- Visual Studio
	- Download installer from [here](https://visualstudio.microsoft.com/thank-you-downloading-visual-studio/?sku=Community&channel=Release)

- VCKPG
	```
	git clone https://github.com/Microsoft/vcpkg.git
	cd vcpkg
	./bootstrap-vcpkg.sh
	./vcpkg integrate install
	```

- pybind11
	```
	./vcpkg install pybind11
	```

- Vulkan
	- Download installer from [here](https://vulkan.lunarg.com/)
