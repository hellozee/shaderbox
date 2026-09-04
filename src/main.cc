#include "shader/program.hh"
#include "window/window.hh"

#include <cstdlib>
#include <exception>
#include <filesystem>
#include <iostream>

int main(int argc, char **argv) {
  try {
    const auto shader_dir = std::filesystem::path{SHADERBOX_SHADER_DIR};
    const auto fragment = argc > 1 ? std::filesystem::path{argv[1]}
                                   : shader_dir / "fragment.glsl";
    auto app = shaderbox::Window{960, 540, "ShaderBox"};
    auto program = shaderbox::ShaderProgram{shader_dir / "vertex.glsl", fragment};
    return app.exec(program);
  } catch (const std::exception &error) {
    std::cerr << "shaderbox: " << error.what() << '\n';
    return EXIT_FAILURE;
  }
}
