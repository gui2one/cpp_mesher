#include "BoxLineObject.h"

namespace GLR {
	BoxLineObject::BoxLineObject():Entity3d("what a name !")
	{
	}

	BoxLineObject::BoxLineObject(const BoxLineObject& other) 
		: Entity3d(other)
	{
		// LOG_INFO("\nCOPY Constructor Call");
		m_PointPositions = other.m_PointPositions;
		m_Indices = other.m_Indices;
		m_VertexBuffer = other.m_VertexBuffer;
		m_IndexBuffer = other.m_IndexBuffer;
		m_VertexArray = other.m_VertexArray;
	}

	BoxLineObject& BoxLineObject::operator=(const BoxLineObject &other)
	{
		// LOG_INFO("\nOperator = Overload");
		m_PointPositions = other.m_PointPositions;
		m_Indices = other.m_Indices;
		m_VertexBuffer = other.m_VertexBuffer;
		m_IndexBuffer = other.m_IndexBuffer;
		m_VertexArray = other.m_VertexArray;

		return *this;
	}

	BoxLineObject::~BoxLineObject()
	{

	}

	void BoxLineObject::SetBox(BoundingBox bbox)
	{
		m_BoundingBox = bbox;
		InitRenderData();
	}

    void BoxLineObject::SetBox(std::shared_ptr<MeshObject> mesh_object)
    {
		m_BoundingBox = mesh_object->GetBoundingBox();
		InitRenderData();
		SetParent(mesh_object.get());
    }

    void BoxLineObject::InitRenderData()
	{
		m_PointPositions.clear();

		auto pt0 = m_BoundingBox.min;
		auto pt1 = glm::vec3(m_BoundingBox.max.x, m_BoundingBox.min.y , m_BoundingBox.min.z);
		auto pt2 = glm::vec3(m_BoundingBox.max.x, m_BoundingBox.max.y , m_BoundingBox.min.z);
		auto pt3 = glm::vec3(m_BoundingBox.min.x, m_BoundingBox.max.y , m_BoundingBox.min.z);

		auto pt4 = glm::vec3(m_BoundingBox.min.x, m_BoundingBox.min.y, m_BoundingBox.max.z);
		auto pt5 = glm::vec3(m_BoundingBox.max.x, m_BoundingBox.min.y, m_BoundingBox.max.z);
		auto pt6 = glm::vec3(m_BoundingBox.max.x, m_BoundingBox.max.y, m_BoundingBox.max.z);
		auto pt7 = glm::vec3(m_BoundingBox.min.x, m_BoundingBox.max.y, m_BoundingBox.max.z);

		m_PointPositions.push_back(pt0);
		m_PointPositions.push_back(pt1);
		m_PointPositions.push_back(pt2);
		m_PointPositions.push_back(pt3);

		m_PointPositions.push_back(pt4);
		m_PointPositions.push_back(pt5);
		m_PointPositions.push_back(pt6);
		m_PointPositions.push_back(pt7);


		m_Indices = {
			//square 1
			0, 1, 
			1, 2,
			2, 3,
			3, 0,

			//square 2
			4, 5,
			5, 6,
			6, 7,
			7, 4,

			// join the 2 squares
			0, 4,
			1, 5,
			2, 6,
			3, 7
		};

		m_IndexBuffer = std::make_shared<IndexBuffer>((uint32_t*)m_Indices.data(), (uint32_t)m_Indices.size() * (uint32_t)sizeof(uint32_t));
	
		// m_VertexArray.reset(new VertexArray());
		m_VertexArray = std::make_shared<VertexArray>();
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);
		
		m_VertexBuffer = std::make_shared<VertexBuffer>((float*)m_PointPositions.data(), (uint32_t)m_PointPositions.size() * (uint32_t)sizeof(glm::vec3));

		m_VertexBuffer->setLayout(m_BufferLayout);
		m_VertexArray->addVertexBuffer(m_VertexBuffer);
	}


	void BoxLineObject::BindRenderData()
	{
		m_VertexArray->Bind();
		m_IndexBuffer->Bind();
	}

	void BoxLineObject::Render()
	{
		BindRenderData();
		glLineWidth(m_LineWidth);        
	   	glDrawElements(GL_LINES, (GLsizei)m_Indices.size(), GL_UNSIGNED_INT, nullptr);
	}
} /* end namespace GLR*/