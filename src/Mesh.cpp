#include "Mesh.h"
namespace msh {
	
	Point::Point(): position(0.0f, 0.0f, 0.0f), normal(1.0f, 0.0f, 0.0f), t_coords(0.0f, 0.0f){}
	Point::Point(float x, float y, float z) :position(x, y, z), normal(1.0f, 0.0f, 0.0f), t_coords(0.0f, 0.0f){}
	Point::Point(glm::vec3 _position) : position(_position), normal(1.0f, 0.0f, 0.0f), t_coords(0.0f, 0.0f){}
	Point::Point(const Point& other) : position(other.position), normal(other.normal), t_coords(other.t_coords){}


	Vertex::Vertex(): normal(1.0f, 1.0f, 0.0f), t_coords(0.0f, 0.0f) {}
	// Vertex::Vertex(uint32_t _point_id): point_id(_point_id), normal(1.0f, 1.0f, 0.0f), t_coords(0.0f, 0.0f){}
	Vertex::Vertex(const Vertex& other) :point_id(other.point_id), normal(other.normal), t_coords(other.t_coords){}

	Mesh::Mesh() : translate(glm::vec3(0.0, 0.0, 0.0)), rotate(glm::vec3(0.0, 0.0, 0.0)), scale(glm::vec3(1.0, 1.0, 1.0))
	{

	}

	Mesh::Mesh(const Mesh& other) : translate(other.translate), rotate(other.rotate), scale(other.scale)
	{
		m_Points = other.m_Points;
		m_Vertices = other.m_Vertices;
		m_Faces = other.m_Faces;

	}


	Mesh::~Mesh()
	{

	}


	void Mesh::Triangulate()
	{
		LOG_WARN("Mesh::Triangulate not implemented");
		// Mesh tri_mesh;
		// std::vector<Face> new_faces;

		// for (size_t i = 0; i < m_Faces.size(); i++)
		// {
		// 	uint32_t num_verts = uint32_t(m_Faces[i].GetNumVertices());



		// 	uint32_t num_tris = num_verts - 3 + 1;

		// 	for (size_t tri = 0; tri < num_tris; tri++)
		// 	{

		// 		Face face;
		// 		face.SetVertices(
		// 			{
		// 				Vertex(m_Faces[i].GetVertex(0).point_id),
		// 				Vertex(m_Faces[i].GetVertex(uint32_t(tri) + 1).point_id),
		// 				Vertex(m_Faces[i].GetVertex(uint32_t(tri) + 2).point_id)
		// 			}
		// 		);

		// 		new_faces.push_back(face);

		// 	}


		// }


		// m_Faces = new_faces;

	}

	// const std::vector<uint32_t> Mesh::GetIndices() const
	// {
	// 	std::vector<uint32_t> indices;
	// 	for (auto& face : m_Faces) {
	// 		for (auto& vertex : face.m_Vertices) {

	// 			indices.push_back(vertex.point_id);

	// 		}
	// 	}
	// 	return indices;
	// }

	// std::vector<uint32_t> Mesh::GetIndices()
	// {
	// 	std::vector<uint32_t> indices;
	// 	for (auto& face : m_Faces) {
	// 		for (auto& vertex : face.m_Vertices) {
		
	// 			indices.push_back(vertex.point_id);

	// 		}
	// 	}
	// 	return indices;
	// }


	// void Mesh::PrintData()
	// {
	// 	printf("Mesh Data ----------------\n");

	// 	printf("\tPoints: \n");
	// 	for (uint32_t i = 0; i < m_Points.size(); i++)
	// 	{
	// 		printf("\t\t%I32d -- %.2f %.2f %.2f\n", i, m_Points[i].position.x, m_Points[i].position.y, m_Points[i].position.z);
	// 	}

	// 	printf("\n\tFaces:\n");

	// 	for (size_t face_id = 0; face_id < m_Faces.size(); face_id++)
	// 	{

	// 		uint32_t num_verts = uint32_t( m_Faces[face_id].GetNumVerticesIndex());
	// 		for (uint32_t i = 0; i < num_verts; i++)
	// 		{

	// 			printf(" %I32d", m_Faces[face_id].GetVertexIndex(i));
	// 			printf("  t_coords --> U: %.2f -- V: %.2f\n", m_Faces[face_id].GetVertex(i).t_coords.x, m_Faces[face_id].GetVertex(i).t_coords.y);
	// 		}


	// 		printf("  normal --> X: %.2f Y: %.2f Z: %.2f\n", m_Faces[face_id].GetVertex(0).normal.x, m_Faces[face_id].GetVertex(0).normal.y, m_Faces[face_id].GetVertex(0).normal.z);

	// 	}


	// }

	void Mesh::ComputeNormals()
	{

		LOG_WARN("Mesh::ComputeNormals Being implemented right now !!");

		std::vector<std::vector<glm::vec3>> point_normals(m_Points.size());
		for (size_t i = 0; i < m_Faces.size(); i++){
			
			Face& face = m_Faces[i];
			uint32_t num_verts = face.GetNumVerticesIndex();
			LOG_INFO("Face {0} nvers {1}", i, num_verts);
			if(num_verts > 2){
				uint32_t id0 = GetVertices()[face.GetVertexIndex(0)].point_id;
				uint32_t id1 = GetVertices()[face.GetVertexIndex(1)].point_id;
				uint32_t id2 = GetVertices()[face.GetVertexIndex(2)].point_id;
				Point p0 = m_Points[id0];
				Point p1 = m_Points[id1];
				Point p2 = m_Points[id2];
				glm::vec3 normal = glm::normalize(glm::cross(p1.position - p0.position, p2.position - p0.position));

				point_normals[id0].push_back(normal);
				point_normals[id1].push_back(normal);
				point_normals[id2].push_back(normal);
			}
			

		}

		// use point_normals list to compute the average normal for each point
		for (size_t i = 0; i < m_Points.size(); i++){
			m_Points[i].normal = glm::vec3(0.0f, 0.0f, 0.0f);
			for (size_t j = 0; j < point_normals[i].size(); j++){
				m_Points[i].normal += point_normals[i][j];
			}
			m_Points[i].normal = glm::normalize(m_Points[i].normal);
		}

	}


	BoundingBox Mesh::ComputeAABB()
	{
		BoundingBox bbox;
		float minx = 10000000.0;
		float miny = 10000000.0;
		float minz = 10000000.0;

		float maxx = -10000000.0;
		float maxy = -10000000.0;
		float maxz = -10000000.0;

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

		bbox.position = glm::vec3(minx, miny, minz);
		bbox.size = glm::vec3(maxx - minx, maxy - miny, maxz - minz);

		return bbox;

	}


}

