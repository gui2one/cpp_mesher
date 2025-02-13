#ifndef OPENGLHDRSKYBOX_H
#define OPENGLHDRSKYBOX_H
#pragma once

#include "Render/Camera.h"
#include "Render/OpenGLHDRTexture.h"
#include "Render/OpenGLShader.h"

namespace GLR {

class HDRSkybox {
 public:
  uint32_t m_ID;
  uint32_t m_Vao;
  uint32_t m_Vbo;

  float m_Intensity;
  std::shared_ptr<Shader> backgroundShader;

  std::shared_ptr<HDRTexture> m_HDRTexture;

  unsigned int m_IrradianceMap = 0;
  unsigned int prefilterMap = 0;
  unsigned int brdfLUTTexture = 0;

  std::shared_ptr<Shader> irradianceShader;
  std::shared_ptr<Shader> prefilterShader;
  std::shared_ptr<Shader> brdfShader;

 public:
  HDRSkybox();
  virtual ~HDRSkybox();

  void Init();
  void Bind();
  void Draw(std::shared_ptr<Camera> camera, float intensity = 1.0f);
  void Load(std::filesystem::path path);

 private:
  void RenderCube();
  void RenderQuad();
  unsigned int m_EnvCubeMap = 0;
  unsigned int cubeVAO = 0;
  unsigned int cubeVBO = 0;
  unsigned int quadVAO = 0;
  unsigned int quadVBO = 0;

  std::filesystem::path m_EnvTexturePath = "mesher_resources/Textures/hdr/HDR_110_Tunnel_Ref.hdr";
  std::shared_ptr<Shader> equirectangularToCubemapShader;
};
}  // namespace GLR
#endif /* OPENGLHDRSKYBOX_H */