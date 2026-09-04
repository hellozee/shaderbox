cmake_minimum_required(VERSION 3.16)

project(shaderbox VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SDL2 CONFIG QUIET)
if (NOT TARGET SDL2::SDL2)
    find_package(SDL2 REQUIRED)
endif()
find_package(OpenGL REQUIRED)

add_executable(
    shaderbox
    src/main.cc
    src/window/window.hh
    src/window/window.cc
    src/shader/program.hh
    src/shader/program.cc
)

target_compile_definitions(shaderbox PRIVATE GL_GLEXT_PROTOTYPES=1)

target_compile_options(shaderbox PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall;-Wextra;-Wpedantic>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
)

target_link_libraries(
    shaderbox
    SDL2::SDL2
    OpenGL::GL
)
if (WIN32 AND TARGET SDL2::SDL2main)
    target_link_libraries(shaderbox PRIVATE SDL2::SDL2main)
endif()

add_custom_command(TARGET shaderbox POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:shaderbox>/shaders"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/src/res"
            "$<TARGET_FILE_DIR:shaderbox>/shaders"
)

install(TARGETS shaderbox RUNTIME DESTINATION bin)
install(DIRECTORY src/res/ DESTINATION bin/shaders)
