#include "MeshImporter.h"
namespace msh {

MeshImporter* MeshImporter::s_Instance = nullptr;

MeshImporter* MeshImporter::GetInstance() {
	if (s_Instance == nullptr) {
		s_Instance = new MeshImporter();
	}

	return s_Instance;
}
// MeshImporter::MeshImporter()
// {
// 	//LOG_INFO("MeshImporter Singleton created.");
// }

Mesh MeshImporter::Import(const char *path)
{

	Assimp::Importer Importer;
    const aiScene* scene = Importer.ReadFile(path, aiProcess_Triangulate | aiProcess_ForceGenNormals);
	LOG_INFO("loading mesh file : {}", path);
	
	if (scene) {

		std::vector<Point> points;
		std::vector<Vertex> vertices;

		Mesh imported_mesh;
		for(size_t mesh_id=0; mesh_id < scene->mNumMeshes; mesh_id++){
			auto& cur_mesh = scene->mMeshes[mesh_id];
			for (size_t i = 0; i < cur_mesh->mNumVertices; i++) {

				Point point;
				Vertex vtx;
				vtx.point_id = (uint32_t)i;
				auto& mesh = cur_mesh;
				point.position.x = mesh->mVertices[i].x;
				point.position.y = mesh->mVertices[i].y;
				point.position.z = mesh->mVertices[i].z;
				
				if (cur_mesh->HasNormals()) {
					point.normal.x = mesh->mNormals[i].x;
					point.normal.y = mesh->mNormals[i].y;
					point.normal.z = mesh->mNormals[i].z;

					// invert normal ....?!
					//point.normal *= -1.0f;
				}

				if (cur_mesh->HasTextureCoords(0)) {


					point.t_coords.x = cur_mesh->mTextureCoords[0][i].x;
					point.t_coords.y = cur_mesh->mTextureCoords[0][i].y;

				}
				else {
					point.t_coords.x = point.position.x;
					point.t_coords.y = point.position.z;
				}
				points.push_back(point);
				vertices.push_back(vtx);


			}

			std::vector<Face> faces;
			for (size_t i = 0; i < cur_mesh->mNumFaces; i++)
			{
				std::vector<uint32_t> vertices_index;

				for (size_t j = 0; j < cur_mesh->mFaces[i].mNumIndices; j++)
				{

					uint32_t index = cur_mesh->mFaces[i].mIndices[j];
					
					vertices_index.push_back(index);

				}

				faces.push_back(Face(vertices_index));
			}


			Mesh mesh;
			mesh.SetPoints(points);
			mesh.SetVertices(vertices);
			mesh.SetFaces(faces);

			points.clear();
			faces.clear();

			imported_mesh = meshutils::merge(imported_mesh, mesh);
		}
		return imported_mesh;
	}
	// LOG_ERROR("problem loading {0}", path);
	Mesh empty_mesh;
	return empty_mesh;

}

}