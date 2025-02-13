#ifndef HELPERMESHBUFFER_H
#define HELPERMESHBUFFER_H
#pragma once

#include "Mesh/Mesh.h"
#include "Objects/IRenderable.h"

#include "OpenGLBuffer.h"
#include "OpenGLVertexArray.h"

namespace GLR {
class HelperMeshBuffer : public IRenderable
{
public:
	HelperMeshBuffer() = default;
	virtual ~HelperMeshBuffer() {};

	void SetMesh(std::shared_ptr<Mesh> mesh){ m_Mesh = mesh;}

	const std::shared_ptr<Mesh> GetMesh() const { return m_Mesh; }
		  std::shared_ptr<Mesh> GetMesh()       { return m_Mesh; }


	void InitRenderData() override;
	void BindRenderData() override;
	void Render() override;
private:

	std::shared_ptr<Mesh> m_Mesh;


	std::shared_ptr<VertexBuffer> m_VertexBuffer;
	std::shared_ptr<IndexBuffer> m_IndexBuffer;
	std::shared_ptr<VertexArray> m_VertexArray;

	BufferLayout m_BufferLayout = {
		{ShaderDataType::Float3, "position"},
		{ShaderDataType::Float3, "color"}
	};    
};
} /* end namespace GLR*/

#endif