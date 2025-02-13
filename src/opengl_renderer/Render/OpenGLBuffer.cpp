#include "OpenGLBuffer.h"

namespace GLR {


//////////////////////////////////////////////////////
//////   OpenGLVertexBuffer
//////////////////////////////////////////////////////

VertexBuffer::VertexBuffer(float *vertices, uint32_t size)
{

	glGenBuffers(1, &m_ID);
	glBindBuffer(GL_ARRAY_BUFFER, m_ID);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

}

VertexBuffer::~VertexBuffer()
{

	glDeleteBuffers(1, &m_ID);
}
void VertexBuffer::bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_ID);
}

void VertexBuffer::unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//////////////////////////////////////////////////////
//////    OpenGLIndexBuffer
//////////////////////////////////////////////////////

IndexBuffer::IndexBuffer(uint32_t *indices, uint32_t size)
{
	glGenBuffers(1, &m_ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &m_ID);
}
void IndexBuffer::Bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
}

void IndexBuffer::unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
}