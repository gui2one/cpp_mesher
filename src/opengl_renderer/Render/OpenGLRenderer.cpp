#include "OpenGLRenderer.h"

namespace GLR {

/* ObjectID stuff utilities */
static glm::vec3 entity_id_to_color(uint32_t id) {
  unsigned char red = (id >> 0) & 0xFF;
  unsigned char green = (id >> 8) & 0xFF;
  unsigned char blue = (id >> 16) & 0xFF;

  return glm::vec3((float)red / 255.0f, (float)green / 255.0f, (float)blue / 255.0f);
}

static uint32_t color_to_entity_id(uint8_t* clr) {
  uint32_t a = uint32_t((uint8_t)(clr[0]) << 0 | (uint8_t)(clr[1]) << 8 | (uint8_t)(clr[2]) << 16 | (uint8_t)(0));
  return a;
}

OpenGLRenderer::OpenGLRenderer() {}

OpenGLRenderer::~OpenGLRenderer() {}

void OpenGLRenderer::Init() {
  m_ShaderManager = ShaderManager::GetInstance();
  InitTextures();

  m_DefaultMaterial = std::make_shared<BasicMaterial>();
  m_DefaultMaterial->Init();

  auto mat = std::dynamic_pointer_cast<BasicMaterial>(m_DefaultMaterial);
  if (mat != nullptr) {
    mat->m_DiffuseColor = glm::vec3(1.0f, 0.f, 1.f);
    mat->m_DiffuseTexture = uv_grid_texture;
    // auto normal_map = std::make_shared<Texture>();
    // normal_map->Load("mesher_resources/Textures/flat_normal.png");
    // mat->m_NormalTexture = normal_map;
  }

  glGenBuffers(1, &m_DirectionalLightsUbo);
  glGenBuffers(1, &m_SpotLightsUbo);
  glGenBuffers(1, &m_PointLightsUbo);
}

void OpenGLRenderer::InitTextures() {
  /* Load Default Textures */
  white_texture = std::make_shared<Texture>();
  white_texture->Load("mesher_resources/Textures/white.png");

  uv_grid_texture = std::make_shared<Texture>();
  uv_grid_texture->Load("mesher_resources/Textures/uv_grid.jpg");
}

void OpenGLRenderer::CollectLightsUniforms(const Scene& scene) {
  std::vector<DirectionalLightUniform> dir_light_uniforms;
  std::vector<SpotLightUniform> spot_light_uniforms;
  std::vector<PointLightUniform> point_light_uniforms;

  m_DirectionalLightsShadowMapUniformNames.clear();
  m_SpotLightsShadowMapUniformNames.clear();
  m_PointLightsShadowMapUniformNames.clear();

  m_NumDirectionalLights = 0;
  m_NumSpotLights = 0;
  m_NumPointLights = 0;

  for (const auto& light : scene.GetLights()) {
    auto dir_light_ptr = std::dynamic_pointer_cast<DirectionalLight>(light);
    auto spot_light_ptr = std::dynamic_pointer_cast<SpotLight>(light);
    auto point_light_ptr = std::dynamic_pointer_cast<PointLight>(light);

    if (dir_light_ptr != nullptr) {
      DirectionalLightUniform uniform;
      uniform.intensity = dir_light_ptr->m_Intensity;
      uniform.shadowBias = dir_light_ptr->m_ShadowBias;
      uniform.position = dir_light_ptr->m_Position;
      uniform.direction = dir_light_ptr->GetDirection();
      uniform.color = dir_light_ptr->m_Color;
      uniform.lightSpaceMatrix = dir_light_ptr->GetLightSpaceMatrix();

      dir_light_uniforms.push_back(uniform);

      std::string shadow_uniform_name = "u_directionalShadowMaps[" + std::to_string(m_NumDirectionalLights) + "]";
      m_DirectionalLightsShadowMapUniformNames.push_back(shadow_uniform_name);

      m_NumDirectionalLights++;

    } else if (spot_light_ptr != nullptr) {
      SpotLightUniform uniform;
      uniform.intensity = spot_light_ptr->m_Intensity;
      uniform.angle = spot_light_ptr->GetAngle();
      uniform.shadowBias = spot_light_ptr->m_ShadowBias;
      uniform.near_plane = spot_light_ptr->GetZNear();
      uniform.far_plane = spot_light_ptr->GetZFar();
      uniform.position = spot_light_ptr->m_Position;
      uniform.direction = spot_light_ptr->GetDirection();
      uniform.color = spot_light_ptr->m_Color;
      uniform.lightSpaceMatrix = spot_light_ptr->GetLightSpaceMatrix();

      spot_light_uniforms.push_back(uniform);

      std::string shadow_uniform_name = "u_spotShadowMaps[" + std::to_string(m_NumSpotLights) + "]";
      m_SpotLightsShadowMapUniformNames.push_back(shadow_uniform_name);

      m_NumSpotLights++;

    } else if (point_light_ptr != nullptr) {
      PointLightUniform uniform;
      uniform.intensity = point_light_ptr->m_Intensity;
      uniform.shadowBias = point_light_ptr->m_ShadowBias;
      uniform.near_plane = point_light_ptr->GetZNear();
      uniform.far_plane = point_light_ptr->GetZFar();
      uniform.position = point_light_ptr->m_Position;

      uniform.color = point_light_ptr->m_Color;

      point_light_uniforms.push_back(uniform);

      std::string shadow_uniform_name = "u_pointShadowMaps[" + std::to_string(m_NumPointLights) + "]";
      m_PointLightsShadowMapUniformNames.push_back(shadow_uniform_name);

      m_NumPointLights++;
    }
  }

  if (m_NumDirectionalLights > 0) {
    glBindBuffer(GL_UNIFORM_BUFFER, m_DirectionalLightsUbo);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(DirectionalLightUniform) * dir_light_uniforms.size(),
                 dir_light_uniforms.data(), GL_DYNAMIC_DRAW);

    // Bind the UBO to the appropriate binding point (0 in this example)
    m_DirectionalLightsBindingPoint = 0;
    glBindBufferBase(GL_UNIFORM_BUFFER, m_DirectionalLightsBindingPoint, m_DirectionalLightsUbo);
    // Unbind the UBO
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
  }

