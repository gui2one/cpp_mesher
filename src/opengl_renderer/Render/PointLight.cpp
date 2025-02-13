#include "PointLight.h"

namespace GLR
{
    
    PointLight::PointLight()
        : m_ZNear(0.1f), 
        m_ZFar(50.f),
        LightObject()
    {
        // LOG_INFO("PointLight Constructor");

        m_ShadowMap = std::make_shared<CubeShadowMap>();
        m_ShadowMap->Init();

    }

    PointLight::PointLight(const PointLight &other)
        :LightObject(other)
    {
        // LOG_INFO("SpotLight COPY Constructor");
        m_ZNear = other.m_ZNear;
        m_ZFar = other.m_ZFar;
    }

    PointLight::~PointLight()
    {

    }
} // namespace GLR
