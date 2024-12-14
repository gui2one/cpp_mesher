#ifndef CPP_MESHER_OPENMESH_UTILS_H
#define CPP_MESHER_OPENMESH_UTILS_H
#pragma once

#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>

#include "gmesh.h"
namespace NED::openmeshutils {

const enum AXIS_ORDER { XYZ = 0, XZY = 1, YXZ = 2, YZX = 3, ZXY = 4, ZYX = 5 };

const enum TRANSFORM_ORDER { TRS = 0, TSR = 1, RTS = 2, RST = 3, STR = 4, SRT = 5 };

GMesh openmesh_cube();
GMesh openmesh_square();

void compute_normals(GMesh &mesh);
void set_normals(GMesh &mesh, glm::vec3 normal);

void triangulate(GMesh &mesh);
GMesh combine(GMesh &meshA, GMesh &meshB);

GMesh translate(GMesh &mesh, glm::vec3 offset);
GMesh scale(GMesh &mesh, glm::vec3 scale);
GMesh rotate(GMesh &mesh, glm::vec3 radians, AXIS_ORDER axis_order);

};  // namespace NED::openmeshutils
#endif  // CPP_MESHER_OPENMESH_UTILS_H
