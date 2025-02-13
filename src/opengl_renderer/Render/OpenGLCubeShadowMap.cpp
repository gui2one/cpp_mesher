#include "OpenGLCubeShadowMap.h"

namespace GLR {
	CubeShadowMap::CubeShadowMap()
	{

	}

	CubeShadowMap::~CubeShadowMap()
	{
		glDeleteBuffers(1, &m_DepthFBO);
	}
	void CubeShadowMap::Init()
	{
		// LOG_INFO("init point shadow map : res {} ", m_Resolution);

		glGenFramebuffers(1, &m_DepthFBO);
		glGenTextures(1, &m_DepthMap);
		glGenTextures(1, &m_CubeMap);

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


		glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubeMap);
		for (unsigned int i = 0; i < 6; ++i){

				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, 
							m_Resolution, m_Resolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL); 		
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	}
	
	void CubeShadowMap::BindFBO()
	{

		glBindFramebuffer(GL_FRAMEBUFFER, m_DepthFBO);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_CubeMap, 0);

		// glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glViewport(0, 0, m_Resolution, m_Resolution);

		// GLuint buffers[2] = { GL_COLOR_ATTACHMENT0, GL_DEPTH_ATTACHMENT }; // this assumes that there is another     texture that is for the render buffer. Color attachment1 is preserved for the element ids.
		// glDrawBuffers(2, buffers);
 

	}

	void CubeShadowMap::Bind(unsigned int slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubeMap);
	}

	void CubeShadowMap::Unbind(unsigned int slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}
} /* end namespace GLR*/