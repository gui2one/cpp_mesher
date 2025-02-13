#ifndef OPENGLMESHBUFFER_H
#define OPENGLMESHBUFFER_H
#pragma once

#include "Mesh/Mesh.h"
#include "Objects/IRenderable.h"

#include "Render/OpenGLBuffer.h"
#include "Render/OpenGLVertexArray.h"

namespace GLR {
class MeshBuffer : public IRenderable
{
public:
	MeshBuffer() = default;
	virtual ~MeshBuffer() {};

	void InitRenderData() override;
	void BindRenderData() override;
	void Render() override;
	
	
	// void SetLayout(BufferLayout& layout) { m_BufferLayout = layout; }
	void SetMesh(std::shared_ptr<Mesh> mesh){ m_Mesh = mesh; }

	const std::shared_ptr<Mesh> GetMesh() const { return m_Mesh; }
		  std::shared_ptr<Mesh> GetMesh()       { return m_Mesh; }


private:

	std::shared_ptr<Mesh> m_Mesh;


	std::shared_ptr<VertexBuffer> m_VertexBuffer;
	std::shared_ptr<IndexBuffer> m_IndexBuffer;
	std::shared_ptr<VertexArray> m_VertexArray;

	// BufferLayout m_BufferLayout;

	BufferLayout m_BufferLayout = {
		{ShaderDataType::Float3, "position"},
		{ShaderDataType::Float3, "normal"},
		{ShaderDataType::Float3, "color"},
		{ShaderDataType::Float2, "t_coords"}
	};    
};
} /* end namespace GLR*/

#endif