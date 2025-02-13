#ifndef CAMERA_H
#define CAMERA_H
#pragma once

#include "Objects/Entity3d.h"

namespace GLR {

class Camera : public Entity3d
{
public:


public:
	Camera();
	Camera(const Camera& other);
	Camera(float _fov_angle, float ratio);
	void SetProjection(float angle, float screen_ratio, float near, float far);
	void SetFovAngle(float angle);
	void SetNear(float near);
	void SetFar(float far);
	void SetScreenRatio(float ratio);

	glm::vec3 target_position;
	glm::vec3 up_vector;

	glm::mat4 m_Projection;

private:
	float m_fov_angle = 45.0f;
	float m_screen_ratio = 1.0f;
	float m_near = 0.1f;
	float m_far = 10.0f;
};

}
#endif /*CAMERA_H*/