#include "OpenGLHDRSkybox.h"

#include "stb_image.h"
// #include <glad/glad.h>

/*
        all the complicated code ( equirectangular conversion,  shaders ... where found at this url. Great resource !
        https://learnopengl.com/code_viewer_gh.php?code=src/6.pbr/2.1.2.ibl_irradiance/ibl_irradiance.cpp
*/
namespace GLR {

HDRSkybox::HDRSkybox() : m_Intensity(1.0f) {}
HDRSkybox::~HDRSkybox() {
  LOG_INFO("deleting Skybox data in destructor");
  glDeleteTextures(1, &m_HDRTexture->GetID());
  glDeleteTextures(1, &m_IrradianceMap);
  glDeleteTextures(1, &prefilterMap);
  glDeleteTextures(1, &brdfLUTTexture);
  glDeleteTextures(1, &m_EnvCubeMap);
}
void HDRSkybox::Init() {
  if (m_IrradianceMap != 0) glDeleteTextures(1, &m_IrradianceMap);
  if (prefilterMap != 0) glDeleteTextures(1, &prefilterMap);
  if (brdfLUTTexture != 0) glDeleteTextures(1, &brdfLUTTexture);
  if (m_EnvCubeMap != 0) glDeleteTextures(1, &m_EnvCubeMap);

  glGenTextures(1, &m_ID);
  glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);

  glEnable(GL_DEPTH_TEST);
  // set depth function to less than AND equal for skybox depth trick.
  glDepthFunc(GL_LEQUAL);
  // enable seamless cubemap sampling for lower mip levels in the pre-filter map.
  // glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

  std::filesystem::path shaders_dir = "mesher_resources/Shaders/irradiance";

  equirectangularToCubemapShader =
      std::make_shared<Shader>(shaders_dir / "2.2.2.cubemap.vs", shaders_dir / "2.2.2.equirectangular_to_cubemap.fs");
  irradianceShader =
      std::make_shared<Shader>(shaders_dir / "2.2.2.cubemap.vs", shaders_dir / "2.2.2.irradiance_convolution.fs");
  prefilterShader = std::make_shared<Shader>(shaders_dir / "2.2.2.cubemap.vs", shaders_dir / "2.2.2.prefilter.fs");
  brdfShader = std::make_shared<Shader>(shaders_dir / "2.2.2.brdf.vs", shaders_dir / "2.2.2.brdf.fs");
  backgroundShader = std::make_shared<Shader>(shaders_dir / "2.2.2.background.vs", shaders_dir / "2.2.2.background.fs");

  backgroundShader->UseProgram();
  backgroundShader->SetInt("environmentMap", 0);

  m_HDRTexture = std::make_shared<HDRTexture>();
  if (m_HDRTexture->GetID()) glDeleteTextures(1, &m_HDRTexture->GetID());
  if (!m_EnvTexturePath.empty()) {
    m_HDRTexture->Load(m_EnvTexturePath);
    LOG_INFO("Loaded Data for HDR env texture map from {}", m_EnvTexturePath.string());
  } else {
    LOG_WARN("Setting Data for HDR env texture map as PURE RED !!!");
    m_HDRTexture->SetData({0.f, 1.f, 0.f}, 1, 1);
  }

  // pbr: setup framebuffer
  // ----------------------
  unsigned int captureFBO;
  unsigned int captureRBO;
  glGenFramebuffers(1, &captureFBO);
  glGenRenderbuffers(1, &captureRBO);

  glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
  glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

  m_HDRTexture->Bind(0);
  // pbr: setup cubemap to render to and attach to framebuffer
  // ---------------------------------------------------------

