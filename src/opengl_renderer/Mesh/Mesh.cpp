#include "Mesh.h"
namespace GLR {

	/* Point */

	Point::Point()
		: position(glm::vec3(0.f)), 
		normal(glm::vec3(0.0f, 1.0f, 0.0f)),
		color(glm::vec3(1.0f))

	{

	}

	Point::Point(float x, float y, float z)
	{
		position.x = x;
		position.y = y;
		position.z = z;
		color = glm::vec3(1.0f);
	}

	Point::Point(glm::vec3 _position)
	{
		position = _position; 
		color = glm::vec3(1.0f);
	}

	/* 	BoundingBox */

    BoundingBox::BoundingBox()
		: min(glm::vec3(0.f)), max(glm::vec3(0.f))
    {

    }

    BoundingBox::BoundingBox(glm::vec3 _min, glm::vec3 _max)
    {
		min = _min;
		max = _max;
    }

    BoundingBox::BoundingBox(const BoundingBox& other){
		min = other.min;
		max = other.max;
	}

    glm::vec3 BoundingBox::GetSize()
    {
        return glm::vec3(max.x - min.x, max.y - min.y, max.z - min.z);
    }

    glm::vec3 BoundingBox::GetCenter()
    {
		glm::vec3 center;
		center = (min + max) / 2.0f;
        return center;
    }

    BoundingBox BoundingBox::operator=(const BoundingBox &other)
    {
		min = other.min;
		max = other.max;

		return *this;	
    }

	/* Mesh */

    Mesh::Mesh() : m_BoundingBoxIsComputed(false)
	{

	}

	Mesh::Mesh(const Mesh& other)
	{
		m_Points = other.m_Points;
		m_Faces = other.m_Faces;
		m_BoundingBox = other.m_BoundingBox;
		m_BoundingBoxIsComputed = other.m_BoundingBoxIsComputed;
	}

    Mesh Mesh::operator=(const Mesh &other)
    {
		m_Points = other.m_Points;
		m_Faces = other.m_Faces;
		m_BoundingBox = other.m_BoundingBox;
		m_BoundingBoxIsComputed = other.m_BoundingBoxIsComputed;

		return *this;	
    }

    Mesh::~Mesh()
	{

	}

	void Mesh::Triangulate()
	{
		Mesh tri_mesh;
		std::vector<Face> new_faces;

		for (size_t i = 0; i < m_Faces.size(); i++)
		{
			uint32_t num_verts = uint32_t(m_Faces[i].GetNumVertices());
			uint32_t num_tris = num_verts - 3 + 1;

			for (size_t tri = 0; tri < num_tris; tri++)
			{
				Face face;
				face.SetVertices(
					{
						Vertex(m_Faces[i].GetVertex(0).point_id),
						Vertex(m_Faces[i].GetVertex(uint32_t(tri) + 1).point_id),
						Vertex(m_Faces[i].GetVertex(uint32_t(tri) + 2).point_id)
					}
				);

				new_faces.push_back(face);
			}
		}

		m_Faces = new_faces;

	}

	const std::vector<uint32_t> Mesh::GetIndices() const
	{
		std::vector<uint32_t> indices;
		for (auto& face : m_Faces) 
		{
			for (auto& vertex : face.m_Vertices) 
			{
				indices.push_back(vertex.point_id);

			}
		}
		return indices;
	}

	std::vector<uint32_t> Mesh::GetIndices()
	{
		std::vector<uint32_t> indices;
		for (auto& face : m_Faces) 
		{
			for (auto& vertex : face.m_Vertices)
			{
				indices.push_back(vertex.point_id);
			}
		}
		return indices;
	}

    void Mesh::SetColor(glm::vec3 clr)
    {
		for(auto& pt : GetPoints())
		{
			pt.color = clr;
		}
    }

	BoundingBox Mesh::ComputeAABB()
	{
		BoundingBox bbox;
		float minx = 10000000.0f;
		float miny = 10000000.0f;
		float minz = 10000000.0f;

		float maxx = -10000000.0f;
		float maxy = -10000000.0f;
		float maxz = -10000000.0f;

		for (size_t i = 0; i < m_Points.size(); i++)
		{

			if (m_Points[i].position.x < minx) {
				minx = m_Points[i].position.x;
			}
			else if (m_Points[i].position.x > maxx) {
				maxx = m_Points[i].position.x;
			}

			if (m_Points[i].position.y < miny) {
				miny = m_Points[i].position.y;
			}
			else if (m_Points[i].position.y > maxy) {
				maxy = m_Points[i].position.y;
			}

			if (m_Points[i].position.z < minz) {
				minz = m_Points[i].position.z;
			}
			else if (m_Points[i].position.z > maxz) {
				maxz = m_Points[i].position.z;
			}
		}

		bbox.min = glm::vec3(minx, miny, minz);
		bbox.max = glm::vec3(maxx, maxy, maxz);

		m_BoundingBox = bbox;
		return bbox;

	}

}