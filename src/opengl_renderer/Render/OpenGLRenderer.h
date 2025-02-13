#ifndef OPENGLRENDERER_H
#define OPENGLRENDERER_H

#pragma once

#include "Scene.h"
#include "Camera.h"
#include "Render/OpenGLShader.h"
#include "Render/OpenGLLayer.h"
#include "Render/DirectionalLight.h"
#include "Render/SpotLight.h"
#include "Render/PointLight.h"
#include "Render/OpenGLTexture.h"
#include "ShaderManager.h"
namespace GLR{

class OpenGLRenderer
{
public:
    OpenGLRenderer();
    ~OpenGLRenderer();

    void Init();
    void InitTextures();

    void CollectLightsUniforms(const Scene& scene);
    void BindShadowMaps(const Scene &scene, std::shared_ptr<Shader> shader);
    
    void RenderShadowPass(std::shared_ptr<Layer> layer, const Scene& scene, std::shared_ptr<Camera> camera);
    void RenderObjects(std::shared_ptr<Layer> layer, const Scene& scene, std::shared_ptr<Camera> camera);
    void Render(std::shared_ptr<Layer> layer, const Scene& scene, std::shared_ptr<Camera> camera);
private:

    std::shared_ptr<Material> m_DefaultMaterial;
    std::shared_ptr<Texture> white_texture;
    std::shared_ptr<Texture> uv_grid_texture;

    ShaderManager* m_ShaderManager;

    GLuint m_DirectionalLightsUbo;
    GLuint m_SpotLightsUbo;
    GLuint m_PointLightsUbo;

    uint32_t m_NumDirectionalLights = 0;
    GLuint m_DirectionalLightsBindingPoint = 0;
    uint32_t m_NumSpotLights = 0;
    GLuint m_SpotLightsBindingPoint = 0;
    uint32_t m_NumPointLights = 0;
    GLuint m_PointLightsBindingPoint = 0;

    std::vector<std::string> m_DirectionalLightsShadowMapUniformNames;
    std::vector<std::string> m_SpotLightsShadowMapUniformNames;
    std::vector<std::string> m_PointLightsShadowMapUniformNames;
};
}

#endif