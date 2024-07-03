# Find python and pybind11
set(PYBIND11_FINDPYTHON ON)
find_package(pybind11 CONFIG REQUIRED)

# Define module
pybind11_add_module(bindings "bindings_test.cpp")

# Link to internal dependency
target_link_libraries(bindings PRIVATE raytracer)

# Define install target
install(TARGETS bindings DESTINATION ${CMAKE_CURRENT_LIST_DIR}/../../kea)