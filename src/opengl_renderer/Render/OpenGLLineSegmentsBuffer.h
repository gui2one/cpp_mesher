#ifndef OPENGLLINESGEMENTSBUFFER_H
#define OPENGLLINESGEMENTSBUFFER_H
#pragma once

#include "Mesh/Mesh.h"
#include "Objects/IRenderable.h"

#include "Render/OpenGLBuffer.h"
#include "Render/OpenGLVertexArray.h"


namespace GLR {

struct LineSegments
{
	std::vector<glm::vec3> points;
	std::vector<uint32_t> indices;

	void SetPoints(std::vector<glm::vec3> pts)
	{
		points = pts;

		for(size_t i = 0; i<points.size(); i++)
		{
			indices.push_back((uint32_t)(i));
		}
	}
};
class LineSegmentsBuffer : public IRenderable
{
public:
	LineSegmentsBuffer() = default;
	virtual ~LineSegmentsBuffer() {};

	void InitRenderData() override;
	void BindRenderData() override;
	void Render() override;
	
	void SetMesh(std::shared_ptr<LineSegments> mesh){ m_LineSegments = mesh; }

	const std::shared_ptr<LineSegments> GetMesh() const { return m_LineSegments; }
		  std::shared_ptr<LineSegments> GetMesh()       { return m_LineSegments; }


private:

	std::shared_ptr<LineSegments> m_LineSegments;


	std::shared_ptr<VertexBuffer> m_VertexBuffer;
	std::shared_ptr<IndexBuffer> m_IndexBuffer;
	std::shared_ptr<VertexArray> m_VertexArray;

	// BufferLayout m_BufferLayout;

	BufferLayout m_BufferLayout = {
		{ShaderDataType::Float3, "position"}

	};    
};
} /* end namespace GLR*/

#endif /* OPENGLLINESGEMENTSBUFFER_H */