  if (m_NumSpotLights > 0) {
    glBindBuffer(GL_UNIFORM_BUFFER, m_SpotLightsUbo);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(SpotLightUniform) * spot_light_uniforms.size(), spot_light_uniforms.data(),
                 GL_DYNAMIC_DRAW);

    // Bind the UBO to the appropriate binding point (0 in this example)
    m_SpotLightsBindingPoint = 1;
    glBindBufferBase(GL_UNIFORM_BUFFER, m_SpotLightsBindingPoint, m_SpotLightsUbo);

    // Unbind the UBO
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
  }

  if (m_NumPointLights > 0) {
    glBindBuffer(GL_UNIFORM_BUFFER, m_PointLightsUbo);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(PointLightUniform) * point_light_uniforms.size(),
                 point_light_uniforms.data(), GL_DYNAMIC_DRAW);

    // Bind the UBO to the appropriate binding point (0 in this example)
    m_PointLightsBindingPoint = 2;
    glBindBufferBase(GL_UNIFORM_BUFFER, m_PointLightsBindingPoint, m_PointLightsUbo);

    // Unbind the UBO
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
  }
}

void OpenGLRenderer::BindShadowMaps(const Scene& scene, std::shared_ptr<Shader> shader) {
  uint32_t offset =
      10; /* offset to give enough space for actual material texture channels ( i.e roughness, normal_map ... ) */

  uint32_t dir_lights_inc = 0;
  uint32_t spot_lights_inc = 0;
  uint32_t point_lights_inc = 0;

  for (auto& light : scene.GetLights()) {
    auto dir_light_ptr = std::dynamic_pointer_cast<DirectionalLight>(light);
    auto spot_light_ptr = std::dynamic_pointer_cast<SpotLight>(light);
    auto point_light_ptr = std::dynamic_pointer_cast<PointLight>(light);

    if (dir_light_ptr != nullptr) {
      dir_light_ptr->GetShadowMap()->Bind(dir_lights_inc + offset);
      shader->SetInt(m_DirectionalLightsShadowMapUniformNames[dir_lights_inc].c_str(), dir_lights_inc + offset);
      dir_lights_inc++;

    } else if (spot_light_ptr != nullptr) {
      spot_light_ptr->GetShadowMap()->Bind(spot_lights_inc + offset + 8);
      shader->SetInt(m_SpotLightsShadowMapUniformNames[spot_lights_inc].c_str(), spot_lights_inc + offset + 8);
      spot_lights_inc++;

    } else if (point_light_ptr != nullptr) {
      point_light_ptr->GetShadowMap()->Bind(point_lights_inc + offset + (8 + 8));
      shader->SetInt(m_PointLightsShadowMapUniformNames[point_lights_inc].c_str(), point_lights_inc + offset + (8 + 8));
      point_lights_inc++;
    }
  }
}

