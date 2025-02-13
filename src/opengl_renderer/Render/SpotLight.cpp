#include "SpotLight.h"

namespace GLR
{
SpotLight::SpotLight()
: m_ZNear(0.1f), m_ZFar(50.f), m_Angle(20.0f),
	m_Projection( glm::perspective(glm::radians(m_Angle), 1.0f, m_ZNear, m_ZFar)),
	LightObject()
{
	// LOG_INFO("SpotLight Constructor");
	m_Target = std::make_shared<Entity3d>();

	m_ShadowMap = std::make_shared<ShadowMap>();
	m_ShadowMap->Init();

}

SpotLight::SpotLight(const SpotLight &other)
	:LightObject(other)
{
	// LOG_INFO("SpotLight COPY Constructor");
	m_ZNear = other.m_ZNear;
	m_ZFar = other.m_ZFar;
	m_Angle = other.m_Angle;
	m_Projection = other.m_Projection;
	m_Target = other.m_Target;
}

SpotLight::~SpotLight()
{

}

void SpotLight::UpdateProjection()
{
    m_Projection = glm::perspective(glm::radians(m_Angle), 1.0f, m_ZNear, m_ZFar);
}

glm::mat4 SpotLight::GetLightSpaceMatrix()
{
	glm::mat4 light_view = glm::mat4(1.0f);
	light_view *= glm::lookAt(
		GetPosition(),
		m_Target->GetPosition(),
		glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 lightSpaceMatrix = GetProjection() * light_view;

	return lightSpaceMatrix;
}
glm::vec3 SpotLight::GetDirection()
{
    return glm::normalize(GetPosition() - m_Target->GetPosition());
}
} // namespace GLR

