#include "OpenGLHDRTexture.h"

#include "stb_image.h"
namespace GLR {


	HDRTexture::HDRTexture() : m_ID(0), m_Width(0), m_Height(0), m_NumComponents(0)
	{
	}

	void HDRTexture::Load(std::filesystem::path path)
	{
		glDeleteTextures(1, &m_ID);
		glGenTextures(1, &m_ID);
		glBindTexture(GL_TEXTURE_2D, m_ID);

		stbi_set_flip_vertically_on_load(1);

		float* buffer = stbi_loadf((const char *)path.string().c_str(), &m_Width, &m_Height, &m_NumComponents, 0);

		//LOG_INFO("Loaded {1}bits HDR file  width -> {2}, height -> {3} : {0} ", path.string(), m_NumComponents, m_Width, m_Height);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_Width, m_Height, 0, GL_RGB, GL_FLOAT, buffer);


		glBindTexture(GL_TEXTURE_2D, 0);

		if (buffer == nullptr)
		{
			LOG_WARN("stbi_failure_reason() :\n {}", stbi_failure_reason());

		}
		else {
			//LOG_INFO("Success loading Texture : {}", path);
		}


		delete buffer;
	}

	void HDRTexture::SetData(std::vector<float> data, uint32_t width, uint32_t height)
	{

		m_Width = width; 
		m_Height = height; 
		glDeleteTextures(1, &m_ID);
		glGenTextures(1, &m_ID);
		glBindTexture(GL_TEXTURE_2D, m_ID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_Width, m_Height, 0, GL_RGB, GL_FLOAT, data.data());


		glBindTexture(GL_TEXTURE_2D, 0);

	}

	void HDRTexture::Bind(unsigned int slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_ID);
	}

	void HDRTexture::Unbind(unsigned int slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	HDRTexture::~HDRTexture()
	{

		glDeleteTextures(1, &m_ID);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
