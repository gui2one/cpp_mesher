#ifndef CPP_MESHER_GMESH_H
#define CPP_MESHER_GMESH_H
#pragma once
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>

template <typename T>
using VertexPropH = OpenMesh::VPropHandleT<T>;

using VertexPropertyHVariant =
    std::variant<VertexPropH<float>, VertexPropH<int>, VertexPropH<std::string>, VertexPropH<OpenMesh::Vec3f>>;

class GMesh : public OpenMesh::PolyMesh_ArrayKernelT<> {
 public:
  GMesh() : OpenMesh::PolyMesh_ArrayKernelT<>() {}

 public:
  // an array of custom polymesh properties

  std::vector<VertexPropertyHVariant> vertex_prop_handles;
};

#endif  // CPP_MESHER_GMESH_H