cmake_minimum_required(VERSION 3.0)

project(shaderbox)

find_package(sdl2 REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(
    shaderbox
    src/main.cc
    src/window/window.hh
    src/window/window.cc
)

target_link_libraries(
    shaderbox
    SDL2::SDL2
    OpenGL::GL
    GLEW::GLEW
)
