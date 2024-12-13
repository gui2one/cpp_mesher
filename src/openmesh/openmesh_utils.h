#ifndef CPP_MESHER_OPENMESH_UTILS_H
#define CPP_MESHER_OPENMESH_UTILS_H
#pragma once

#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>

#include "gmesh.h"
namespace NED::openmeshutils {

GMesh openmesh_cube();
GMesh openmesh_square();

void compute_normals(GMesh &mesh);
void set_normals(GMesh &mesh, glm::vec3 normal);

void triangulate(GMesh &mesh);
GMesh combine(GMesh &meshA, GMesh &meshB);

};  // namespace NED::openmeshutils
#endif  // CPP_MESHER_OPENMESH_UTILS_H
