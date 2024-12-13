#pragma once
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
#include <assimp/Exporter.hpp>

#include "Mesh.h"
#include "assimp/cexport.h"
#include "assimp/scene.h"
#include "openmesh/gmesh.h"

namespace msh {
class MeshExporter {
 public:
  MeshExporter();
  void MakeScene(const Mesh& mesh);
  void MakeScene(const GMesh& mesh);
  void ClearScene();

  bool Export(fs::path path);
  bool ExportPLY(const char* path);
  inline bool ExportPLY(fs::path path) { return ExportPLY(path.string().c_str()); }
  bool ExportGLTF(const char* path);
  bool ExportFBX(const char* path);

 private:
  std::shared_ptr<Assimp::Exporter> m_AiExporter;
  aiScene m_Scene;
};
}  // namespace msh