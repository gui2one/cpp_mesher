#include "OpenGLFrameBuffer.h"
// #include <glad/glad.h>
namespace GLR {


FrameBuffer::FrameBuffer()
{
	glGenFramebuffers(1, &m_ID);
	
}

FrameBuffer::~FrameBuffer()
{
	// glDeleteFramebuffers(1, &m_ID);
	// for (auto element : m_Layout.m_Elements) {
	// 	LOG_INFO("Delete framebuffer Element");
	// 	if (element->textureID) {

	// 		glDeleteTextures(1, &element->textureID);
	// 	}
	// }
	LOG_INFO("FrameBuffer Destructor");

}

void FrameBuffer::Invalidate(uint32_t width, uint32_t height)
{
	m_Width = width * m_Supersampling;
	m_Height = height * m_Supersampling;

	//LOG_TRACE("Invalidate {} {}", m_Width, m_Height);

	for (auto& element : m_Layout.m_Elements) {
		if (element->textureID)
		{
			glDeleteTextures(1, &element->textureID);         
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
 

	glDeleteFramebuffers(1, &m_ID);
	

	if (m_Width < 16)
		m_Width = 16;
	if (m_Height < 16)
		m_Height = 16;

	glGenFramebuffers(1, &m_ID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_ID);

	for (auto& element : m_Layout.m_Elements) {

		glGenTextures(1, &element->textureID);
		glBindTexture(GL_TEXTURE_2D, element->textureID);

		if (element->attachment == GL_COLOR_ATTACHMENT0) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}

		if (element->attachment ==  GL_COLOR_ATTACHMENT1) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);

		glTexImage2D(GL_TEXTURE_2D, 0, element->internalFormat, m_Width, m_Height, 0, element->format, element->type, nullptr);
		glFramebufferTexture2D(GL_FRAMEBUFFER, element->attachment, GL_TEXTURE_2D, element->textureID, 0);
		
		
		glBindTexture(GL_TEXTURE_2D, 0);

	}

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		LOG_ERROR("PROBLEM with FrameBuffer !!");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned char* FrameBuffer::ReadPixel(GLenum attachment, int x, int y)
{
	unsigned char pixelData[3];
	if (m_ID != 0) {

		glReadBuffer(attachment);
		glReadPixels(x, y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &pixelData);
	}
	return pixelData;
}

void FrameBuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
	glViewport(0, 0, m_Width, m_Height);

	GLuint buffers[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 }; // this assumes that there is another     texture that is for the render buffer. Color attachment1 is preserved for the element ids.
	glDrawBuffers(2, buffers);
}
void FrameBuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

}
