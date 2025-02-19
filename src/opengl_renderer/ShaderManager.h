#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H
#pragma once

#include "Render/OpenGLShader.h"
#include "Render/OpenGLTexture.h"
namespace GLR {
class ShaderManager {
 public:
  std::shared_ptr<Shader> fontShader;
  std::shared_ptr<Shader> msdfFontShader;
  std::shared_ptr<Shader> ui_shader;

  std::shared_ptr<Shader> screen_shader;
  std::shared_ptr<Shader> line_shader;
  std::shared_ptr<Shader> unlit_shader;
  std::shared_ptr<Shader> depth_shader;
  std::shared_ptr<Shader> cube_depth_shader;
  std::shared_ptr<Shader> wire_shader;
  std::shared_ptr<Shader> image_display_shader;
  std::shared_ptr<Shader> outline_shader;

  std::shared_ptr<Shader> no_light_shader;
  std::shared_ptr<Shader> basic_shader;
  std::shared_ptr<Shader> pbr_shader;
  std::shared_ptr<Shader> star_shader;
  std::shared_ptr<Shader> cell_shader;

  std::shared_ptr<Shader> skybox_shader;

  // default textures. Mainly for MBR shader;
  std::shared_ptr<Texture> white_texture;

  std::shared_ptr<Texture> very_light_gray_texture;
  std::shared_ptr<Texture> light_gray_texture;

  std::shared_ptr<Texture> gray_texture;

  std::shared_ptr<Texture> very_dark_gray_texture;
  std::shared_ptr<Texture> dark_gray_texture;

  std::shared_ptr<Texture> black_texture;

  std::shared_ptr<Texture> red_texture;
  std::shared_ptr<Texture> flat_normal_texture;

 public:
  ~ShaderManager();

  static ShaderManager* GetInstance();

  void Init();

 private:
  static ShaderManager* s_Instance;

 protected:
  ShaderManager();
  ShaderManager(const ShaderManager& other) = delete;
};
} /* end namespace GLR*/

#endif