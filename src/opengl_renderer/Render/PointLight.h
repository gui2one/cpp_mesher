#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#pragma once

#include "Objects/LightObject.h"
#include "Render/OpenGLCubeShadowMap.h"
namespace GLR
{

struct PointLightUniform
{
    float intensity;
    float shadowBias;  	

	float near_plane;
	float far_plane;
    alignas(16) glm::vec3 color;
    alignas(16) glm::vec3 position;
};

class PointLight : public LightObject
{
public:
    PointLight();
    PointLight(const PointLight& other);
    ~PointLight();


	inline float GetZNear() const { return m_ZNear; } 
	inline float GetZFar() const { return m_ZFar; }

	inline void SetZNear(float val){ m_ZNear = val; }
	inline void SetZFar (float val){ m_ZFar  = val;	}

    std::shared_ptr<CubeShadowMap> GetShadowMap()const { return m_ShadowMap; }

private:

    float m_ZNear;
    float m_ZFar;
    std::shared_ptr<CubeShadowMap> m_ShadowMap;
};
} // namespace GLR


#endif