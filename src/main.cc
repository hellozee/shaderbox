#include "shader/program.hh"
#include "window/window.hh"

#include <cstdlib>
#include <exception>
#include <filesystem>
#include <iostream>
#include <stdexcept>

int main(int argc, char **argv) {
  try {
    auto app = shaderbox::Window{960, 540, "ShaderBox"};
    const auto base_path = SDL_GetBasePath();
    if (base_path == nullptr) {
      throw std::runtime_error{"Unable to determine executable path: " + std::string{SDL_GetError()}};
    }
    auto executable_dir = std::filesystem::path{base_path};
    const auto shader_dir = executable_dir / "shaders";
    const auto fragment = argc > 1 ? std::filesystem::path{argv[1]}
                                   : shader_dir / "fragment.glsl";
    SDL_free(base_path);
    auto program = shaderbox::ShaderProgram{shader_dir / "vertex.glsl", fragment};
    return app.exec(program);
  } catch (const std::exception &error) {
    std::cerr << "shaderbox: " << error.what() << '\n';
    return EXIT_FAILURE;
  }
}
