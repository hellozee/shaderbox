#pragma once

#include <string>

#include <SDL2/SDL.h>

namespace shaderbox
{
    class Window{
        public:
        Window(unsigned width, unsigned height, std::string title);
        ~Window();

        int exec();

        private:
        SDL_Window *m_window;
        SDL_GLContext m_context;
    };
} // namespace shaderbox
