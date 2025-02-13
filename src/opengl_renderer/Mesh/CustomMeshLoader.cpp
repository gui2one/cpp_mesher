#include "CustomMeshLoader.h"

namespace GLR
{

	CustomMeshLoader* CustomMeshLoader::s_Instance = nullptr;

CustomMeshLoader* CustomMeshLoader::GetInstance(){

	if (s_Instance == nullptr) {
		s_Instance = new CustomMeshLoader();
	}

	return s_Instance;
}   

CustomMeshLoader::CustomMeshLoader()
{
	// LOG_INFO("CustomMeshLoader Singleton created.");

}



CustomMeshLoader::~CustomMeshLoader()
{

}
Mesh CustomMeshLoader::Load(std::filesystem::path path)
{

	LOG_INFO("Loading orbmesh File -> {}", path.string());

	std::fstream inFile(path, std::ios::in | std::ios::binary);

	// char * header_str = new char[7];
	BoundingBox bbox;
	uint32_t num_pts;
	uint32_t num_indices;
	std::vector<float> vertices_data;
	std::vector<uint32_t> indices;
	inFile.seekg(0);
	// inFile.read(header_str, 7);
	inFile.read((char*)&bbox, sizeof(BoundingBox));
	inFile.read((char*)&num_pts, sizeof(num_pts));
	inFile.read((char*)&num_indices, sizeof(num_indices));

	int vertex_size = 3 + 3 + 2;
	// vertices_data.reserve(num_pts * vertex_size);

	for(size_t i=0; i< num_pts * vertex_size; i++){
		float val;
		inFile.read((char*)&val, sizeof(val));
		vertices_data.push_back(val);
	}
	for(size_t i=0; i< num_indices; i++){
		uint32_t val;
		inFile.read((char*)&val, sizeof(val));
		indices.push_back(val);
	}    
	inFile.close();


	//// convert to mesh
	Mesh mesh;

	mesh.m_BoundingBox = bbox;

	mesh.m_BoundingBoxIsComputed = true;
	// LOG_INFO(mesh.m_BoundingBox);
	std::vector<Point> points;
	std::vector<Face> faces;

	int index = 0;

	
	for(size_t i=0; i< num_pts; i++){
		Point pt;
		float pos_x = vertices_data[0 + (i * 3)];
		float pos_y = vertices_data[1 + (i * 3)];
		float pos_z = vertices_data[2 + (i * 3)];

		int offset = num_pts * 3;
		float normal_x = vertices_data[0 + (i * 3) + offset];
		float normal_y = vertices_data[1 + (i * 3) + offset];
		float normal_z = vertices_data[2 + (i * 3) + offset];

		offset = num_pts * (3+3);
		float uv_x = vertices_data[0 + (i * 2) + offset];
		float uv_y = vertices_data[1 + (i * 2) + offset];
		pt.position = glm::vec3(pos_x, pos_y, pos_z);
		pt.normal = glm::vec3(normal_x, normal_y, normal_z);
		pt.t_coords = glm::vec2(uv_x, uv_y);

		points.push_back(pt);

	}

	for(size_t i=0; i<num_indices / 3; i++){
		uint32_t vtx1 = indices[0 + (i * 3)];
		uint32_t vtx2 = indices[2 + (i * 3)];
		uint32_t vtx3 = indices[1 + (i * 3)];

		Face face;
		face.SetVertices({Vertex(vtx1),Vertex(vtx2),Vertex(vtx3)});

		faces.push_back(face);
	}
	mesh.SetPoints(points);
	mesh.SetFaces(faces);

	return mesh;
}
} // namespace GLR
