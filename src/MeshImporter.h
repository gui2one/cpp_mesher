#pragma once

#include <assimp/Importer.hpp>

#include "Mesh.h"
#include "mesh_utils.h"
#include "openmesh/gmesh.h"

namespace msh {
class MeshImporter {
 public:
  Mesh Import(const char* path);
  GMesh GMeshImport(const char* path);

  static MeshImporter* s_Instance;
  static MeshImporter* GetInstance();

 private:
 protected:
  MeshImporter() = default;
};
}  // namespace msh