void OpenGLRenderer::RenderShadowPass(std::shared_ptr<Layer> layer, const Scene& scene,
                                      std::shared_ptr<Camera> camera) {
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);
  for (auto& light : scene.GetLights()) {
    auto dir_ptr = std::dynamic_pointer_cast<DirectionalLight>(light);
    auto spot_ptr = std::dynamic_pointer_cast<SpotLight>(light);
    auto point_ptr = std::dynamic_pointer_cast<PointLight>(light);

    if (dir_ptr) {
      glm::mat4 lightSpaceMatrix = dir_ptr->GetLightSpaceMatrix();
      glm::vec3 lightDir = dir_ptr->GetDirection();

      auto& depth_shader = m_ShaderManager->depth_shader;

      dir_ptr->GetShadowMap()->BindFBO();

      glEnable(GL_DEPTH_TEST);
      // glEnable(GL_CULL_FACE);
      // glCullFace(GL_BACK);
      glClearDepth(5.0f);
      glClear(GL_DEPTH_BUFFER_BIT);

      depth_shader->UseProgram();
      depth_shader->SetMat4("u_lightSpaceMatrix", lightSpaceMatrix);
      for (const auto& object : scene.GetObjects()) {
        white_texture->Bind(0);
        auto tr = object->ApplyParentTransforms();
        depth_shader->SetMat4("u_model", tr);
        depth_shader->SetInt("opacity_texture", 0);
        object->Render();
      }
      for (const auto& object : scene.GetParticleSystems()) {
        white_texture->Bind(0);
        auto tr = object->ApplyParentTransforms();
        depth_shader->SetMat4("u_model", tr);
        depth_shader->SetInt("opacity_texture", 0);
        object->Render();
      }

      glUseProgram(0);
      glBindTexture(GL_TEXTURE_2D, 0);

      glBindFramebuffer(GL_FRAMEBUFFER, 0);

    } else if (spot_ptr) {
      glm::mat4 lightSpaceMatrix = spot_ptr->GetLightSpaceMatrix();
      glm::vec3 lightDir = spot_ptr->GetDirection();

      auto& depth_shader = m_ShaderManager->depth_shader;

      spot_ptr->GetShadowMap()->BindFBO();

      glEnable(GL_DEPTH_TEST);
      // glEnable(GL_CULL_FACE);
      // glCullFace(GL_BACK);
      glClearDepth(5.0f);
      glClear(GL_DEPTH_BUFFER_BIT);

      depth_shader->UseProgram();
      depth_shader->SetMat4("u_lightSpaceMatrix", lightSpaceMatrix);
      for (const auto& object : scene.GetObjects()) {
        white_texture->Bind(0);
        auto tr = object->ApplyParentTransforms();
        depth_shader->SetMat4("u_model", tr);
        depth_shader->SetInt("opacity_texture", 0);
        object->Render();
      }

      for (const auto& object : scene.GetParticleSystems()) {
        white_texture->Bind(0);
        auto tr = object->ApplyParentTransforms();
        depth_shader->SetMat4("u_model", tr);
        depth_shader->SetInt("opacity_texture", 0);
        object->Render();
      }

      glUseProgram(0);
      glBindTexture(GL_TEXTURE_2D, 0);

      glCullFace(GL_FRONT);
      glBindFramebuffer(GL_FRAMEBUFFER, 0);

    } else if (point_ptr) {
      float aspect = 1.0f;
      float znear = 1.0f;
      float zfar = 25.0f;
      glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, znear, zfar);
      std::vector<glm::mat4> shadowTransforms;
      shadowTransforms.push_back(shadowProj * glm::lookAt(point_ptr->GetPosition(),
                                                          point_ptr->GetPosition() + glm::vec3(1.0, 0.0, 0.0),
                                                          glm::vec3(0.0, -1.0, 0.0)));
      shadowTransforms.push_back(shadowProj * glm::lookAt(point_ptr->GetPosition(),
                                                          point_ptr->GetPosition() + glm::vec3(-1.0, 0.0, 0.0),
                                                          glm::vec3(0.0, -1.0, 0.0)));
      shadowTransforms.push_back(shadowProj * glm::lookAt(point_ptr->GetPosition(),
                                                          point_ptr->GetPosition() + glm::vec3(0.0, 1.0, 0.0),
                                                          glm::vec3(0.0, 0.0, 1.0)));
      shadowTransforms.push_back(shadowProj * glm::lookAt(point_ptr->GetPosition(),
                                                          point_ptr->GetPosition() + glm::vec3(0.0, -1.0, 0.0),
                                                          glm::vec3(0.0, 0.0, -1.0)));
      shadowTransforms.push_back(shadowProj * glm::lookAt(point_ptr->GetPosition(),
                                                          point_ptr->GetPosition() + glm::vec3(0.0, 0.0, 1.0),
                                                          glm::vec3(0.0, -1.0, 0.0)));
      shadowTransforms.push_back(shadowProj * glm::lookAt(point_ptr->GetPosition(),
                                                          point_ptr->GetPosition() + glm::vec3(0.0, 0.0, -1.0),
                                                          glm::vec3(0.0, -1.0, 0.0)));

      auto& cube_depth_shader = m_ShaderManager->cube_depth_shader;

      point_ptr->GetShadowMap()->BindFBO();

      glEnable(GL_DEPTH_TEST);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glEnable(GL_BLEND);
      glClearDepth(5.0f);
      glClear(GL_DEPTH_BUFFER_BIT);

      for (const auto& object : scene.GetObjects()) {
        white_texture->Bind(0);
        cube_depth_shader->UseProgram();

        /*
            for sources see : $project_dir/mesher_resources/Shaders/cube_depth_shader.geo
        */
        cube_depth_shader->SetMat4("shadowMatrices[0]", shadowTransforms[0]);
        cube_depth_shader->SetMat4("shadowMatrices[1]", shadowTransforms[1]);
        cube_depth_shader->SetMat4("shadowMatrices[2]", shadowTransforms[2]);
        cube_depth_shader->SetMat4("shadowMatrices[3]", shadowTransforms[3]);
        cube_depth_shader->SetMat4("shadowMatrices[4]", shadowTransforms[4]);
        cube_depth_shader->SetMat4("shadowMatrices[5]", shadowTransforms[5]);

        cube_depth_shader->SetVec3("u_lightPos", point_ptr->GetPosition());
        cube_depth_shader->SetFloat("u_farPlane", point_ptr->GetZFar());

        auto tr = object->ApplyParentTransforms();
        cube_depth_shader->SetMat4("u_model", tr);

        cube_depth_shader->SetInt("opacity_texture", 0);
        object->Render();
      }

      for (const auto& object : scene.GetParticleSystems()) {
        white_texture->Bind(0);
        cube_depth_shader->UseProgram();

        /*
            for sources see : $project_dir/mesher_resources/Shaders/cube_depth_shader.geo
        */
        cube_depth_shader->SetMat4("shadowMatrices[0]", shadowTransforms[0]);
        cube_depth_shader->SetMat4("shadowMatrices[1]", shadowTransforms[1]);
        cube_depth_shader->SetMat4("shadowMatrices[2]", shadowTransforms[2]);
        cube_depth_shader->SetMat4("shadowMatrices[3]", shadowTransforms[3]);
        cube_depth_shader->SetMat4("shadowMatrices[4]", shadowTransforms[4]);
        cube_depth_shader->SetMat4("shadowMatrices[5]", shadowTransforms[5]);

        cube_depth_shader->SetVec3("u_lightPos", point_ptr->GetPosition());
        cube_depth_shader->SetFloat("u_farPlane", point_ptr->GetZFar());

        auto tr = object->ApplyParentTransforms();
        cube_depth_shader->SetMat4("u_model", tr);

        cube_depth_shader->SetInt("opacity_texture", 0);
        object->Render();
      }

      glUseProgram(0);
      glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

      glCullFace(GL_FRONT);
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
  }
}

