#ifndef BOXLINEOBJECT_H
#define BOXLINEOBJECT_H
#pragma once

#include "Objects/Entity3d.h"
#include "Objects/IRenderable.h"
#include "Objects/MeshObject.h"
#include "Mesh/Mesh.h"
#include "Render/OpenGLBuffer.h"
#include "Render/OpenGLVertexArray.h"

namespace GLR {

	
  
	class BoxLineObject : public Entity3d, public IRenderable
	{
	public:

	public:
		BoxLineObject();
		BoxLineObject(const BoxLineObject& other);

		BoxLineObject& operator=(const BoxLineObject& other);
		~BoxLineObject();

		void SetLineWidth(float width){ m_LineWidth = width; }
		void SetBox(BoundingBox bbox);
		void SetBox(std::shared_ptr<MeshObject> mesh_object);

		void InitRenderData() override;
		void BindRenderData() override;
		void Render() override;


		

	private:
		BoundingBox m_BoundingBox;
		std::vector<glm::vec3> m_PointPositions;
		std::vector<uint32_t> m_Indices;


		//rendering
		std::shared_ptr<VertexBuffer> m_VertexBuffer;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
		std::shared_ptr<VertexArray> m_VertexArray;

		BufferLayout m_BufferLayout = {
			{ShaderDataType::Float3, "position"}
		};   


		float m_LineWidth = 2.0f;

	};
} /* end namespace GLR*/

#endif /* BOXLINEOBJECT_H */