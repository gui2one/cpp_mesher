#include "OpenGLLineSegmentsBuffer.h"

namespace GLR 
{

	void LineSegmentsBuffer::InitRenderData()
	{
		m_IndexBuffer = std::make_shared<IndexBuffer>((uint32_t*)m_LineSegments->indices.data(), (uint32_t)m_LineSegments->indices.size() * (uint32_t)sizeof(uint32_t));
		
		// m_VertexArray.reset(new VertexArray());
		m_VertexArray = std::make_shared<VertexArray>();
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);
		m_VertexBuffer = std::make_shared<VertexBuffer>((float*)m_LineSegments->points.data(), (uint32_t)m_LineSegments->points.size() * (uint32_t)sizeof(glm::vec3));

		m_VertexBuffer->setLayout(m_BufferLayout);
		m_VertexArray->addVertexBuffer(m_VertexBuffer);    
	}

	void LineSegmentsBuffer::BindRenderData()
	{
		m_VertexArray->Bind();
		m_IndexBuffer->Bind();    
	}

	void LineSegmentsBuffer::Render()
	{
		BindRenderData();
		glLineWidth(1.0f);
		glDrawElements(GL_LINES, (GLsizei)m_LineSegments->indices.size(), GL_UNSIGNED_INT, nullptr);
	}

} /* end namespace GLR*/