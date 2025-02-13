#ifndef OPENGLSKYBOX_H
#define OPENGLSKYBOX_H
#pragma once

#include <Render/OpenGLShader.h>
#include <Render/Camera.h>
#include "ShaderManager.h"

namespace GLR{

	class Skybox{
	public:

	public:
		Skybox();
		virtual ~Skybox(){};


		void Init(
			std::filesystem::path right, 
			std::filesystem::path left, 
			std::filesystem::path top,
			std::filesystem::path bottom,
			std::filesystem::path front,
			std::filesystem::path back			
		);
		void Bind();
		void Draw(std::shared_ptr<Camera> camera);
		uint32_t m_ID;
		uint32_t m_Vao;
		uint32_t m_Vbo;

		std::shared_ptr<Shader> m_Shader;


	private:

	};
}

#endif /* OPENGLSKYBOX_H */
