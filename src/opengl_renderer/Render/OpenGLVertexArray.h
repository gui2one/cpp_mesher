#ifndef OPENGL_VERTEX_ARRAY_H
#define OPENGL_VERTEX_ARRAY_H
#pragma once


#include "OpenGLBuffer.h"

namespace GLR {


class VertexArray
{
public:
	VertexArray();
	virtual ~VertexArray();

	void Bind();
	void unbind();

	GLuint ShaderDataTypeToOpenGLBaseType(ShaderDataType type);
	void addVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer);
	void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer);

	uint32_t getID() { return m_id; };

private:
	uint32_t m_id;
	std::vector<std::shared_ptr<VertexBuffer>> m_vertexBuffers;
	std::shared_ptr<IndexBuffer> m_indexBuffer;
};
}

#endif /* OPENGL_VERTEX_ARRAY_H */
