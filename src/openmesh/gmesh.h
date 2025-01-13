#ifndef CPP_MESHER_GMESH_H
#define CPP_MESHER_GMESH_H
#pragma once
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>

// A variant to represent different possible property types
using VertexPropertyVariant =
    std::variant<OpenMesh::VPropHandleT<int>, OpenMesh::VPropHandleT<float>, OpenMesh::VPropHandleT<std::string>,
                 OpenMesh::VPropHandleT<OpenMesh::Vec2f>, OpenMesh::VPropHandleT<OpenMesh::Vec3f>>;

enum PropertyType { PROP_NONE, PROP_INT, PROP_FLOAT, PROP_STRING, PROP_VEC3F, PROP_VEC2F };

struct VertexProperty {
  VertexPropertyVariant handle;
  std::string name;
  PropertyType type;  // A string to represent the type name
  const char* type_name;

  static VertexProperty None() { return {OpenMesh::VPropHandleT<int>(0), "none", PropertyType::PROP_NONE, "none"}; }
};
struct VertexPropResult {
  bool success;
  VertexProperty prop;
};
class GMesh : public OpenMesh::PolyMesh_ArrayKernelT<> {
 public:
  GMesh() : OpenMesh::PolyMesh_ArrayKernelT<>() {}

 public:
  VertexPropertyVariant add_dynamic_property(const std::string& name, PropertyType type) {
    if (HasVertexProp(name)) {
      return GetVertexProp(name).prop.handle;
    }
    if (type == PropertyType::PROP_INT) {
      OpenMesh::VPropHandleT<int> handle;
      add_property(handle, name);
      vertex_props.push_back({handle, name, type, "int"});
      for (auto v_it = vertices_begin(); v_it != vertices_end(); ++v_it) {
        property(handle, *v_it) = 0;
      }
      return handle;
    } else if (type == PropertyType::PROP_FLOAT) {
      OpenMesh::VPropHandleT<float> handle;
      add_property(handle, name);
      vertex_props.push_back({handle, name, type, "float"});
      for (auto v_it = vertices_begin(); v_it != vertices_end(); ++v_it) {
        property(handle, *v_it) = 0.0f;
      }
      return handle;
    } else if (type == PropertyType::PROP_STRING) {
      OpenMesh::VPropHandleT<std::string> handle;
      add_property(handle, name);
      vertex_props.push_back({handle, name, type, "string"});
      for (auto v_it = vertices_begin(); v_it != vertices_end(); ++v_it) {
        property(handle, *v_it) = "";
      }
      return handle;
    } else if (type == PropertyType::PROP_VEC2F) {
      OpenMesh::VPropHandleT<OpenMesh::Vec2f> handle;
      add_property(handle, name);
      vertex_props.push_back({handle, name, type, "vec2f"});
      for (auto v_it = vertices_begin(); v_it != vertices_end(); ++v_it) {
        property(handle, *v_it) = {0.0f, 0.0f};
      }
      return handle;
    } else if (type == PropertyType::PROP_VEC3F) {
      OpenMesh::VPropHandleT<OpenMesh::Vec3f> handle;
      add_property(handle, name);
      vertex_props.push_back({handle, name, type, "vec3f"});
      for (auto v_it = vertices_begin(); v_it != vertices_end(); ++v_it) {
        property(handle, *v_it) = {0.0f, 0.0f, 0.0f};
      }
      return handle;
    } else {
      std::cerr << "Unsupported property type: " << type << std::endl;
      auto handle = VertexProperty::None().handle;
      return handle;
    }
  }

  inline VertexPropResult GetVertexProp(std::string prop_name) const {
    for (auto& prop : vertex_props) {
      if (prop.name == prop_name) {
        return {true, prop};
      }
    }

    auto none_prop = VertexProperty::None();
    return {false, none_prop};
  }

  bool HasVertexProp(std::string prop_name) const { return GetVertexProp(prop_name).success; }

  template <typename T>
  T GetVertexPropValue(VertexProperty prop, GMesh::VertexHandle vh) const {
    // auto prop = GetVertexProp(prop_name).prop;

    return property(std::get<OpenMesh::VPropHandleT<T>>(prop.handle), vh);
  }

  template <typename T>
  void SetVertexPropValue(VertexProperty prop, GMesh::VertexHandle vh, T value) {
    auto ph = std::get<OpenMesh::VPropHandleT<T>>(prop.handle);
    property(ph, vh) = value;
  }

 public:
  // an array of custom polymesh properties
  std::vector<VertexProperty> vertex_props;
};

#endif  // CPP_MESHER_GMESH_H