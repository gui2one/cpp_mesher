#ifndef MESH_H
#define MESH_H
#pragma once


#include <spdlog/fmt/ostr.h> 


namespace GLR 
{

	struct Point
	{
		Point();
		Point(float x, float y, float z);
		Point(glm::vec3 _position);
		Point(const Point& other) = default;

		glm::vec3 position;
		glm::vec3 normal;
		glm::vec3 color;
		glm::vec2 t_coords;
	};

	struct Vertex
	{
		Vertex() {}
		Vertex(uint32_t _point_id)
			: point_id(_point_id) 
		{	
		}

		Vertex(const Vertex& other)
			: point_id(other.point_id), normal(other.normal)
		{
		}

		inline void SetPointID(uint32_t _id)
		{
			point_id = _id;
		}

		uint32_t point_id;
		
		glm::vec3 normal;
		glm::vec3 color;
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
		}

		const inline uint32_t GetNumVertices() { return (uint32_t)m_Vertices.size(); } const
		
		inline Vertex& GetVertex(uint32_t _id) const { return m_Vertices[_id]; }
		
		inline void SetVertices(std::vector<Vertex> _vertices) { m_Vertices = _vertices; }
		
		inline std::vector<Vertex>& GetVertices() 
		{
			return m_Vertices;
		}

		std::vector<Vertex> m_Vertices;
	private:

	};

	struct BoundingBox
	{
		glm::vec3 min;
		glm::vec3 max;

		BoundingBox();
		BoundingBox(glm::vec3 min, glm::vec3 max);
		BoundingBox(const BoundingBox& other);
		
		glm::vec3 GetSize();
		glm::vec3 GetCenter();

		BoundingBox operator=(const BoundingBox& other);
		friend std::ostream& operator<<(std::ostream& os, const BoundingBox bbox);


	};

    inline std::ostream& operator<<(std::ostream& os, const BoundingBox bbox)
    {
        os << "\nbbox  : \npos -> " << glm::to_string(bbox.min)  << std::endl;
        os << "size -> " << glm::to_string(bbox.max - bbox.min)  << std::endl;
        return os;
    }	

	class Mesh
	{

	public:
		BoundingBox m_BoundingBox = {{0.f, 0.f, 0.f}, {0.f, 0.f, 0.f}};
		bool m_BoundingBoxIsComputed = false;
	public:
		Mesh();
		Mesh(const Mesh& other);
		Mesh operator=(const Mesh& other);
		virtual ~Mesh();

		void Triangulate();
		// void ComputeNormals();

		BoundingBox ComputeAABB();

		inline void Reset(){
			m_Points.clear();
			m_Faces.clear();
		}

		const std::vector<uint32_t> GetIndices () const;
		std::vector<uint32_t> GetIndices ();

		inline const std::vector<Point>& GetPoints() const { return m_Points; }
		inline std::vector<Point>& GetPoints() { return m_Points; }

		inline const std::vector<Face>& GetFaces() const { return m_Faces; }
		inline std::vector<Face>& GetFaces() { return m_Faces; }

		inline void SetPoints(std::vector<Point> pts) { m_Points = pts; }
		inline void SetFaces(std::vector<Face> faces) { m_Faces = faces; }

		void SetColor(glm::vec3 clr);
		
		friend std::ostream& operator<<(std::ostream& os, const Mesh& mesh);
		friend std::ostream& operator<<(std::ostream& os, const std::shared_ptr<Mesh> mesh_ptr);


	private:

		std::vector<Point> m_Points;
		std::vector<Face> m_Faces;
	};
	
	inline std::ostream& operator<<(std::ostream& os, const Mesh& mesh)
	{
		os << "This is the Mesh Information : " << std::endl;
		
		uint32_t pt_inc = 0;
		for(const auto& pt : mesh.GetPoints())
		{
			os << "Point : " << pt_inc << std::endl;
			os << "\tposition(" << std::fixed << std::setprecision( 2 ) << pt.position.x << ", " << pt.position.y << ", " << pt.position.z << ")";
			os << " normal(" << std::setprecision( 2 ) << pt.normal.x << ", " << pt.normal.y << ", " << pt.normal.z << ")";
			os << " t_coords(" << std::setprecision( 2 ) << pt.t_coords.x << ", " << pt.t_coords.y << ")" << std::endl ;

			pt_inc++;
		}

		uint32_t face_inc = 0;
		for(const auto& face : mesh.GetFaces())
		{
			os << "Face " << face_inc << std::endl;
			os << "Vertices : " << face.m_Vertices[0].point_id << "|" <<  face.m_Vertices[1].point_id << "|" << face.m_Vertices[2].point_id << std::endl;
		}

		return os;
	}

	inline std::ostream& operator<<(std::ostream& os,const std::shared_ptr<Mesh> mesh_ptr)
	{

		os << *(mesh_ptr.get()) << std::endl;

		return os;
	}	

}

#endif /* MESH_H */
