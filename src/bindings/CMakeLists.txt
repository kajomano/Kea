# Find python and pybind11
set(PYBIND11_FINDPYTHON ON)
find_package(pybind11 CONFIG REQUIRED)

# Define module
pybind11_add_module(Bindings "bindings_test.cpp")

# Link to internal dependency
target_link_libraries(Bindings PRIVATE Raytracer)

# Define install target
install(TARGETS Bindings DESTINATION ${CMAKE_CURRENT_LIST_DIR}/../../kea)