cmake_minimum_required(VERSION 3.18.0)

# Set the project name and version.
project(UndoRedo VERSION 1.0)

# Specify the C++ standard.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++17")
endif()

include_directories(./include/)

file(GLOB PROJECT_HEADERS ./include/*.h)
file(GLOB PROJECT_SOURCES ./src/*.cpp
                          ./test/*.cpp)

file(GLOB PROJECT_CONFIGS ./CMakeLists.txt)

add_definitions(-DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS}
                               ${PROJECT_CONFIGS})

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

