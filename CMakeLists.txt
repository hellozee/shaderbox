cmake_minimum_required(VERSION 3.0)

project(shaderbox)

find_package(sdl2 REQUIRED)

add_executable(
    shaderbox
    src/main.cc
    )

target_link_libraries(
    shaderbox
    SDL2::SDL2
    )
