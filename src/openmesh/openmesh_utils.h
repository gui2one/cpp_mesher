#ifndef CPP_MESHER_OPENMESH_UTILS_H
#define CPP_MESHER_OPENMESH_UTILS_H
#pragma once
#include <FastNoise/FastNoise.h>
#include <corecrt_math_defines.h>
#include <opensubdiv/far/primvarRefiner.h>
#include <opensubdiv/far/stencilTableFactory.h>
#include <opensubdiv/far/topologyDescriptor.h>
#include <opensubdiv/osd/cpuEvaluator.h>
#include <opensubdiv/osd/cpuVertexBuffer.h>

#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/Utils/PropertyManager.hh>

#include "gmesh.h"
namespace NED::openmeshutils {

const enum AXIS_ORDER { XYZ = 0, XZY = 1, YXZ = 2, YZX = 3, ZXY = 4, ZYX = 5 };

const enum TRANSFORM_ORDER { TRS = 0, TSR = 1, RTS = 2, RST = 3, STR = 4, SRT = 5 };

GMesh openmesh_cube();
GMesh openmesh_square(bool compute_uvs = true);
GMesh openmesh_grid(float width, float length, uint32_t _cols, uint32_t _rows, bool compute_uvs = true);
GMesh openmesh_torus(float radius1, float radius2, uint32_t _cols, uint32_t _rows);

void compute_normals(GMesh &mesh, bool reverse = false);
void set_normals(GMesh &mesh, glm::vec3 normal);

GMesh triangulate(GMesh &mesh);
GMesh combine(GMesh &meshA, GMesh &meshB);

struct osd_Point3 {
  // Minimal required interface ----------------------
  osd_Point3() {}

  void Clear(void * = 0) { _point[0] = _point[1] = _point[2] = 0.0f; }

  void AddWithWeight(osd_Point3 const &src, float weight) {
    _point[0] += weight * src._point[0];
    _point[1] += weight * src._point[1];
    _point[2] += weight * src._point[2];
  }

  // Public interface ------------------------------------
  void SetPoint(float x, float y, float z) {
    _point[0] = x;
    _point[1] = y;
    _point[2] = z;
  }

  const float *GetPoint() const { return _point; }

 private:
  float _point[3];
};

struct osd_Point2 {
  // Minimal required interface ----------------------
  osd_Point2() {}

  void Clear(void * = 0) { _point[0] = _point[1] = 0.0f; }

  void AddWithWeight(osd_Point2 const &src, float weight) {
    _point[0] += weight * src._point[0];
    _point[1] += weight * src._point[1];
  }

  // Public interface ------------------------------------
  void SetPoint(float x, float y) {
    _point[0] = x;
    _point[1] = y;
  }

  const float *GetPoint() const { return _point; }

 private:
  float _point[2];
};

struct LinearFloatAttribute {
  std::string name;
  std::vector<float> values;
};
struct LinearVec2Attribute {
  std::string name;
  std::vector<osd_Point2> values;
};
struct LinearVec3Attribute {
  std::string name;
  std::vector<osd_Point3> values;
};

struct osd_DATA {
  std::vector<osd_Point3> positions;

  int nverts;
  int nfaces;
  std::vector<int> vertsperface;
  std::vector<int> vertIndices;

  std::vector<LinearFloatAttribute> linear_float_attributes;
  std::vector<LinearVec2Attribute> linear_vec2_attributes;
  std::vector<LinearVec3Attribute> linear_vec3_attributes;
};
const enum SubdivSchema { CatmullClark, Loop, Bilinear };
GMesh subdivide(GMesh &mesh, int maxlevel, SubdivSchema schema);

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
GMesh noise_displace(GMesh &mesh, NoiseParamsStruct noise_params, bool multithreaded = true);

void list_vertex_properties(GMesh &mesh);
};  // namespace NED::openmeshutils
#endif  // CPP_MESHER_OPENMESH_UTILS_H
