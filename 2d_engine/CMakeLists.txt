cmake_minimum_required(VERSION 3.18.0)

# Set the project name and version.
project(Application VERSION 1.0)

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(./lib/glfw-3.3.4)

# Specify the C++ standard.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++17")
    if(NOT WIN32)
        set(GLAD_LIBRARIES dl)
    endif()
endif()

include_directories(./src/include/
                    ./lib/glad/include/
                    ./lib/stb_image/
                    ./lib/glm/
                    ./lib/glfw-3.3.4/include/)

file(GLOB VENDORS_SOURCES ./lib/glad/src/glad.c)
file(GLOB PROJECT_HEADERS ./src/include/*.h)
file(GLOB PROJECT_SOURCES ./src/*.cpp)
file(GLOB PROJECT_SHADERS ./shaders/*.comp
                          ./shaders/*.frag
                          ./shaders/*.geom
                          ./shaders/*.vert)
file(GLOB PROJECT_CONFIGS ./CMakeLists.txt)

add_definitions(-DGLFW_INCLUDE_NONE
                -DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS}
                               ${PROJECT_SHADERS} ${PROJECT_CONFIGS}
                               ${VENDORS_SOURCES})

target_link_libraries(${PROJECT_NAME} glfw
                      ${GLFW_LIBRARIES} ${GLAD_LIBRARIES})

# For generating compule_commands.json (used for autocompletion)
# Another means of achieving the same effect would be to run:
# cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

