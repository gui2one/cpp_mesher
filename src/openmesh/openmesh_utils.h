#ifndef CPP_MESHER_OPENMESH_UTILS_H
#define CPP_MESHER_OPENMESH_UTILS_H
#pragma once

#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
namespace NED::openmeshutils {

    OpenMesh::PolyMesh_ArrayKernelT<> openmesh_cube();
    OpenMesh::PolyMesh_ArrayKernelT<> openmesh_square();


};  // namespace NED::openmeshutils
#endif  // CPP_MESHER_OPENMESH_UTILS_H
