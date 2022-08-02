cmake_minimum_required(VERSION 3.16)

# Set the project name and version
project(Kea VERSION 0.1)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the executable as target, add the two sources in this folder
add_executable(kea main.cpp)

# Vulkan
find_package(Vulkan REQUIRED)
target_include_directories(kea PRIVATE ${Vulkan_INCLUDE_DIRS})
target_link_libraries(kea PRIVATE ${Vulkan_LIBRARIES})

# GLFW
target_include_directories(kea PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../deps/glfw/include)
target_link_libraries(kea PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../deps/glfw/lib-vc2022/glfw3.lib)

# GLM
target_include_directories(kea PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../deps/glm)


# find_package(nlohmann_json 3.2.0 REQUIRED)
# target_link_libraries(season PRIVATE nlohmann_json::nlohmann_json)

# # StaticJSON does not create a proper library, so workaround here:
# target_sources(season PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../deps/StaticJSON/src/staticjson.cpp)
# target_include_directories(season PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../deps/StaticJSON/include)

# # Boost
# find_package(Boost 1.79 COMPONENTS filesystem thread REQUIRED)
# target_include_directories(season PRIVATE ${Boost_INCLUDE_DIR})
# target_link_libraries(season PRIVATE ${Boost_LIBRARIES})

# # Threading
# set(THREADS_PREFER_PTHREAD_FLAG ON)
# find_package(Threads REQUIRED)
# target_link_libraries(season PRIVATE Threads::Threads)

# # Jpeg
# find_package(JPEG REQUIRED)
# target_link_libraries(season PRIVATE ${JPEG_LIBRARIES})

# # Curl
# find_package(CURL REQUIRED) 
# target_include_directories(season PRIVATE ${CURL_INCLUDE_DIR})
# target_link_libraries(season PRIVATE ${CURL_LIBRARIES})

# # TBB
# find_package(TBB REQUIRED)
# target_link_libraries(season PRIVATE TBB::tbb)

# # OSG
# find_package(OpenSceneGraph COMPONENTS osg osgViewer osgDB REQUIRED)
# target_include_directories(season PRIVATE ${OPENSCENEGRAPH_INCLUDE_DIRS})
# target_link_libraries(season PRIVATE ${OPENSCENEGRAPH_LIBRARIES})

# # FFTS
# # No idea where this is actually installed, grabbing it from the deps folder
# target_include_directories(season PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../deps/ffts/include)
# target_link_libraries(season PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../deps/ffts/libffts.a)

# # GL
# find_package(OpenGL REQUIRED)
# target_include_directories(season PRIVATE ${OPENGL_INCLUDE_DIRS})
# target_link_libraries(season PRIVATE ${OPENGL_LIBRARIES})

# # Add flags
# # AVX on
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")

# # Limit max errors printed
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fmax-errors=32")