  glGenTextures(1, &m_EnvCubeMap);
  glBindTexture(GL_TEXTURE_CUBE_MAP, m_EnvCubeMap);
  for (unsigned int i = 0; i < 6; ++i) {
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
  }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // pbr: set up projection and view matrices for capturing data onto the 6 cubemap face directions
  // ----------------------------------------------------------------------------------------------
  glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
  glm::mat4 captureViews[] = {
      glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
      glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
      glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
      glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
      glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
      glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f))};

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

  // pbr: convert HDR equirectangular environment map to cubemap equivalent
  // ----------------------------------------------------------------------
  equirectangularToCubemapShader->UseProgram();
  equirectangularToCubemapShader->SetInt("equirectangularMap", 0);
  equirectangularToCubemapShader->SetMat4("projection", captureProjection);
  m_HDRTexture->Bind(0);

  // don't forget to configure the viewport to the capture dimensions.
  glViewport(0, 0, 512, 512);
  glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
  for (unsigned int i = 0; i < 6; ++i) {
    glUniformMatrix4fv(glGetUniformLocation(equirectangularToCubemapShader->GetID(), "view"), 1, GL_FALSE,
                       glm::value_ptr(captureViews[i]));

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_EnvCubeMap, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    RenderCube();
  }

  glBindTexture(GL_TEXTURE_CUBE_MAP, m_EnvCubeMap);
  glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  // pbr: create an irradiance cubemap, and re-scale capture FBO to irradiance scale.
  // --------------------------------------------------------------------------------

  glGenTextures(1, &m_IrradianceMap);
  glBindTexture(GL_TEXTURE_CUBE_MAP, m_IrradianceMap);
  for (unsigned int i = 0; i < 6; ++i) {
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0, GL_RGB, GL_FLOAT, nullptr);
  }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
  glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);

  // pbr: solve diffuse integral by convolution to create an irradiance (cube)map.
  // -----------------------------------------------------------------------------
  irradianceShader->UseProgram();
  irradianceShader->SetInt("environmentMap", 0);
  irradianceShader->SetMat4("projection", captureProjection);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, m_EnvCubeMap);

  // don't forget to configure the viewport to the capture dimensions.
  glViewport(0, 0, 32, 32);
  glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
  for (unsigned int i = 0; i < 6; ++i) {
    irradianceShader->SetMat4("view", captureViews[i]);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_IrradianceMap,
                           0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    RenderCube();
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  /*
          Specular IBL
  */

  // pbr: create a pre-filter cubemap, and re-scale capture FBO to pre-filter scale.
  // --------------------------------------------------------------------------------

  glGenTextures(1, &prefilterMap);
  glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
  for (unsigned int i = 0; i < 6; ++i) {
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 128, 128, 0, GL_RGB, GL_FLOAT, nullptr);
  }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);  // be sure to set minification filter to mip_linear
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // generate mipmaps for the cubemap so OpenGL automatically allocates the required memory.
  glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

  // pbr: run a quasi monte-carlo simulation on the environment lighting to create a prefilter (cube)map.
  // ----------------------------------------------------------------------------------------------------
  prefilterShader->UseProgram();
  prefilterShader->SetInt("environmentMap", 0);
  prefilterShader->SetMat4("projection", captureProjection);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, m_EnvCubeMap);

  glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
  unsigned int maxMipLevels = 5;
  for (unsigned int mip = 0; mip < maxMipLevels; ++mip) {
    // reisze framebuffer according to mip-level size.
    unsigned int mipWidth = static_cast<unsigned int>(128 * std::pow(0.5, mip));
    unsigned int mipHeight = static_cast<unsigned int>(128 * std::pow(0.5, mip));
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
    glViewport(0, 0, mipWidth, mipHeight);

    float roughness = (float)mip / (float)(maxMipLevels - 1);
    prefilterShader->SetFloat("roughness", roughness);
    for (unsigned int i = 0; i < 6; ++i) {
      prefilterShader->SetMat4("view", captureViews[i]);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilterMap,
                             mip);

      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      RenderCube();
    }
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  // pbr: generate a 2D LUT from the BRDF equations used.
  // ----------------------------------------------------

  glGenTextures(1, &brdfLUTTexture);

  // pre-allocate enough memory for the LUT texture.
  glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, 0);
  // be sure to set wrapping mode to GL_CLAMP_TO_EDGE
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // then re-configure capture framebuffer object and render screen-space quad with BRDF shader.
  glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
  glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfLUTTexture, 0);

  glViewport(0, 0, 512, 512);
  brdfShader->UseProgram();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  RenderQuad();

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void HDRSkybox::Bind() {
  // LOG_TRACE("HDRSkybox ID -> {}", m_ID);

  // glBindTextureUnit(0, m_ID);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);
}

