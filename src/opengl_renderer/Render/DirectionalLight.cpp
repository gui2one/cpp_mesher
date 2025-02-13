#include "DirectionalLight.h"

namespace GLR {
DirectionalLight::DirectionalLight()
: m_ZNear(0.1f), m_ZFar(50.f), m_Size(20.0f),
	m_Projection( glm::ortho(-10.f, 10.f, -10.f, 10.0f, m_ZNear, m_ZFar)),
	LightObject()
{
	// LOG_INFO("DirectionalLight Constructor");
	m_Target = std::make_shared<Entity3d>();

	// m_Target->SetParent(this);

	m_ShadowMap = std::make_shared<ShadowMap>();
	m_ShadowMap->Init();

}

DirectionalLight::DirectionalLight(const DirectionalLight &other)
	:LightObject(other)
{
	LOG_INFO("DirectionalLight COPY Constructor");
	m_ZNear = other.m_ZNear;
	m_ZFar = other.m_ZFar;
	m_Size = other.m_Size;
	m_Projection = other.m_Projection;
	m_Target = other.m_Target;
}

DirectionalLight::~DirectionalLight()
{

}
void DirectionalLight::UpdateProjection()
{
	// LOG_INFO("Update Light Projection");
	m_Projection = glm::ortho(-m_Size/2.0f, m_Size/2.0f, -m_Size/2.0f, m_Size/2.0f, m_ZNear, m_ZFar);
}
glm::mat4 DirectionalLight::GetLightSpaceMatrix()
{
	glm::mat4 light_view = glm::mat4(1.0f);
	light_view *= glm::lookAt(
		GetPosition(),
		m_Target->GetPosition(),
		glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 lightSpaceMatrix = GetProjection() * light_view;

	return lightSpaceMatrix;
}
glm::vec3 DirectionalLight::GetDirection()
{
    return glm::normalize(GetPosition() - m_Target->GetPosition());
}
} /* end namespace GLR*/