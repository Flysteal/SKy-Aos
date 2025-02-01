cmake_minimum_required(VERSION 3.10)
project(SkyAos VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Find GLFW
find_package(glfw3 REQUIRED)

# Find OpenGL
find_package(OpenGL REQUIRED)

# Include directories
include_directories(include)
include_directories(hederfiles)


# file(GLOB SOURCES src/*.cpp)
# add_executable(${PROJECT_NAME} ${SOURCES})


# Find all .cpp files in src directory 
file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")

# Add the executable target
add_executable(SKY_LANDS ${SOURCES} src/glad.c)

# Link OpenGL and GLFW to the executable
target_link_libraries(SKY_LANDS ${OPENGL_LIBRARIES} glfw)

message(STATUS "include paths: ${CMAKE_INCLUDE_PATH}")