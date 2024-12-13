#ifndef CPP_MESHER_GMESH_H
#define CPP_MESHER_GMESH_H
#pragma once
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>

template <typename T>
using VertexPropH = OpenMesh::VPropHandleT<T>;

class GMesh : public OpenMesh::PolyMesh_ArrayKernelT<> {
 public:
  GMesh() : OpenMesh::PolyMesh_ArrayKernelT<>() {}

 public:
  // an array of custom polymesh properties

  std::vector<VertexPropH<OpenMesh::Vec3f>> vertex_props;
};

#endif  // CPP_MESHER_GMESH_H