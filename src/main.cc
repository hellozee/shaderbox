#include "window/window.hh"

int main() {
    auto app = shaderbox::Window{800, 600, "ShaderBox"};
    return app.exec();
}