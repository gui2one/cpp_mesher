#include "OpenGLTexture.h"

#include "stb_image.h"
namespace GLR {


	Texture::Texture() : m_ID(0), m_Width(0), m_Height(0), m_BitsPerPixel(0)
	{
	}

	void Texture::Load(std::string path)
	{
		glDeleteTextures(1, &m_ID);
		glGenTextures(1, &m_ID);
		glBindTexture(GL_TEXTURE_2D, m_ID);

		stbi_set_flip_vertically_on_load(1);

		unsigned char* buffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BitsPerPixel, 4);

		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);

		if (buffer == nullptr)
		{
			LOG_WARN("Error Loading Texture : {} -- {}", path, stbi_failure_reason());

		}
		else {
			// LOG_INFO("Loaded Texture : {}", path);
		}

		if (buffer) {

			stbi_image_free(buffer);
		}

		// delete buffer;
	}


    void Texture::SetData(int _width, int _height, unsigned char* buffer)
	{

		glDeleteTextures(1, &m_ID);
		glGenTextures(1, &m_ID);
		glBindTexture(GL_TEXTURE_2D, m_ID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &buffer[0]);

		glBindTexture(GL_TEXTURE_2D, 0);

		m_Width = _width;
		m_Height = _height;

		//~ std::cout << "__ setting texture data : " << _width << "/" << _height  << "\n";
	}

	void Texture::Bind(unsigned int slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_ID);
		//glBindTextureUnit(slot, id);
	}

	void Texture::Unbind(unsigned int slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	Texture::~Texture()
	{

		glDeleteTextures(1, &m_ID);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
