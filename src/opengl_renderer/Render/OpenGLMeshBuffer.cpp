#include "OpenGLMeshBuffer.h"
namespace GLR {


void MeshBuffer::InitRenderData()
{

	m_VertexBuffer = std::make_shared<VertexBuffer>((float*)m_Mesh->GetPoints().data(), (uint32_t)m_Mesh->GetPoints().size() * (uint32_t)sizeof(GLR::Point));
	m_VertexBuffer->setLayout(m_BufferLayout);

	m_IndexBuffer = std::make_shared<IndexBuffer>((uint32_t*)m_Mesh->GetIndices().data(), (uint32_t)m_Mesh->GetIndices().size() * (uint32_t)sizeof(uint32_t));

	m_VertexArray = std::make_shared<VertexArray>();	
	m_VertexArray->SetIndexBuffer(m_IndexBuffer);	
	m_VertexArray->addVertexBuffer(m_VertexBuffer);    
}

void MeshBuffer::BindRenderData()
{
	m_VertexArray->Bind();
	m_IndexBuffer->Bind();    
}

void MeshBuffer::Render()
{
	BindRenderData();

	glDrawElements(GL_TRIANGLES, (GLsizei)m_Mesh->GetIndices().size(), GL_UNSIGNED_INT, nullptr);
}

} /* end namespace GLR*/