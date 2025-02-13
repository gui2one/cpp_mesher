#ifndef LIGHTOBJECT_H
#define LIGHTOBJECT_H
#pragma once

#include "Entity3d.h"
#include "Render/OpenGLShader.h"

namespace GLR {

class LightObject : public Entity3d {
public: 
	float m_Intensity;
	glm::vec3 m_Color;
	
	
	bool b_IsOn;
	bool b_UseShadowMap;
	float m_ShadowBias;

	Shader lineShader;

public:
	LightObject();
	LightObject(char* name);
	virtual ~LightObject() {};

	void DisplayGizmo();

	
	void SetShadowBias(float bias){ m_ShadowBias = bias; }
private:
	uint32_t vao = 0;
	uint32_t vbo = 0;
	uint32_t ibo = 0;



	std::vector<float> vertices;
	std::vector<unsigned int> indices;


};
}

#endif /* LIGHTOBJECT_H */