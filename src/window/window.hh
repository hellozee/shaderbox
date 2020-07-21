#pragma once

#include <SDL2/SDL.h>
#include <string>

namespace shaderbox {
class Window {
public:
  Window(unsigned width, unsigned height, std::string title);
  ~Window();

  int exec(unsigned int program);

private:
  SDL_Window *m_window;
  SDL_GLContext m_context;
};
} // namespace shaderbox
