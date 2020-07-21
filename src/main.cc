#include "window/window.hh"
#include "shader/program.hh"

int SDL_main(int, char**) {
    auto app = shaderbox::Window{800, 600, "ShaderBox"};
    auto program = shaderbox::Program{"vertex.glsl", "fragment.glsl"};
    return app.exec(program.get());
}