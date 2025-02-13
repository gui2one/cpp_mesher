#ifndef OPENGLFRAMEBUFFER_H
#define OPENGLFRAMEBUFFER_H
#pragma once

namespace GLR 
{


struct OpenGLFrameBufferLayoutElement {

	uint32_t textureID;
	GLenum attachment;
	int internalFormat;
	GLenum format;
	GLenum type;
};
struct OpenGLFrameBufferLayout {
	OpenGLFrameBufferLayout() {}

	void AddElement(std::shared_ptr<OpenGLFrameBufferLayoutElement> element) {
		m_Elements.push_back(element);
	}

	std::vector<std::shared_ptr<OpenGLFrameBufferLayoutElement>> m_Elements;    
};


class FrameBuffer
{

public : 

	OpenGLFrameBufferLayout m_Layout;
	uint32_t m_Supersampling = 1;
public:
	FrameBuffer();
	virtual ~FrameBuffer();


	void AddLayoutElement(std::shared_ptr<OpenGLFrameBufferLayoutElement> element){
		m_Layout.AddElement(element);
	}

	void Bind();
	void Unbind();
	void Invalidate(uint32_t width, uint32_t height);
	unsigned char* ReadPixel(GLenum attachment, int x, int y);


	const uint32_t GetWidth() const { return m_Width; };
	const uint32_t GetHeight() const { return m_Height; };
	const uint32_t GetID() const { return m_ID; };

	void SetWidth(uint32_t width) { m_Width = width; };
	void SetHeight(uint32_t height) { m_Height = height; };

private:
	uint32_t m_ID;
	uint32_t m_ColorAttachment;
	uint32_t m_DepthAttachment;

	uint32_t m_Width;
	uint32_t m_Height;

};

}

#endif /* OPENGLFRAMEBUFFER_H */