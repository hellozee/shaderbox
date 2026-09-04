#pragma once

#include <SDL2/SDL.h>

#include <string>

namespace shaderbox {
class ShaderProgram;

class Window {
public:
  Window(unsigned width, unsigned height, std::string title);
  ~Window();

  Window(const Window &) = delete;
  Window &operator=(const Window &) = delete;

  int exec(ShaderProgram &program);

private:
  SDL_Window *m_window = nullptr;
  SDL_GLContext m_context = nullptr;
  unsigned m_width;
  unsigned m_height;
};
} // namespace shaderbox
