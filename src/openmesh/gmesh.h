#ifndef CPP_MESHER_GMESH_H
#define CPP_MESHER_GMESH_H
#pragma once
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>

class GMesh : public OpenMesh::PolyMesh_ArrayKernelT<> {
 public:
  GMesh() : OpenMesh::PolyMesh_ArrayKernelT<>() {}

 public:
  // Separate container for points
  std::vector<OpenMesh::VertexHandle> g_points;
  OpenMesh::PolyMesh_ArrayKernelT<> points_handler;

  // Add a point to your custom structure
  size_t add_point(OMesh::Point point) {
    // g_points.push_back(point);
    points_handler.add_vertex(point);
    return points_handler.n_vertices() - 1;  // Return the index of the added point
  }

  // Add a vertex using a point from your custom structure
  OpenMesh::VertexHandle add_vertex_from_point(size_t point_index) {
    if (point_index >= points_handler.n_vertices()) {
      throw std::out_of_range("Invalid point index for add_vertex_from_point()");
    }

    // Use the point from the custom structure to add a vertex
    return add_vertex(points_handler.points()[point_index]);
  }
};

#endif  // CPP_MESHER_GMESH_H