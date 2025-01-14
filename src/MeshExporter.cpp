#include "MeshExporter.h"
namespace msh {
MeshExporter::MeshExporter() { m_AiExporter = std::make_shared<Assimp::Exporter>(); }

void MeshExporter::MakeScene(const Mesh& mesh) {
  m_Scene.mRootNode = new aiNode();

  m_Scene.mMaterials = new aiMaterial*[1];
  m_Scene.mMaterials[0] = nullptr;
  m_Scene.mNumMaterials = 1;

  m_Scene.mMaterials[0] = new aiMaterial();

  m_Scene.mMeshes = new aiMesh*[1];
  m_Scene.mMeshes[0] = nullptr;
  m_Scene.mNumMeshes = 1;

  m_Scene.mMeshes[0] = new aiMesh();
  m_Scene.mMeshes[0]->mMaterialIndex = 0;

  m_Scene.mRootNode->mMeshes = new unsigned int[1];
  m_Scene.mRootNode->mMeshes[0] = 0;
  m_Scene.mRootNode->mNumMeshes = 1;

  auto pMesh = m_Scene.mMeshes[0];

  std::vector<glm::vec3> vertices;
  std::vector<glm::vec3> normals;
  std::vector<glm::vec2> uvs;

  auto& points = mesh.GetPoints();
  for (size_t i = 0; i < points.size(); i++) {
    // vertices.push_back(data[draw_order[i]]);
    vertices.push_back(points[i].position);
    normals.push_back(points[i].normal);
    uvs.push_back(points[i].t_coords);
  }

  const auto& vVertices = vertices;

  pMesh->mVertices = new aiVector3D[vVertices.size()];
  pMesh->mNormals = new aiVector3D[vVertices.size()];
  pMesh->mNumVertices = int(vVertices.size());

  pMesh->mTextureCoords[0] = new aiVector3D[vVertices.size()];
  pMesh->mNumUVComponents[0] = int(vVertices.size());

  int j = 0;
  for (auto itr = vVertices.begin(); itr != vVertices.end(); ++itr) {
    pMesh->mVertices[itr - vVertices.begin()] = aiVector3D(vVertices[j].x, vVertices[j].y, vVertices[j].z);
    pMesh->mNormals[itr - vVertices.begin()] = aiVector3D(normals[j].x, normals[j].y, normals[j].z);
    pMesh->mTextureCoords[0][itr - vVertices.begin()] = aiVector3D(uvs[j].x, uvs[j].y, 0);
    j++;
  }

  auto& faces = mesh.GetFaces();
  pMesh->mFaces = new aiFace[faces.size()];
  pMesh->mNumFaces = (unsigned int)(faces.size());

  for (size_t i = 0; i < faces.size(); i++) {
    auto& vFace = mesh.GetFaces()[i];
    const int numIndices = int(faces[i].m_VerticesIndex.size());

    aiFace& face = pMesh->mFaces[i];
    face.mIndices = new unsigned int[numIndices];
    face.mNumIndices = numIndices;

    for (int j = 0; j < numIndices; j++) {
      face.mIndices[j] = faces[i].GetVertexIndex(j);
    }
  }
}

void MeshExporter::MakeScene(const GMesh& mesh) {
  if (mesh.n_vertices() == 0 || mesh.n_faces() == 0) return;

  m_Scene.mRootNode = new aiNode();

  m_Scene.mMaterials = new aiMaterial*[1];
  m_Scene.mMaterials[0] = nullptr;
  m_Scene.mNumMaterials = 1;

  m_Scene.mMaterials[0] = new aiMaterial();

  m_Scene.mMeshes = new aiMesh*[1];
  m_Scene.mMeshes[0] = nullptr;
  m_Scene.mNumMeshes = 1;

  m_Scene.mMeshes[0] = new aiMesh();
  m_Scene.mMeshes[0]->mMaterialIndex = 0;

  m_Scene.mRootNode->mMeshes = new unsigned int[1];
  m_Scene.mRootNode->mMeshes[0] = 0;
  m_Scene.mRootNode->mNumMeshes = 1;

  auto pMesh = m_Scene.mMeshes[0];

  std::vector<glm::vec3> vertices;
  std::vector<glm::vec3> normals;
  std::vector<glm::vec2> uvs;

  for (GMesh::VertexIter v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it) {
    auto pt = mesh.point(*v_it);
    vertices.push_back(glm::vec3(pt[0], pt[1], pt[2]));

    if (mesh.HasVertexProp("normal")) {
      auto ph = mesh.GetVertexProp("normal").prop;
      // auto vh = *v_it;
      auto value = mesh.GetVertexPropValue<OpenMesh::Vec3f>(ph, *v_it);
      normals.push_back(glm::vec3(value[0], value[1], value[2]));
    }
    if (mesh.HasVertexProp("uv")) {
      auto ph = mesh.GetVertexProp("uv").prop;
      // auto vh = *v_it;
      auto value = mesh.GetVertexPropValue<OpenMesh::Vec2f>(ph, *v_it);
      uvs.push_back(glm::vec2(value[0], value[1]));
    }
    // if (mesh.has_vertex_texcoords2D()) {
    //   // LOG_INFO("HAS VERTEX TEXCOORD2D");
    //   auto tex = mesh.texcoord2D(*v_it);
    //   uvs.push_back(glm::vec2(tex[0], tex[1]));
    // }
    if (mesh.has_vertex_colors()) {
      LOG_INFO("has colors ?");
    }
  }

  const auto& vVertices = vertices;

  pMesh->mVertices = new aiVector3D[vVertices.size()];
  pMesh->mNormals = new aiVector3D[vVertices.size()];
  pMesh->mNumVertices = int(vVertices.size());

  pMesh->mTextureCoords[0] = new aiVector3D[vVertices.size()];
  pMesh->mNumUVComponents[0] = int(vVertices.size());

  pMesh->mColors[0] = new aiColor4D[vVertices.size()];

  int j = 0;
  for (auto itr = vVertices.begin(); itr != vVertices.end(); ++itr) {
    pMesh->mVertices[itr - vVertices.begin()] = aiVector3D(vVertices[j].x, vVertices[j].y, vVertices[j].z);
    if (mesh.has_vertex_normals()) {
      pMesh->mNormals[itr - vVertices.begin()] = aiVector3D(normals[j].x, normals[j].y, normals[j].z);
    }
    if (mesh.HasVertexProp("uv")) {
      pMesh->mTextureCoords[0][itr - vVertices.begin()] = aiVector3D(uvs[j].x, uvs[j].y, 0);
    }

    auto clr = aiColor4D(0.5f, 1.0f, 0.5f, 1.0f);
    pMesh->mColors[0][itr - vVertices.begin()] = clr;

    // LOG_INFO("vertex color {} {} {} {}", clr.r, clr.g, clr.b, clr.a);

    j++;
  }

  const auto& faces = mesh.faces();
  pMesh->mFaces = new aiFace[mesh.n_faces()];
  pMesh->mNumFaces = (unsigned int)(mesh.n_faces());

  int inc = 0;
  for (GMesh::FaceIter f_it = mesh.faces_begin(); f_it != mesh.faces_end(); ++f_it) {
    const int numIndices = int(mesh.valence(*f_it));

    aiFace& face = pMesh->mFaces[inc];
    face.mIndices = new unsigned int[numIndices];
    face.mNumIndices = numIndices;

    j = 0;

    for (auto fv_it : mesh.fv_range(*f_it)) {
      face.mIndices[j] = fv_it.idx();
      j++;
    }

    inc++;
  }
}

void MeshExporter::ClearScene() { delete[] m_Scene.mMeshes; }

bool MeshExporter::Export(fs::path path) {
  std::string ext = path.extension().string();
  if (ext == ".ply") {
    return ExportPLY(path.string().c_str());
  } else if (ext == ".glb") {
    return ExportGLTF(path.string().c_str());
  } else if (ext == ".fbx") {
    return ExportFBX(path.string().c_str());
  }
  LOG_ERROR("EXPORT unknown extension {} ... nothing exported", ext);
  return false;
}
bool MeshExporter::ExportPLY(const char* path) {
  if (AI_SUCCESS == m_AiExporter->Export(&m_Scene, std::string("plyb"), std::string(path))) {
    LOG_INFO("Exported GMesh to {0}", path);
    return true;
  };

  LOG_ERROR("Error exporting {0}", path);
  return false;
}
bool MeshExporter::ExportGLTF(const char* path) {
  if (AI_SUCCESS == m_AiExporter->Export(&m_Scene, std::string("gltf2"), std::string(path))) {
    LOG_INFO("Exported GMesh to {0}", path);
    return true;
  };

  LOG_ERROR("Error exporting {0}", path);
  return false;
}

bool MeshExporter::ExportFBX(const char* path) {
  if (AI_SUCCESS == m_AiExporter->Export(&m_Scene, std::string("fbx"), std::string(path))) {
    LOG_INFO("Exported GMesh to {0}", path);
    return true;
  };

  LOG_ERROR("Error exporting {0}", path);
  return false;
}
}  // namespace msh