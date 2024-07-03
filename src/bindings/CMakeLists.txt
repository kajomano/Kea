set(PYBIND11_FINDPYTHON ON)
find_package(pybind11 CONFIG REQUIRED)

pybind11_add_module(bindings "bindings_test.cpp")

# target_sources(bindings PRIVATE "color_bindings.cpp")

target_link_libraries(bindings PRIVATE raytracer)
# target_link_libraries(bindings PRIVATE COMMON)
# target_link_libraries(bindings PRIVATE LOG)
# target_link_libraries(bindings PRIVATE RADIO)
# target_link_libraries(bindings PRIVATE MOVEMENT)

install(TARGETS bindings DESTINATION .)