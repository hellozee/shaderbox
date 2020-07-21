#pragma once

#include <string>
#include <GL/glew.h>

namespace shaderbox{
    class Shader{
        public:
        Shader(const std::string &shader_source, GLenum shader_type, GLuint program);
        GLuint get();

    private:
        void checkout_shader();
        GLuint m_shader;
    };


    class Program {
        public:
        Program(const std::string& vertex_source, const std::string& fragment_source);
        ~Program();

        GLuint get();

        private:
        const std::string load_from_file(const std::string& filename);
        void checkout_program(GLuint flag);

        GLuint m_program, m_vertex_shader, m_fragment_shader;
    };
}