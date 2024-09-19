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
    const aiScene* scene = Importer.ReadFile(path, 0);
	LOG_INFO("loading mesh file : {}", path);
	
	if (scene) {

		std::vector<Point> points;


		for (size_t i = 0; i < scene->mMeshes[0]->mNumVertices; i++) {

			Point point;
			auto& mesh = scene->mMeshes[0];
			point.position.x = mesh->mVertices[i].x;
			point.position.y = mesh->mVertices[i].y;
			point.position.z = mesh->mVertices[i].z;
			
			if (scene->mMeshes[0]->HasNormals()) {
				point.normal.x = mesh->mNormals[i].x;
				point.normal.y = mesh->mNormals[i].y;
				point.normal.z = mesh->mNormals[i].z;

				// invert normal ....?!
				//point.normal *= -1.0f;
			}

			if (scene->mMeshes[0]->HasTextureCoords(0)) {


				point.t_coords.x = scene->mMeshes[0]->mTextureCoords[0][i].x;
				point.t_coords.y = scene->mMeshes[0]->mTextureCoords[0][i].y;

			}
			else {
				point.t_coords.x = point.position.x;
				point.t_coords.y = point.position.z;
			}
			points.push_back(point);


		}

		std::vector<Face> faces;
		for (size_t i = 0; i < scene->mMeshes[0]->mNumFaces; i++)
		{
			std::vector<Vertex> vertices;

			for (size_t j = 0; j < scene->mMeshes[0]->mFaces[i].mNumIndices; j++)
			{

				uint32_t index = scene->mMeshes[0]->mFaces[i].mIndices[j];

				vertices.push_back(Vertex(index));

			}

			faces.push_back(Face(vertices));
		}


		Mesh mesh;
		mesh.SetPoints(points);
		mesh.SetFaces(faces);

		points.clear();
		faces.clear();

		return mesh;
	}
	// LOG_ERROR("problem loading {0}", path);
	Mesh empty_mesh;
	return empty_mesh;

}

}