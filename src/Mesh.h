#pragma once
namespace msh {


	struct Point
	{
		Point() {}
		Point(float x, float y, float z) {
			position.x = x;
			position.y = y;
			position.z = z;
		}
		Point(glm::vec3 _position) { position = _position; }
		Point(const Point& other) : position(other.position), normal(other.normal), t_coords(other.t_coords)
		{

		}


		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 t_coords;
	};

	struct Vertex
	{

		Vertex() {}
		Vertex(uint32_t _point_id) {
			point_id = _point_id;
		}

		Vertex(const Vertex& other)
			:point_id(other.point_id), normal(other.normal)
		{

		}

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
		Face(std::vector<Vertex> _vertices) { m_Vertices = _vertices; }
		Face(const Face& other)
			:m_Vertices(other.m_Vertices)
		{
			//~ printf("copy Face !!!!\n");
		}

		const inline size_t GetNumVertices() { return m_Vertices.size(); } const
		inline Vertex& GetVertex(uint32_t _id) const { return m_Vertices[_id]; }
		inline void SetVertices(std::vector<Vertex> _vertices) { m_Vertices = _vertices; }
		inline std::vector<Vertex>& GetVertices() {
			return m_Vertices;
		}

		std::vector<Vertex> m_Vertices;
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

		const std::vector<uint32_t> GetIndices () const;
		std::vector<uint32_t> GetIndices ();
		inline const std::vector<Point>& GetPoints() const { return m_Points; }
		inline std::vector<Point>& GetPoints() { return m_Points; }

		inline const std::vector<Face>& GetFaces() const { return m_Faces; }
		inline std::vector<Face>& GetFaces() { return m_Faces; }

		inline void SetPoints(std::vector<Point> pts) { m_Points = pts; }
		inline void SetFaces(std::vector<Face> faces) { m_Faces = faces; }

		void PrintData();

		glm::vec3 translate;
		glm::vec3 rotate;
		glm::vec3 scale;


		friend std::ostream& operator<<(std::ostream& os, const msh::Mesh& mesh);

	private:

		/* add your private declarations */
		std::vector<Point> m_Points;
		std::vector<Face> m_Faces;
	};

}

