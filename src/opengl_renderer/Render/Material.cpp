#include "Material.h"

#include "ShaderManager.h"
namespace GLR {

void BasicMaterial::Init() {
  m_Shader = std::make_shared<Shader>("mesher_resources/Shaders/basic_shader.vert",
                                      "mesher_resources/Shaders/basic_shader.frag");
}
void BasicMaterial::SetUniforms() {
  m_Shader->SetVec3("material.diffuseColor", m_DiffuseColor);
  if (m_DiffuseTexture != nullptr) {
    // LOG_INFO("DIFFUSE COLOR !!!");
    m_DiffuseTexture->Bind(0);
    m_Shader->SetInt("material.diffuseTexture", 0);
  } else {
    ShaderManager::GetInstance()->white_texture->Bind(0);
    m_Shader->SetInt("material.diffuseTexture", 0);
  }
  if (m_NormalTexture != nullptr) {
    // LOG_INFO("NORMAL TEXTURE !!!");
    m_NormalTexture->Bind(1);
    m_Shader->SetInt("material.normalTexture", 1);
  } else {
    ShaderManager::GetInstance()->white_texture->Bind(1);
    m_Shader->SetInt("material.normalTexture", 1);
  }
  if (m_SpecularTexture != nullptr) {
    // LOG_INFO("NORMAL TEXTURE !!!");
    m_SpecularTexture->Bind(2);
    m_Shader->SetInt("material.normalTexture", 2);
  } else {
    ShaderManager::GetInstance()->white_texture->Bind(2);
    m_Shader->SetInt("material.specularTexture", 2);
  }
}

void PBRMaterial::Init() {
  m_Shader = std::make_shared<Shader>("mesher_resources/Shaders/irradiance/2.2.2.pbr.vs",
                                      "mesher_resources/Shaders/irradiance/2.2.2.pbr.fs");
}

void PBRMaterial::SetUniforms() {
  m_Shader->SetVec3("u_albedo", m_Albedo);

  // start binding at 3 because of IBL taking ,  and 2;

  unsigned int bind_id = 3;
  if (m_AlbedoTexture != nullptr) {
    m_AlbedoTexture->Bind(bind_id);
    m_Shader->SetInt("albedoMap", bind_id);
  } else {
    ShaderManager::GetInstance()->white_texture->Bind(bind_id);
    m_Shader->SetInt("albedoMap", bind_id);
  }

  bind_id++;

  if (m_NormalTexture != nullptr) {
    m_NormalTexture->Bind(bind_id);
    m_Shader->SetInt("normalMap", bind_id);
  } else {
    ShaderManager::GetInstance()->white_texture->Bind(bind_id);
    m_Shader->SetInt("normalMap", bind_id);
  }

  bind_id++;

  if (m_MetallicTexture != nullptr) {
    m_MetallicTexture->Bind(bind_id);
    m_Shader->SetInt("metallicMap", bind_id);
  } else {
    ShaderManager::GetInstance()->very_dark_gray_texture->Bind(bind_id);
    m_Shader->SetInt("metallicMap", bind_id);
  }

  bind_id++;

  if (m_RoughnessTexture != nullptr) {
    m_RoughnessTexture->Bind(bind_id);
    m_Shader->SetInt("roughnessMap", bind_id);
  } else {
    ShaderManager::GetInstance()->dark_gray_texture->Bind(bind_id);
    m_Shader->SetInt("roughnessMap", bind_id);
  }

  bind_id++;

  if (m_AOTexture != nullptr) {
    m_AOTexture->Bind(bind_id);
    m_Shader->SetInt("aoMap", bind_id);
  } else {
    ShaderManager::GetInstance()->white_texture->Bind(bind_id);
    m_Shader->SetInt("aoMap", bind_id);
  }
}
void PBRMaterial::LoadTexturePaths(PBRMaterialTexturePaths paths) {
  if (fs::exists(paths.AlbedoTexture) && !fs::is_empty(paths.AlbedoTexture)) {
    auto texture = std::make_shared<Texture>();
    texture->Load(paths.AlbedoTexture.string());
    m_AlbedoTexture = texture;
  }

  if (fs::exists(paths.NormalTexture) && !fs::is_empty(paths.NormalTexture)) {
    auto texture = std::make_shared<Texture>();
    texture->Load(paths.NormalTexture.string());
    m_NormalTexture = texture;
  }

  if (fs::exists(paths.MetallicTexture) && !fs::is_empty(paths.MetallicTexture)) {
    auto texture = std::make_shared<Texture>();
    texture->Load(paths.MetallicTexture.string());
    m_MetallicTexture = texture;
  }

  if (fs::exists(paths.RoughnessTexture) && !fs::is_empty(paths.RoughnessTexture)) {
    auto texture = std::make_shared<Texture>();
    texture->Load(paths.RoughnessTexture.string());
    m_RoughnessTexture = texture;
  }

  if (fs::exists(paths.AOTexture) && !fs::is_empty(paths.AOTexture)) {
    auto texture = std::make_shared<Texture>();
    texture->Load(paths.AOTexture.string());
    m_AOTexture = texture;
  }
}

};  // namespace GLR