#include "OpenGLShadowMap.h"

namespace GLR {
	ShadowMap::ShadowMap()
	{

	}

	ShadowMap::~ShadowMap()
	{
		glDeleteBuffers(1, &m_DepthFBO);
	}
	void ShadowMap::Init()
	{

		glGenFramebuffers(1, &m_DepthFBO);
		glGenTextures(1, &m_DepthMap);

		glBindTexture(GL_TEXTURE_2D, m_DepthMap);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, m_Resolution, m_Resolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		// float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		// glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

		glBindTexture(GL_TEXTURE_2D, 0);


		glBindTexture(GL_TEXTURE_2D, 0);

		glBindFramebuffer(GL_FRAMEBUFFER, m_DepthFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthMap, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	
	void ShadowMap::BindFBO()
	{

		glBindFramebuffer(GL_FRAMEBUFFER, m_DepthFBO);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthMap, 0);

		// glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glViewport(0, 0, m_Resolution, m_Resolution);

		// GLuint buffers[2] = { GL_COLOR_ATTACHMENT0, GL_DEPTH_ATTACHMENT }; // this assumes that there is another     texture that is for the render buffer. Color attachment1 is preserved for the element ids.
		// glDrawBuffers(2, buffers);
 

	}

	void ShadowMap::Bind(unsigned int slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_DepthMap);
	}

	void ShadowMap::Unbind(unsigned int slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
} /* end namespace GLR*/