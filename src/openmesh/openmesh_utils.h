#ifndef CPP_MESHER_OPENMESH_UTILS_H
#define CPP_MESHER_OPENMESH_UTILS_H
#pragma once

#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
#include "gmesh.h"
namespace NED::openmeshutils {

    GMesh openmesh_cube();
    GMesh openmesh_square();


};  // namespace NED::openmeshutils
#endif  // CPP_MESHER_OPENMESH_UTILS_H
