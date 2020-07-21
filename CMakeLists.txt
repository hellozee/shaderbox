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
    src/shader/program.hh
    src/shader/program.cc
)

target_link_libraries(
    shaderbox
    SDL2::SDL2
    SDL2::SDL2main
    OpenGL::GL
    GLEW::GLEW
)

configure_file(src/res/vertex.glsl ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(src/res/fragment.glsl ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
