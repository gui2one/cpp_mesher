#ifndef CPP_MESHER_GMESH_H
#define CPP_MESHER_GMESH_H
#pragma once
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>

class GMesh : public OpenMesh::PolyMesh_ArrayKernelT<> {
public:
    GMesh() : OpenMesh::PolyMesh_ArrayKernelT<>() {}
public : 
    std::vector<OpenMesh::VertexHandle> points;
};

#endif // CPP_MESHER_GMESH_H