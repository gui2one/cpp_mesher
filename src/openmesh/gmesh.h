#ifndef CPP_MESHER_GMESH_H
#define CPP_MESHER_GMESH_H
#pragma once
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>

// A variant to represent different possible property types
using VertexPropertyVariant =
    std::variant<OpenMesh::VPropHandleT<int>, OpenMesh::VPropHandleT<float>, OpenMesh::VPropHandleT<std::string>,
                 OpenMesh::VPropHandleT<OpenMesh::Vec3f>>;

enum PropertyType { PROP_INT, PROP_FLOAT, PROP_STRING, PROP_VEC3F };

struct VertexProperty {
  VertexPropertyVariant handle;
  std::string name;
  PropertyType type;  // A string to represent the type name
  const char* type_name;
};

class GMesh : public OpenMesh::PolyMesh_ArrayKernelT<> {
 public:
  GMesh() : OpenMesh::PolyMesh_ArrayKernelT<>() {}

 public:
  VertexPropertyVariant add_dynamic_property(const std::string& name, PropertyType type) {
    if (type == PropertyType::PROP_INT) {
      OpenMesh::VPropHandleT<int> handle;
      add_property(handle, name);
      vertex_props.push_back({handle, name, type, "int"});
      return handle;
    } else if (type == PropertyType::PROP_FLOAT) {
      OpenMesh::VPropHandleT<float> handle;
      add_property(handle, name);
      vertex_props.push_back({handle, name, type, "float"});
      return handle;
    } else if (type == PropertyType::PROP_STRING) {
      OpenMesh::VPropHandleT<std::string> handle;
      add_property(handle, name);
      vertex_props.push_back({handle, name, type, "string"});
      return handle;
    } else if (type == PropertyType::PROP_VEC3F) {
      OpenMesh::VPropHandleT<OpenMesh::Vec3f> handle;
      add_property(handle, name);
      vertex_props.push_back({handle, name, type, "vec3f"});
      return handle;
    } else {
      std::cerr << "Unsupported property type: " << type << std::endl;
    }
  }

 public:
  // an array of custom polymesh properties
  std::vector<VertexProperty> vertex_props;
};

#endif  // CPP_MESHER_GMESH_H