void HDRSkybox::RenderQuad() {
  if (quadVAO == 0) {
    float quadVertices[] = {
        // positions        // texture Coords
        -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        1.0f,  1.0f, 0.0f, 1.0f, 1.0f, 1.0f,  -1.0f, 0.0f, 1.0f, 0.0f,
    };
    // setup plane VAO
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  }
  glBindVertexArray(quadVAO);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  glBindVertexArray(0);
}

void HDRSkybox::RenderCube() {
  // initialize (if necessary)
  if (cubeVAO == 0) {
    float vertices[] = {
        // back face
        -1.0f, -1.0f, -1.0f,  // bottom-left
        1.0f, 1.0f, -1.0f,    // top-right
        1.0f, -1.0f, -1.0f,   // bottom-right
        1.0f, 1.0f, -1.0f,    // top-right
        -1.0f, -1.0f, -1.0f,  // bottom-left
        -1.0f, 1.0f, -1.0f,   // top-left
        // front face
        -1.0f, -1.0f, 1.0f,  // bottom-left
        1.0f, -1.0f, 1.0f,   // bottom-right
        1.0f, 1.0f, 1.0f,    // top-right
        1.0f, 1.0f, 1.0f,    // top-right
        -1.0f, 1.0f, 1.0f,   // top-left
        -1.0f, -1.0f, 1.0f,  // bottom-left
        // left face
        -1.0f, 1.0f, 1.0f,    // top-right
        -1.0f, 1.0f, -1.0f,   // top-left
        -1.0f, -1.0f, -1.0f,  // bottom-left
        -1.0f, -1.0f, -1.0f,  // bottom-left
        -1.0f, -1.0f, 1.0f,   // bottom-right
        -1.0f, 1.0f, 1.0f,    // top-right
                              // right face
        1.0f, 1.0f, 1.0f,     // top-left
        1.0f, -1.0f, -1.0f,   // bottom-right
        1.0f, 1.0f, -1.0f,    // top-right
        1.0f, -1.0f, -1.0f,   // bottom-right
        1.0f, 1.0f, 1.0f,     // top-left
        1.0f, -1.0f, 1.0f,    // bottom-left
        // bottom face
        -1.0f, -1.0f, -1.0f,  // top-right
        1.0f, -1.0f, -1.0f,   // top-left
        1.0f, -1.0f, 1.0f,    // bottom-left
        1.0f, -1.0f, 1.0f,    // bottom-left
        -1.0f, -1.0f, 1.0f,   // bottom-right
        -1.0f, -1.0f, -1.0f,  // top-right
        // top face
        -1.0f, 1.0f, -1.0f,  // top-left
        1.0f, 1.0f, 1.0f,    // bottom-right
        1.0f, 1.0f, -1.0f,   // top-right
        1.0f, 1.0f, 1.0f,    // bottom-right
        -1.0f, 1.0f, -1.0f,  // top-left
        -1.0f, 1.0f, 1.0f    // bottom-left
    };
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    // fill buffer
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // link vertex attributes
    glBindVertexArray(cubeVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // glEnableVertexAttribArray(1);
    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    // glEnableVertexAttribArray(2);
    // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
  }
  // render Cube
  glBindVertexArray(cubeVAO);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);
}

void HDRSkybox::Draw(std::shared_ptr<Camera> camera, float intensity) {
  glm::mat4 view = glm::mat4(1.0f);
  glm::vec3 up_vector(0.f, 1.f, 0.f);
  view *= glm::lookAt(camera->m_Position, camera->target_position, glm::normalize(up_vector));
  // remove position from matrix
  view = glm::mat4(glm::mat3(view));

  backgroundShader->UseProgram();
  backgroundShader->SetInt("environmentMap", 0);
  backgroundShader->SetVec3("WorldPos", camera->m_Position);
  backgroundShader->SetMat4("view", view);
  backgroundShader->SetMat4("projection", camera->m_Projection);
  backgroundShader->SetFloat("envIntensity", m_Intensity);

  Bind();
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, m_EnvCubeMap);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);

  RenderCube();

  // unbind
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void HDRSkybox::Load(std::filesystem::path path) {
  m_EnvTexturePath = path;
  Init();
}
}  // namespace GLR