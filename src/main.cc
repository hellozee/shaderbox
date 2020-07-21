#include "shader/program.hh"
#include "window/window.hh"

#if defined(_MSC_VER)
int SDL_main(int, char **)
#else
int main(int, char **)
#endif
{
  auto app = shaderbox::Window{800, 600, "ShaderBox"};
  auto program = shaderbox::Program{"vertex.glsl", "fragment.glsl"};
  return app.exec(program.get());
}