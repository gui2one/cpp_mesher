#pragma once

// #include "Log.h"
#include <spdlog/fmt/ostr.h>
#include <fmt/format.h>
namespace msh {


	struct Point
	{
		Point();
		Point(float x, float y, float z);
		Point(glm::vec3 _position);
		Point(const Point& other);


		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 t_coords;
	};

	struct Vertex
	{

		Vertex();
		// Vertex(uint32_t _point_id);
		Vertex(const Vertex& other);

		inline void SetPointID(uint32_t _id)
		{
			point_id = _id;
		}



		uint32_t point_id;
		glm::vec3 normal;
		glm::vec2 t_coords;
	};

	class Face
	{
	public:
		Face() {}
		Face(std::vector<Vertex> _vertices) { m_Vertices = _vertices; } // TODO : this should be deleted at some point, using a vector<uint32_t> instead
		Face(std::vector<uint32_t> _verticesindex) { m_VerticesIndex = _verticesindex; }
		Face(const Face& other)
			:m_Vertices(other.m_Vertices), m_VerticesIndex(other.m_VerticesIndex)
		{
			
		}
		const inline size_t GetNumVertices() { return m_Vertices.size(); } const
		inline Vertex& GetVertex(uint32_t _id) const { return m_Vertices[_id]; }
		inline void SetVertices(std::vector<Vertex> _vertices) { m_Vertices = _vertices; }
		inline std::vector<Vertex>& GetVertices() {
			return m_Vertices;
		}

		const inline size_t GetNumVerticesIndex() { return m_VerticesIndex.size(); } const
		inline uint32_t GetVertexIndex(uint32_t _id) const { return m_VerticesIndex[_id]; }
		inline void SetVerticesIndex(std::vector<uint32_t> _verticesIndex) { m_VerticesIndex = _verticesIndex; }
		inline std::vector<uint32_t>& GetVerticesIndex() {
			return m_VerticesIndex;
		}

		inline void reverse(){
			std::reverse(m_Vertices.begin(), m_Vertices.end());
		}

		std::vector<Vertex> m_Vertices;
		std::vector<uint32_t> m_VerticesIndex;
	private:

	};

	struct BoundingBox
	{
		glm::vec3 position;
		glm::vec3 size;
	};

	class Mesh
	{
	public:
		Mesh();
		Mesh(const Mesh& other);
		virtual ~Mesh();

		void Triangulate();
		void ComputeNormals();

		BoundingBox ComputeAABB();

		// const std::vector<uint32_t> GetIndices () const;
		// std::vector<uint32_t> GetIndices ();

		inline const std::vector<Point>& GetPoints() const { return m_Points; }
		inline std::vector<Point>& GetPoints() { return m_Points; }

		inline const std::vector<Vertex>& GetVertices() const { return m_Vertices; }
		inline std::vector<Vertex>& GetVertices() { return m_Vertices; }

		inline const std::vector<Face>& GetFaces() const { return m_Faces; }
		inline std::vector<Face>& GetFaces() { return m_Faces; }

		inline void SetPoints(std::vector<Point> pts) { m_Points = pts; }
		inline void SetVertices(std::vector<Vertex> verts) { m_Vertices = verts; }
		inline void SetFaces(std::vector<Face> faces) { m_Faces = faces; }



		void PrintData();

		glm::vec3 translate;
		glm::vec3 rotate;
		glm::vec3 scale;


		friend std::ostream& operator<<(std::ostream& os, const Mesh& mesh);

	private:

		std::vector<Point> m_Points;
		std::vector<Vertex> m_Vertices;
		std::vector<Face> m_Faces;
	};

	


	
	inline std::ostream& operator<<(std::ostream& os, const Mesh& mesh){
		os << "msh::Mesh -> ";
		os << mesh.GetPoints().size() << " Point, ";
		os << mesh.GetVertices().size() << " Vertex, ";
		os << mesh.GetFaces().size()<< " Face";
		return os;
	}

}


