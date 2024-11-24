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

void MeshExporter::MakeScene(const OMesh& mesh) {
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
  // std::vector<glm::vec3> normals;
  // std::vector<glm::vec2> uvs;

  for (OMesh::VertexIter v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it) {
    auto pt = mesh.point(*v_it);
    // std::cout << "vertex : " << *v_it << std::endl;
    std::cout << "vertex " << *v_it << ": " << pt << std::endl;
    vertices.push_back(glm::vec3(pt[0], pt[1], pt[2]));
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
    // pMesh->mNormals[itr - vVertices.begin()] = aiVector3D(normals[j].x, normals[j].y, normals[j].z);
    // pMesh->mTextureCoords[0][itr - vVertices.begin()] = aiVector3D(uvs[j].x, uvs[j].y, 0);
    j++;
  }

  const auto& faces = mesh.faces();
  pMesh->mFaces = new aiFace[mesh.n_faces()];
  pMesh->mNumFaces = (unsigned int)(mesh.n_faces());

  int inc = 0;
  for (OMesh::FaceIter f_it = mesh.faces_begin(); f_it != mesh.faces_end(); ++f_it) {
    const int numIndices = int(mesh.valence(*f_it));

    aiFace& face = pMesh->mFaces[inc];
    face.mIndices = new unsigned int[numIndices];
    face.mNumIndices = numIndices;

    j = 0;
    std::cout << "face : " << *f_it << " : ";
    for (auto fv_it : mesh.fv_range(*f_it)) {
      std::cout << fv_it.idx() << " ";
      face.mIndices[j] = fv_it.idx();
      j++;
    }

    std::cout << std::endl;

    inc++;
  }
}

void MeshExporter::ClearScene() { delete[] m_Scene.mMeshes; }
bool MeshExporter::ExportPLY(const char* path) {
  if (AI_SUCCESS == m_AiExporter->Export(&m_Scene, std::string("plyb"), std::string(path))) {
    LOG_INFO("Successfully exported {0}", path);
    return true;
  };

  LOG_ERROR("Error exporting {0}", path);
  return false;
}
bool MeshExporter::ExportGLTF(const char* path) {
  // mExportFormatDesc->id is "collada"  and mFilePath is "C:/Users/kevin/Desktop/myColladaFile.dae"

  if (AI_SUCCESS == m_AiExporter->Export(&m_Scene, std::string("gltf2"), std::string(path))) {
    LOG_INFO("Successfully exported {0}", path);
    return true;
  };

  LOG_ERROR("Error exporting {0}", path);
  return false;
}

bool MeshExporter::ExportFBX(const char* path) {
  // mExportFormatDesc->id is "collada"  and mFilePath is "C:/Users/kevin/Desktop/myColladaFile.dae"

  if (AI_SUCCESS == m_AiExporter->Export(&m_Scene, std::string("fbx"), std::string(path))) {
    LOG_INFO("Successfully exported {0}", path);
    return true;
  };

  LOG_ERROR("Error exporting {0}", path);
  return false;
}
}  // namespace msh