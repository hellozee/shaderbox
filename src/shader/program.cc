#include "program.hh"

#include <fstream>
#include <sstream>

using namespace shaderbox;

Shader::Shader(const std::string& source, GLenum type, GLuint program) {
	m_shader = glCreateShader(type);

	if (!m_shader) {
		throw std::exception{"Can't create a shader"};
	}

	auto shader_source = static_cast<const GLchar*>(source.c_str());
	auto shader_length = static_cast<GLint>(source.length());

	glShaderSource(m_shader, 1, &shader_source, &shader_length);
	glCompileShader(m_shader);

	glAttachShader(program, m_shader);
}

GLuint Shader::get() {
	return m_shader;
}

void Shader::checkout_shader() {
	auto success = GLint{0};
	auto error = new GLchar[1024];
	
	glGetShaderiv(m_shader, GL_COMPILE_STATUS, &success);

	if (success == GL_FALSE) {
		glGetShaderInfoLog(m_shader, sizeof(error), nullptr, error);
		throw std::exception{error};
	}

	delete error;
}

Program::Program(const std::string& vertex_source, const std::string& fragment_source) {
	m_program = glCreateProgram();

	m_vertex_shader = Shader{load_from_file(vertex_source), GL_VERTEX_SHADER, m_program}.get();
	m_fragment_shader = Shader{load_from_file(fragment_source), GL_FRAGMENT_SHADER, m_program}.get();

	glLinkProgram(m_program);
	checkout_program(GL_LINK_STATUS);

	glValidateProgram(m_program);
	checkout_program(GL_VALIDATE_STATUS);
}

Program::~Program() {
	glDetachShader(m_program, m_vertex_shader);
	glDetachShader(m_program, m_fragment_shader);
	glDeleteShader(m_vertex_shader);
	glDeleteShader(m_fragment_shader);
	glDeleteProgram(m_program);
}

GLuint Program::get() {
	return m_program;
}

const std::string Program::load_from_file(const std::string& source) {
	auto file = std::ifstream{source};
	auto buffer = std::stringstream{};
	buffer << file.rdbuf();
	return buffer.str();
}

void Program::checkout_program(GLuint flag) {
	auto success = GLint{0};
	auto error = new GLchar[1024];
	
	glGetProgramiv(m_program, flag, &success);

	if (success == GL_FALSE) {
		glGetShaderInfoLog(m_program, sizeof(error), nullptr, error);
		throw std::exception{error};
	}

	delete error;
}