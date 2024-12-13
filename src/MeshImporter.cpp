#include "MeshImporter.h"
namespace msh {

MeshImporter* MeshImporter::s_Instance = nullptr;

MeshImporter* MeshImporter::GetInstance() {
  if (s_Instance == nullptr) {
    s_Instance = new MeshImporter();
  }

  return s_Instance;
}

Mesh MeshImporter::Import(const char* path) {
  Assimp::Importer Importer;
  const aiScene* scene = Importer.ReadFile(path, aiProcess_Triangulate | aiProcess_ForceGenNormals);
  LOG_INFO("loading mesh file : {}", path);

  if (scene) {
    std::vector<Point> points;
    std::vector<Vertex> vertices;

    Mesh imported_mesh;
    for (size_t mesh_id = 0; mesh_id < scene->mNumMeshes; mesh_id++) {
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
          // point.normal *= -1.0f;
        }

        if (cur_mesh->HasTextureCoords(0)) {
          point.t_coords.x = cur_mesh->mTextureCoords[0][i].x;
          point.t_coords.y = cur_mesh->mTextureCoords[0][i].y;

        } else {
          point.t_coords.x = point.position.x;
          point.t_coords.y = point.position.z;
        }
        points.push_back(point);
        vertices.push_back(vtx);
      }

      std::vector<Face> faces;
      for (size_t i = 0; i < cur_mesh->mNumFaces; i++) {
        std::vector<uint32_t> vertices_index;

        for (size_t j = 0; j < cur_mesh->mFaces[i].mNumIndices; j++) {
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

GMesh MeshImporter::GMeshImport(const char* path) {
  Assimp::Importer Importer;
  const aiScene* scene = Importer.ReadFile(path, aiProcess_Triangulate | aiProcess_ForceGenNormals);
  LOG_INFO("loading mesh file (GMesh Format) : {}", path);

  if (scene) {
    GMesh imported_mesh;
    for (size_t mesh_id = 0; mesh_id < scene->mNumMeshes; mesh_id++) {
      std::vector<GMesh::Point> points;
      GMesh cur_gmesh;
      auto& cur_mesh = scene->mMeshes[mesh_id];
      for (size_t i = 0; i < cur_mesh->mNumVertices; i++) {
        GMesh::Point point;
        auto& mesh = cur_mesh;
        float x = mesh->mVertices[i].x;
        float y = mesh->mVertices[i].y;
        float z = mesh->mVertices[i].z;
        point[0] = x;
        point[1] = y;
        point[2] = z;
        // if (cur_mesh->HasNormals()) {
        //   point.normal.x = mesh->mNormals[i].x;
        //   point.normal.y = mesh->mNormals[i].y;
        //   point.normal.z = mesh->mNormals[i].z;

        //  // invert normal ....?!
        //  // point.normal *= -1.0f;
        //}

        // if (cur_mesh->HasTextureCoords(0)) {
        //   point.t_coords.x = cur_mesh->mTextureCoords[0][i].x;
        //   point.t_coords.y = cur_mesh->mTextureCoords[0][i].y;

        //} else {
        //  point.t_coords.x = point.position.x;
        //  point.t_coords.y = point.position.z;
        //}

        points.push_back(point);
      }

      for (const auto& pt : points) {
        cur_gmesh.add_vertex(pt);
      }
      std::vector<Face> faces;
      for (size_t i = 0; i < cur_mesh->mNumFaces; i++) {
        // std::vector<uint32_t> vertices_index;

        std::vector<GMesh::VertexHandle> vhs;
        for (size_t j = 0; j < cur_mesh->mFaces[i].mNumIndices; j++) {
          uint32_t index = cur_mesh->mFaces[i].mIndices[j];
          auto vh = cur_gmesh.vertex_handle(index);
          // LOG_INFO("vertex index : {}", index);
          if (cur_gmesh.is_valid_handle(vh)) {
            // LOG_INFO("valid vertex : id {}", index);
            vhs.push_back(vh);
          }
        }

        if (vhs.size() > 2) {
          // LOG_INFO("adding face !!!!!!");

          cur_gmesh.add_face(vhs.data(), vhs.size());
        }
        //  faces.push_back(Face(vertices_index));
      }
      LOG_INFO("cur gmesh : {}", cur_gmesh);
      // Mesh mesh;
      // mesh.SetPoints(points);
      // mesh.SetVertices(vertices);
      // mesh.SetFaces(faces);

      // points.clear();
      // faces.clear();
      auto before = GMesh(cur_gmesh);
      imported_mesh = NED::openmeshutils::combine(imported_mesh, cur_gmesh);
      LOG_INFO("combining mesh : \n\tbefore : {} \n\tafter : {}", before, imported_mesh);
    }
    return imported_mesh;
  }
  // LOG_ERROR("problem loading {0}", path);
  GMesh empty_mesh;
  return empty_mesh;
}

}  // namespace msh