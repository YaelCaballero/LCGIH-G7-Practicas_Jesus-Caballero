cmake_minimum_required(VERSION 3.10)

project(ConfigInicial)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Resolver advertencia de OpenGL
set(OpenGL_GL_PREFERENCE GLVND)

find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(assimp REQUIRED)

find_package(soil2 REQUIRED)

add_executable(ConfigInicial
    Carga_de_Modelos.cpp
)

# 3. Agregar SOIL2 al link_libraries
target_link_libraries(ConfigInicial
    glfw
    GLEW::GLEW
    OpenGL::GL
    assimp::assimp
    soil2
)
