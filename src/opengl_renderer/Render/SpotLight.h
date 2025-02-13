#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#pragma once


#include "Objects/LightObject.h"
#include "Render/OpenGLShadowMap.h"

namespace GLR
{

struct  SpotLightUniform
{
    float intensity;
    float shadowBias;  	

    float angle;

	float near_plane;
	float far_plane;
    alignas(16) glm::vec3 color;
    alignas(16) glm::vec3 position;
    alignas(16) glm::vec3 direction;
	alignas(16) glm::mat4 lightSpaceMatrix;
};

class SpotLight : public LightObject
{
public:
    SpotLight();
    SpotLight(const SpotLight& other);
    ~SpotLight();


	inline float GetZNear() const { return m_ZNear; } 
	inline float GetZFar() const { return m_ZFar; }

	inline void SetZNear(float val){ 
		m_ZNear = val; 
		UpdateProjection();
	}
	
	inline void SetZFar (float val){ 
		m_ZFar  = val; 
		UpdateProjection();    
	}

	inline void SetAngle(float angle){
		m_Angle = angle;
		UpdateProjection();
	}

	inline float GetAngle() { return m_Angle;}
	inline void SetTargetPosition(float x, float y, float z){ m_Target->SetPosition(glm::vec3(x, y, z)); }
	inline void SetTargetPosition(glm::vec3 target_pos){ m_Target->SetPosition(target_pos); }

	inline void SetProjection(glm::mat4 proj){ m_Projection = proj; }

	void UpdateProjection();
	
	inline std::shared_ptr<Entity3d>       GetTarget(){ return m_Target; }
	inline const std::shared_ptr<Entity3d> GetTarget() const { return m_Target; }
	
	inline void SetTarget(std::shared_ptr<Entity3d> target){ m_Target = target; }

	std::shared_ptr<ShadowMap> GetShadowMap()const { return m_ShadowMap; }

	inline glm::vec3 GetTargetPosition(){ return m_Target->GetPosition(); }

	inline glm::mat4 GetProjection(){ return m_Projection; }

	glm::mat4 GetLightSpaceMatrix();

	glm::vec3 GetDirection();

private:
	float m_ZNear;
	float m_ZFar;
	std::shared_ptr<Entity3d> m_Target;
	glm::mat4 m_Projection;

	float m_Angle;
	std::shared_ptr<ShadowMap> m_ShadowMap;
}; 
} // namespace GLR


#endif