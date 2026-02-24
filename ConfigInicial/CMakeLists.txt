cmake_minimum_required(VERSION 3.10)

project(ConfigInicial)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(GLEW REQUIRED)

add_executable(ConfigInicial
    Main_Modelado.cpp
)

target_link_libraries(ConfigInicial
    glfw
    GLEW::GLEW
    OpenGL::GL
)
