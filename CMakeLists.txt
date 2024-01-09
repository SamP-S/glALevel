cmake_minimum_required(VERSION 3.8)

project(glALevel VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD          17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)
set(CMAKE_BUILD_TYPE            Debug)

find_package(Git REQUIRED)
if (NOT EXISTS ${LIBTFD_DIR}/CMakeLists.txt)
    execute_process(COMMAND ${GIT_EXECUTABLE}
        submodule update --recursive
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
endif()


find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIR})

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

find_package(glfw3 REQUIRED)
include_directories(${GLFW3_INCLUDE_DIR})

include_directories(include)
add_subdirectory("external/GLM")


add_executable (app src/Application.cpp)
target_link_libraries(app PUBLIC OpenGL::GL GLEW::GLEW glfw)
