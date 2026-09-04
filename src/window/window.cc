#include "window.hh"

#include "../shader/program.hh"

#include <GL/glcorearb.h>

#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

namespace shaderbox {

Window::Window(unsigned width, unsigned height, std::string title)
    : m_width(width), m_height(height) {
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
    throw std::runtime_error{"SDL initialization failed: " + std::string{SDL_GetError()}};
  }
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0);
  m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED, static_cast<int>(width),
                              static_cast<int>(height),
                              SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
  if (!m_window) {
    const auto error = std::string{SDL_GetError()};
    SDL_Quit();
    throw std::runtime_error{"Window creation failed: " + error};
  }
  m_context = SDL_GL_CreateContext(m_window);
  if (!m_context) {
    const auto error = std::string{SDL_GetError()};
    SDL_DestroyWindow(m_window);
    SDL_Quit();
    throw std::runtime_error{"OpenGL context creation failed: " + error};
  }
  SDL_GL_SetSwapInterval(1);
  glDisable(GL_DEPTH_TEST);
  glClearColor(0.02F, 0.02F, 0.025F, 1.0F);
}

Window::~Window() {
  if (m_context) SDL_GL_DeleteContext(m_context);
  if (m_window) SDL_DestroyWindow(m_window);
  SDL_Quit();
}

int Window::exec(ShaderProgram &program) {
  GLuint vao = 0;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  const auto start = std::chrono::steady_clock::now();
  auto previous = start;
  int frame = 0;
  bool running = true;
  bool mouse_down = false;
  float mouse_x = 0.0F;
  float mouse_y = 0.0F;

  while (running) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT ||
          (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
        running = false;
      } else if (event.type == SDL_WINDOWEVENT &&
                 event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
        m_width = static_cast<unsigned>(event.window.data1);
        m_height = static_cast<unsigned>(event.window.data2);
      } else if (event.type == SDL_MOUSEMOTION) {
        mouse_x = static_cast<float>(event.motion.x);
        mouse_y = static_cast<float>(m_height - event.motion.y);
      } else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
        mouse_down = true;
      } else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT) {
        mouse_down = false;
      }
    }

    try {
      if (program.reload_if_changed()) std::cout << "Reloaded fragment shader\n";
    } catch (const std::exception &error) {
      std::cerr << error.what() << '\n';
    }
    const auto now = std::chrono::steady_clock::now();
    const float time = std::chrono::duration<float>(now - start).count();
    const float delta = std::chrono::duration<float>(now - previous).count();
    previous = now;
    glViewport(0, 0, static_cast<GLsizei>(m_width), static_cast<GLsizei>(m_height));
    glClear(GL_COLOR_BUFFER_BIT);
    program.set_uniforms(time, std::min(delta, 0.1F), frame++,
                         static_cast<int>(m_width), static_cast<int>(m_height),
                         mouse_x, mouse_y, mouse_down);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    SDL_GL_SwapWindow(m_window);
  }
  glDeleteVertexArrays(1, &vao);
  return EXIT_SUCCESS;
}

} // namespace shaderbox
