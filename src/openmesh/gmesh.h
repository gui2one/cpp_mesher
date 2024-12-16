#ifndef CPP_MESHER_GMESH_H
#define CPP_MESHER_GMESH_H
#pragma once
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>

// A variant to represent different possible property types
using VertexPropertyVariant =
    std::variant<OpenMesh::VPropHandleT<int>, OpenMesh::VPropHandleT<float>, OpenMesh::VPropHandleT<std::string>,
                 OpenMesh::VPropHandleT<OpenMesh::Vec3f>>;

struct VertexProperty {
  VertexPropertyVariant handle;
  std::string name;
  std::string type;  // A string to represent the type name
};

class GMesh : public OpenMesh::PolyMesh_ArrayKernelT<> {
 public:
  GMesh() : OpenMesh::PolyMesh_ArrayKernelT<>() {}

 public:
  void add_dynamic_property(const std::string &name, const std::string &type) {
    if (type == "int") {
      OpenMesh::VPropHandleT<int> handle;
      add_property(handle, name);
      vertex_props.push_back({handle, name, type});
    } else if (type == "float") {
      OpenMesh::VPropHandleT<float> handle;
      add_property(handle, name);
      vertex_props.push_back({handle, name, type});
    } else if (type == "string") {
      OpenMesh::VPropHandleT<std::string> handle;
      add_property(handle, name);
      vertex_props.push_back({handle, name, type});
    } else if (type == "Vec3f") {
      OpenMesh::VPropHandleT<OpenMesh::Vec3f> handle;
      add_property(handle, name);
      vertex_props.push_back({handle, name, type});
    } else {
      std::cerr << "Unsupported property type: " << type << std::endl;
    }
  }

 public:
  // an array of custom polymesh properties
  std::vector<VertexProperty> vertex_props;
};

#endif  // CPP_MESHER_GMESH_H