cmake_minimum_required(VERSION 3.8)

project(glALevel VERSION 1.0.0 LANGUAGES C CXX)

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


add_executable (gl_v0.1 src/OpenGL_0.1.cpp)
target_link_libraries(gl_v0.1 PUBLIC glfw)

add_executable (gl_v0.2 src/OpenGL_0.2.cpp)
target_link_libraries(gl_v0.2 PUBLIC OpenGL::GL GLEW::GLEW glfw)

add_executable (gl_v0.3 src/OpenGL_0.3.cpp)
target_link_libraries(gl_v0.3 PUBLIC OpenGL::GL GLEW::GLEW glfw)

add_executable (gl_v0.4 src/OpenGL_0.4.cpp)
target_link_libraries(gl_v0.4 PUBLIC OpenGL::GL GLEW::GLEW glfw)

add_executable (gl_v0.5 src/OpenGL_0.5.cpp)
target_link_libraries(gl_v0.5 PUBLIC OpenGL::GL GLEW::GLEW glfw)

add_executable (gl_v0.6 src/OpenGL_0.5.cpp)
target_link_libraries(gl_v0.6 PUBLIC OpenGL::GL GLEW::GLEW glfw glm::glm)


