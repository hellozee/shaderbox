#include "program.hh"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <utility>

namespace shaderbox {
namespace {

std::string info_log(GLuint object, bool program) {
  GLint length = 0;
  if (program) glGetProgramiv(object, GL_INFO_LOG_LENGTH, &length);
  else glGetShaderiv(object, GL_INFO_LOG_LENGTH, &length);
  std::string log(static_cast<size_t>(length), '\0');
  if (program) glGetProgramInfoLog(object, length, nullptr, log.data());
  else glGetShaderInfoLog(object, length, nullptr, log.data());
  return log;
}

} // namespace

ShaderProgram::ShaderProgram(std::filesystem::path vertex_path,
                             std::filesystem::path fragment_path)
    : m_vertex_path(std::move(vertex_path)),
      m_fragment_path(std::move(fragment_path)) {
  reload();
}

ShaderProgram::~ShaderProgram() {
  if (m_program != 0) glDeleteProgram(m_program);
}

std::string ShaderProgram::read_file(const std::filesystem::path &path) {
  std::ifstream file(path);
  if (!file) throw std::runtime_error{"Unable to open shader: " + path.string()};
  std::ostringstream source;
  source << file.rdbuf();
  return source.str();
}

GLuint ShaderProgram::compile(GLenum type, const std::string &source,
                              const std::filesystem::path &path) {
  const GLuint shader = glCreateShader(type);
  if (shader == 0) throw std::runtime_error{"Unable to create shader object"};
  const auto *text = source.c_str();
  const auto length = static_cast<GLint>(source.size());
  glShaderSource(shader, 1, &text, &length);
  glCompileShader(shader);
  GLint success = GL_FALSE;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (success == GL_FALSE) {
    const auto log = info_log(shader, false);
    glDeleteShader(shader);
    throw std::runtime_error{"Failed to compile " + path.string() + ":\n" + log};
  }
  return shader;
}

void ShaderProgram::check_program(GLuint program,
                                  const std::filesystem::path &path) {
  GLint success = GL_FALSE;
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (success == GL_FALSE) {
    throw std::runtime_error{"Failed to link shaders for " + path.string() +
                             ":\n" + info_log(program, true)};
  }
}

void ShaderProgram::reload() {
  const auto vertex = compile(GL_VERTEX_SHADER, read_file(m_vertex_path), m_vertex_path);
  const auto fragment = compile(GL_FRAGMENT_SHADER, read_file(m_fragment_path), m_fragment_path);
  const auto program = glCreateProgram();
  glAttachShader(program, vertex);
  glAttachShader(program, fragment);
  glLinkProgram(program);
  glDeleteShader(vertex);
  glDeleteShader(fragment);
  try { check_program(program, m_fragment_path); }
  catch (...) { glDeleteProgram(program); throw; }
  if (m_program != 0) glDeleteProgram(m_program);
  m_program = program;
  m_vertex_write_time = std::filesystem::last_write_time(m_vertex_path);
  m_fragment_write_time = std::filesystem::last_write_time(m_fragment_path);
}

bool ShaderProgram::reload_if_changed() {
  if (std::filesystem::last_write_time(m_vertex_path) == m_vertex_write_time &&
      std::filesystem::last_write_time(m_fragment_path) == m_fragment_write_time) {
    return false;
  }
  reload();
  return true;
}

void ShaderProgram::set_uniforms(float time, float delta_time, int frame,
                                 int width, int height, float mouse_x,
                                 float mouse_y, bool mouse_down) const {
  glUseProgram(m_program);
  const auto set_float = [this](const char *name, float value) {
    glUniform1f(glGetUniformLocation(m_program, name), value);
  };
  set_float("iTime", time);
  set_float("iTimeDelta", delta_time);
  glUniform1i(glGetUniformLocation(m_program, "iFrame"), frame);
  glUniform3f(glGetUniformLocation(m_program, "iResolution"),
              static_cast<float>(width), static_cast<float>(height), 1.0F);
  glUniform4f(glGetUniformLocation(m_program, "iMouse"), mouse_x, mouse_y,
              mouse_down ? mouse_x : 0.0F, mouse_down ? mouse_y : 0.0F);
}

} // namespace shaderbox
