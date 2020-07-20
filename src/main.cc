#include "window/window.hh"

int SDL_main(int, char**) {
    auto app = shaderbox::Window{800, 600, "ShaderBox"};
    return app.exec();
}