#pragma once

#include <GL/glcorearb.h>

#include <filesystem>
#include <string>

namespace shaderbox {

class ShaderProgram {
public:
  ShaderProgram(std::filesystem::path vertex_path,
                std::filesystem::path fragment_path);
  ~ShaderProgram();

  ShaderProgram(const ShaderProgram &) = delete;
  ShaderProgram &operator=(const ShaderProgram &) = delete;

  [[nodiscard]] GLuint id() const noexcept { return m_program; }
  [[nodiscard]] bool reload_if_changed();
  void reload();
  void set_uniforms(float time, float delta_time, int frame, int width,
                    int height, float mouse_x, float mouse_y,
                    bool mouse_down) const;

private:
  static std::string read_file(const std::filesystem::path &path);
  static GLuint compile(GLenum type, const std::string &source,
                        const std::filesystem::path &path);
  static void check_program(GLuint program, const std::filesystem::path &path);

  std::filesystem::path m_vertex_path;
  std::filesystem::path m_fragment_path;
  std::filesystem::file_time_type m_vertex_write_time;
  std::filesystem::file_time_type m_fragment_write_time;
  GLuint m_program = 0;
};

} // namespace shaderbox
