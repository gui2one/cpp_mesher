#ifndef MATERIAL_H
#define MATERIAL_H

#pragma once

#include "Render/OpenGLTexture.h"
#include "Render/OpenGLShader.h"
namespace GLR{
enum class MaterialType{
    None,
    Basic,
    PBR
};

class Material
{
public:
    std::shared_ptr<Shader> m_Shader;
    MaterialType m_Type;
public:
    Material(){};
    virtual ~Material() = default;

    virtual void Init() = 0;
    virtual void SetUniforms() = 0;

    std::shared_ptr<Shader> GetShader(){ return m_Shader; }
    MaterialType GetType(){ return m_Type; }
    
private:



};


class BasicMaterial : public Material
{
public:
    glm::vec3 m_DiffuseColor = glm::vec3(1.0f, 1.0f, 1.0f);
    std::shared_ptr<Texture> m_DiffuseTexture;

    glm::vec3 m_SpecularColor = glm::vec3(1.0f, 1.0f, 1.0f);;
    std::shared_ptr<Texture> m_SpecularTexture;

    std::shared_ptr<Texture> m_NormalTexture;
public:
    BasicMaterial(){
        m_Type = MaterialType::Basic;
    };
    virtual ~BasicMaterial() = default;

    void Init() override;
    void SetUniforms() override;
private:
};


struct PBRMaterialTexturePaths
{
    fs::path AlbedoTexture;
    fs::path NormalTexture;
    fs::path MetallicTexture;
    fs::path RoughnessTexture;
    fs::path AOTexture;
};

class PBRMaterial : public Material
{
public:
    glm::vec3 m_Albedo = glm::vec3(1.0f, 1.0f, 1.0f);
    PBRMaterialTexturePaths m_TexturePaths;


public:
    PBRMaterial(){
        m_Type = MaterialType::Basic;
    };
    virtual ~PBRMaterial() = default;

    void Init() override;
    void SetUniforms() override;

    void LoadTexturePaths(PBRMaterialTexturePaths paths);

    std::shared_ptr<Texture> m_AlbedoTexture;
    std::shared_ptr<Texture> m_NormalTexture;
    std::shared_ptr<Texture> m_MetallicTexture;
    std::shared_ptr<Texture> m_RoughnessTexture;
    std::shared_ptr<Texture> m_AOTexture;
private:
};
}

#endif