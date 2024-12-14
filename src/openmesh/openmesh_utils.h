#ifndef CPP_MESHER_OPENMESH_UTILS_H
#define CPP_MESHER_OPENMESH_UTILS_H
#pragma once
#include <FastNoise/FastNoise.h>

#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>

#include "gmesh.h"
namespace NED::openmeshutils {

const enum AXIS_ORDER { XYZ = 0, XZY = 1, YXZ = 2, YZX = 3, ZXY = 4, ZYX = 5 };

const enum TRANSFORM_ORDER { TRS = 0, TSR = 1, RTS = 2, RST = 3, STR = 4, SRT = 5 };

GMesh openmesh_cube();
GMesh openmesh_square();
GMesh openmesh_grid(float width, float length, uint32_t _cols, uint32_t _rows);

void compute_normals(GMesh &mesh);
void set_normals(GMesh &mesh, glm::vec3 normal);

void triangulate(GMesh &mesh);
GMesh combine(GMesh &meshA, GMesh &meshB);

GMesh translate(GMesh &mesh, glm::vec3 offset);
GMesh scale(GMesh &mesh, glm::vec3 scale);
GMesh rotate(GMesh &mesh, glm::vec3 radians, AXIS_ORDER axis_order);

GMesh transform(GMesh &mesh, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale_, TRANSFORM_ORDER tr_order,
                AXIS_ORDER axis_order);

enum NoiseType { Simplex = 0, Perlin = 1, Cellular = 2 };
struct NoiseParamsStruct {
  NoiseType noise_type = NoiseType::Cellular;
  float lacunarity = 1.0f;
  float gain = 1.0f;
  float amplitude = 1.0f;
  float frequency = 1.0f;
  float weightedStrength = 1.0f;
  glm::vec3 offset = glm::vec3(0.0f, 0.0f, 0.0f);

  uint32_t seed = 0;
  uint32_t octaves = 1;
};
GMesh noise_displace(GMesh &mesh, NoiseParamsStruct noise_params);

};  // namespace NED::openmeshutils
#endif  // CPP_MESHER_OPENMESH_UTILS_H
