#ifndef OPENGLSHADER_H
#define OPENGLSHADER_H
#pragma once

#include "Shadinclude.hpp"

namespace GLR {

	static std::string loadShaderFile(std::filesystem::path path);

	class Shader
	{

	public:
		Shader();
		Shader(std::filesystem::path vertex_path, std::filesystem::path fragment_path, std::filesystem::path geometry_path = "");
		~Shader();
		void LoadVertexShaderSource(std::filesystem::path filePath);
		void LoadFragmentShaderSource(std::filesystem::path filePath);
		void LoadGeometryShaderSource(std::filesystem::path filePath);


		unsigned int CompileShader(unsigned int type, const std::string& source);
		unsigned int CreateShader();

		std::string GetVertexShaderSource();
		std::string GetFragmentShaderSource();
		std::string GetGeometryShaderSource();

		uint32_t GetID() { return m_ID; }
		
		void UseProgram();

		inline void SetFloat(const char* location_name, float value)
		{
			GLint location = glGetUniformLocation(m_ID, location_name);
			glUniform1f(location, value);
		};

		inline void SetFloatArray(const char* location_name, std::vector<float> values)
		{
			GLint location = glGetUniformLocation(m_ID, location_name);
			glUniform1fv(location, (GLsizei)values.size(), &values[0]);
		};

		inline void SetVec3(const char *location_name, glm::vec3& clr)
		{
			GLint location = glGetUniformLocation(m_ID, location_name);
			glUniform3fv(location, 1, glm::value_ptr(clr));
		};
		
		inline void SetVec4(const char *location_name, glm::vec4& clr)
		{
			GLint location = glGetUniformLocation(m_ID, location_name);
			glUniform4fv(location, 1, glm::value_ptr(clr));
		};

		inline void SetVec3Array(const char* location_name, std::vector<glm::vec3>& values)
		{
			GLint location = glGetUniformLocation(m_ID, location_name);
			glUniform3fv(location, (GLsizei)values.size(), &values[0].x);
		};

		inline void SetInt(const char* location_name, int value)
		{
			GLint location = glGetUniformLocation(m_ID, location_name);
			glUniform1i(location, value);
		};

		inline void SetUInt(const char* location_name, unsigned int value)
		{
			GLint location = glGetUniformLocation(m_ID, location_name);
			glUniform1ui(location, value);
		};

		inline void SetMat4(const char* location_name, glm::mat4& value)
		{
			glUniformMatrix4fv(glGetUniformLocation(m_ID, location_name), 1, GL_FALSE, glm::value_ptr(value));
		};

	public:
		uint32_t m_ID;
		std::string vertexShaderSource;

		std::string fragmentShaderSource;

		std::string geometryShaderSource;

	private:

	};
}

#endif /* OPENGLSHADER_H */