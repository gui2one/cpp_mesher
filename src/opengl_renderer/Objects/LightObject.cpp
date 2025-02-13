#include "LightObject.h"

namespace GLR {
	LightObject::LightObject():
		m_Intensity(1.0),
		m_Color(glm::vec3(1.0f,1.0f,1.0f)),
		b_IsOn(true),
		b_UseShadowMap(true),
		m_ShadowBias(0.002f),
		Entity3d() 
	{
		// LOG_INFO("LightObject Constructor");
		lineShader.LoadVertexShaderSource("Resources/Shaders/wire_shader.vert");
		lineShader.LoadFragmentShaderSource("Resources/Shaders/wire_shader.frag");
		lineShader.CreateShader();
	}

	LightObject::LightObject(char * name) : 
		m_Intensity(1.0), 
		m_Color(glm::vec3(1.0f, 1.0f, 1.0f)),
		b_IsOn(true),
		b_UseShadowMap(true),
		m_ShadowBias(0.005f),		
		Entity3d(name) 
	{
		// LOG_INFO("LightObject COPY Constructor");

		lineShader.LoadVertexShaderSource("Resources/Shaders/wire_shader.vert");
		lineShader.LoadFragmentShaderSource("Resources/Shaders/wire_shader.frag");
		lineShader.CreateShader();
	}
	void LightObject::DisplayGizmo()
	{

		if (vao == 0) {
			vertices = {
				0.0f,  0.1f ,  0.0f,
				0.0f, -0.1f ,  0.0f,
				0.1f,  0.0f ,  0.0f,
				-0.1f, 0.0f , 0.0f,
			     0.0f, 0.0f , 0.1f,
			     0.0f, 0.0f , -0.1f
			};

			indices = {
				0, 2,
				0, 3,
				0, 4,
				0, 5,

				1, 2,
				1, 3,
				1, 4,
				1, 5,

				2, 4,
				3, 5,
				5, 2,
				4, 3
			};


			glGenVertexArrays(1, &vao);
			glGenBuffers(1, &vbo);
			glGenBuffers(1, &ibo);

			glBindVertexArray(vao);

			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), GL_STATIC_DRAW);


			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		


		}

		// glPointSize(10.0);
		glLineWidth(5.0);

		//glBindBuffer(GL_ARRAY_BUFFER, vbo);
		//glDrawArrays(GL_LINES, 0, 6);
		
		glBindVertexArray(vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glDrawElements(GL_LINES, (GLsizei)24, GL_UNSIGNED_INT, nullptr);
	}
}