#include "OpenGLVertexArray.h"
#include "../Mesh/Mesh.h"
namespace GLR 
{
	VertexArray::VertexArray()
	{
		glGenVertexArrays(1, &m_id);
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &m_id);
	}

	void VertexArray::Bind()
	{
		glBindVertexArray(m_id);
	}

	void VertexArray::unbind()
	{
		glBindVertexArray(0);
	}

	GLenum VertexArray::ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::None:
			return 0;
		case ShaderDataType::Float:
			return GL_FLOAT;
		case ShaderDataType::Float2:
			return GL_FLOAT;
		case ShaderDataType::Float3:
			return GL_FLOAT;
		case ShaderDataType::Float4:
			return GL_FLOAT;
		case ShaderDataType::Mat3:
			return GL_FLOAT;
		case ShaderDataType::Mat4:
			return GL_FLOAT;
		case ShaderDataType::Int:
			return GL_INT;
		case ShaderDataType::Int2:
			return GL_INT;
		case ShaderDataType::Int3:
			return GL_INT;
		case ShaderDataType::Int4:
			return GL_INT;
		case ShaderDataType::Bool:
			return GL_BOOL;
		default : 
			return 0;
		}

		return 0;
	}

	void VertexArray::addVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{

		glBindVertexArray(m_id);
		int inc = 0;
		for (const auto& element : vertexBuffer->getLayout().GetElements()) 
		{

			glEnableVertexAttribArray(inc);
			glVertexAttribPointer(
				inc,                                          /* index      */
				element.GetNumComponents(),                   /* size       */
				ShaderDataTypeToOpenGLBaseType(element.Type), /* enum type  */
				GL_FALSE,                                     /* normalized */
				vertexBuffer->getLayout().GetStride(),        /* stride     */
				(void*)((uint16_t)element.Offset));

			inc++;
		}

		glBindVertexArray(0);
	}

	void VertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		m_indexBuffer = indexBuffer;
	}
}