void OpenGLRenderer::RenderObjects(std::shared_ptr<Layer> layer, const Scene& scene, std::shared_ptr<Camera> camera) {
  glm::mat4 view_matrix = glm::lookAt(camera->GetPosition(), camera->target_position, glm::vec3(0.0f, 1.0f, 0.0f));

  auto& basic_shader = m_ShaderManager->basic_shader;
  // auto& msdf_font_shader = m_ShaderManager->msdfFontShader;

  layer->Bind();

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  // glClearDepth(5.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if (scene.GetDisplaySkybox()) {
    if (scene.GetSkybox()) {
      scene.GetSkybox()->Draw(camera);
    }

    if (scene.GetHDRSkybox() != nullptr) {
      // glDepthFunc(GL_EQUAL);
      scene.GetHDRSkybox()->Draw(camera, 1.0f);
    }
  }

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  glEnable(GL_FRAMEBUFFER_SRGB_EXT);

  std::shared_ptr<Material> cur_material;

  for (const auto& object : scene.GetObjects()) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    if (object->m_Material != nullptr) {
      cur_material = object->m_Material;
    } else {
      cur_material = m_DefaultMaterial;
    }

    cur_material->GetShader()->UseProgram();

    /* bind lights uniform buffers */
    if (m_NumDirectionalLights > 0) {
      glBindBufferBase(GL_UNIFORM_BUFFER, m_DirectionalLightsBindingPoint, m_DirectionalLightsUbo);
    }
    if (m_NumSpotLights > 0) {
      glBindBufferBase(GL_UNIFORM_BUFFER, m_SpotLightsBindingPoint, m_SpotLightsUbo);
    }

    BindShadowMaps(scene, cur_material->GetShader());

    cur_material->GetShader()->SetInt("u_numDirectionalLights", m_NumDirectionalLights);
    cur_material->GetShader()->SetInt("u_numSpotLights", m_NumSpotLights);
    cur_material->GetShader()->SetInt("u_numPointLights", m_NumPointLights);

    auto tr = object->ApplyParentTransforms();
    cur_material->GetShader()->SetMat4("u_model", tr);
    cur_material->GetShader()->SetMat4("u_projection", camera->m_Projection);
    cur_material->GetShader()->SetMat4("u_view", view_matrix);

    cur_material->GetShader()->SetVec3("u_viewPos", camera->GetPosition());

    auto clr = entity_id_to_color(object->m_ID);
    cur_material->GetShader()->SetVec3("u_ColorID", clr);

    if (auto ptr = std::dynamic_pointer_cast<PBRMaterial>(cur_material)) {
      if (scene.GetHDRSkybox()) {
        ptr->GetShader()->SetFloat("envIntensity", scene.GetHDRSkybox()->m_Intensity);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, scene.GetHDRSkybox()->m_IrradianceMap);
        ptr->GetShader()->SetInt("irradianceMap", 0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_CUBE_MAP, scene.GetHDRSkybox()->prefilterMap);
        ptr->GetShader()->SetInt("prefilterMap", 1);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, scene.GetHDRSkybox()->brdfLUTTexture);
        ptr->GetShader()->SetInt("brdfLUT", 2);
      }

      ptr->GetShader()->SetVec3("WorldPos", camera->m_Position);

      // LOG_INFO("\nsettings PBR IBL stuff.....");
    }

    cur_material->SetUniforms();

    object->Render();
  }

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

  for (const auto& psystem : scene.GetParticleSystems()) {
    auto shader = ShaderManager::GetInstance()->basic_shader;
    shader->UseProgram();
    ShaderManager::GetInstance()->white_texture->Bind(0);
    BindShadowMaps(scene, shader);
    shader->SetInt("u_numDirectionalLights", m_NumDirectionalLights);
    shader->SetInt("u_numSpotLights", m_NumSpotLights);
    shader->SetInt("u_numPointLights", m_NumPointLights);
    // LOG_INFO("Particles ?");
    auto tr = psystem->ApplyParentTransforms();
    shader->SetMat4("u_model", tr);
    shader->SetMat4("u_projection", camera->m_Projection);
    shader->SetMat4("u_view", view_matrix);

    shader->SetVec3("u_viewPos", camera->GetPosition());

    psystem->Render();
  }

  auto& wire_shader = m_ShaderManager->wire_shader;

  for (auto& box : scene.GetBoxLineObjects()) {
    wire_shader->UseProgram();

    auto tr = box->ApplyParentTransforms();
    auto clr = glm::vec3(1.0f, 1.0f, 0.0f);
    wire_shader->SetVec3("u_color", clr);
    wire_shader->SetMat4("u_model", tr);
    wire_shader->SetMat4("u_projection", camera->m_Projection);
    wire_shader->SetMat4("u_view", view_matrix);

    box->Render();
  }

  for (auto& object : scene.GetDebugDraws()) {
    wire_shader->UseProgram();

    auto tr = object->ApplyParentTransforms();
    auto clr = glm::vec3(0.0f, 1.0f, 1.0f);
    wire_shader->SetVec3("u_color", clr);
    wire_shader->SetMat4("u_model", tr);
    wire_shader->SetMat4("u_projection", camera->m_Projection);
    wire_shader->SetMat4("u_view", view_matrix);

    object->Render();
  }

  /*
      draw seleted object outlines
  */

  glCullFace(GL_FRONT);
  for (const auto& object : scene.GetObjects()) {
    if (!object->m_Selected) continue;

    auto outline_shader = ShaderManager::GetInstance()->outline_shader;

    outline_shader->UseProgram();
    auto tr = object->ApplyParentTransforms();
    outline_shader->SetMat4("u_projection", camera->m_Projection);
    outline_shader->SetVec3("u_viewPos", camera->GetPosition());
    outline_shader->SetMat4("u_view", view_matrix);
    outline_shader->SetMat4("u_model", tr);

    glm::vec3 clr = glm::vec3(0.0f, 0.5f, 1.0f);
    outline_shader->SetVec3("u_color", clr);
    object->Render();
  }

  glCullFace(GL_BACK);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, 0);

  layer->Unbind();

  glDisable(GL_FRAMEBUFFER_SRGB_EXT);
}

void OpenGLRenderer::Render(std::shared_ptr<Layer> layer, const Scene& scene, std::shared_ptr<Camera> camera) {
  glViewport(0, 0, layer->GetFrameBuffer().GetWidth(), layer->GetFrameBuffer().GetHeight());

  CollectLightsUniforms(scene);
  // RenderShadowPass(layer, scene, camera);

  RenderObjects(layer, scene, camera);
}
}  // namespace GLR