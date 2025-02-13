#include "Camera.h"
namespace GLR {

Camera::Camera(): Entity3d()
{
	m_Name = "User Camera";
}

Camera::Camera(const Camera &other) : Entity3d(other)
{
	m_screen_ratio =  other.m_screen_ratio;
	target_position =  other.target_position;
	up_vector =  other.up_vector;
	m_Projection = other.m_Projection;
}

Camera::Camera(float fov_angle, float ratio)
: m_near(0.1f), m_far(20.0f)
{
	m_Name = "User Camera";
	m_fov_angle = fov_angle;
	m_screen_ratio = ratio;

	m_Position = glm::vec3(1.0f, 1.0f, -1.0f);
	target_position = glm::vec3(0.0f, 0.0f, 0.0f);
	up_vector = glm::vec3(0.0f, 0.0f, 1.0f);
	SetProjection(m_fov_angle, m_screen_ratio, m_near, m_far);
	
}

void Camera::SetProjection(float angle, float screen_ratio, float _near = 0.1f, float _far = 10.0f)
{
	m_Projection = glm::mat4(1.0f) * glm::perspective(angle, screen_ratio, _near, _far);
}

void Camera::SetFovAngle(float angle)
{
	m_Projection = glm::mat4(1.0f) * glm::perspective(angle, m_screen_ratio, m_near, m_far);
}

void Camera::SetNear(float _near)
{
	m_Projection = glm::mat4(1.0f) * glm::perspective(m_fov_angle, m_screen_ratio, _near, m_far);
}

void Camera::SetFar(float _far)
{
	m_far = _far;
	m_Projection = glm::mat4(1.0f) * glm::perspective(m_fov_angle, m_screen_ratio, m_near, m_far);
}


void Camera::SetScreenRatio(float ratio)
{
	m_Projection = glm::mat4(1.0f) * glm::perspective(m_fov_angle, ratio, m_near, m_far);
}

}