#ifndef CPP_MESHER_GMESH_H
#define CPP_MESHER_GMESH_H
#pragma once
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>

class GMesh : public OpenMesh::PolyMesh_ArrayKernelT<> {
public:
    GMesh() : OpenMesh::PolyMesh_ArrayKernelT<>() {}
public : 
    // Separate container for points
    std::vector<OpenMesh::PolyMesh_ArrayKernelT<>::Point> g_points;

    // Add a point to your custom structure
    size_t add_point(const OpenMesh::PolyMesh_ArrayKernelT<>::Point& point) {
        g_points.push_back(point);
        return g_points.size() - 1;  // Return the index of the added point
    }

    // Add a vertex using a point from your custom structure
    OpenMesh::VertexHandle add_vertex_from_point(size_t point_index) {
        if (point_index >= g_points.size()) {
            throw std::out_of_range("Invalid point index");
        }

        // Use the point from the custom structure to add a vertex
        return OpenMesh::PolyMesh_ArrayKernelT<>::add_vertex(g_points[point_index]);
    }

    inline void set_texcoord2D_glm(OpenMesh::VertexHandle _vh, glm::vec2 coords){
        this->set_texcoord2D(_vh, OpenMesh::Vec2f(coords.x, coords.y));
    }
};

#endif // CPP_MESHER_GMESH_H