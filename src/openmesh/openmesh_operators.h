#ifndef OPENMESH_OPERATORS_H
#define OPENMESH_OPERATORS_H
#pragma once
#include <ImGuiNode.h>

#include "../custom_params.h"
#include "NodeParam.h"
#include "openmesh_utils.h"

namespace NED {

class OpenMeshOperator : public ImGuiNode<GMesh> {
 public:
  OpenMeshOperator() : ImGuiNode("default") {}
  virtual ~OpenMeshOperator() = default;
  virtual void Generate() = 0;
  void ClearCache() { m_DataCache = GMesh(); }
};

class OpenMeshNullOperator : public OpenMeshOperator {
 public:
  OpenMeshNullOperator() : OpenMeshOperator() {
    SetNumAvailableInputs(1);
    color = NODE_COLOR::ORANGE;
  };
  ~OpenMeshNullOperator() {};

  void Generate() override {
    if (GetInput(0) != nullptr) {
      auto op0 = static_cast<OpenMeshOperator *>(GetInput(0));
      m_DataCache = op0->m_DataCache;
    }
  }
};

class OpenMeshSubnetOperator : public SubnetNode<GMesh> {
 public:
  OpenMeshSubnetOperator() : SubnetNode() {};
  ~OpenMeshSubnetOperator() = default;

  void Generate() override {
    if (node_network.outuput_node != nullptr) {
      // node_network.outuput_node->Update();
      auto op = std::dynamic_pointer_cast<OpenMeshOperator>(node_network.outuput_node);
      if (op != nullptr) {
        std::cout << "Generate Subnet Data cache ????" << std::endl;
        m_DataCache = op->m_DataCache;
      }
    }
  }

 public:
  // msh::Mesh m_MeshCache;
};

class OpenMeshCubeGenerator : public OpenMeshOperator {
 public:
  OpenMeshCubeGenerator() : OpenMeshOperator() {
    color = NODE_COLOR::DARK_GREEN;
    SetNumAvailableInputs(0);

    ramp_p = CREATE_PARAM(NED::Param<FloatRamp>, "FloatRamp", this);
    auto float_p = CREATE_PARAM(NED::Param<float>, "Float", this);
    m_ParamLayout.params = {ramp_p, float_p};
  }
  ~OpenMeshCubeGenerator() {}
  void Generate() override { m_DataCache = openmeshutils::openmesh_cube(); }

 public:
  std::shared_ptr<Param<FloatRamp>> ramp_p;
};

class OpenMeshSquareGenerator : public OpenMeshOperator {
 public:
  OpenMeshSquareGenerator() : OpenMeshOperator() {
    color = NODE_COLOR::MAROON;
    SetNumAvailableInputs(0);
  }
  ~OpenMeshSquareGenerator() {}
  void Generate() override { m_DataCache = openmeshutils::openmesh_square(); }
};

class OpenMeshComputeNormals : public OpenMeshOperator {
 public:
  OpenMeshComputeNormals() : OpenMeshOperator() {
    color = NODE_COLOR::DARK_GREEN;
    SetNumAvailableInputs(1);
  }
  ~OpenMeshComputeNormals() {}
  void Generate() override {
    if (GetInput(0) != nullptr) {
      auto op0 = static_cast<OpenMeshOperator *>(GetInput(0));
      m_DataCache = op0->m_DataCache;
      NED::openmeshutils::compute_normals(m_DataCache);
    }
  }
};

class OpenMeshSetNormals : public OpenMeshOperator {
 public:
  OpenMeshSetNormals() : OpenMeshOperator() {
    color = NODE_COLOR::DARK_GREEN;
    SetNumAvailableInputs(1);

    normal_p = CREATE_PARAM(NED::ParamVec3, "Normal", this);
    normal_p->Set(glm::vec3(0.0f, 0.0f, 1.0f));
    m_ParamLayout.params = {normal_p};
  }
  ~OpenMeshSetNormals() {}
  void Generate() override {
    if (GetInput(0) != nullptr) {
      auto op0 = static_cast<OpenMeshOperator *>(GetInput(0));
      m_DataCache = op0->m_DataCache;
      NED::openmeshutils::set_normals(m_DataCache, normal_p->Eval());
    }
  }

 public:
  std::shared_ptr<ParamVec3> normal_p;
};

class OpenMeshTriangulate : public OpenMeshOperator {
 public:
  OpenMeshTriangulate() : OpenMeshOperator() {
    color = NODE_COLOR::DARK_GREEN;
    SetNumAvailableInputs(1);
  }
  ~OpenMeshTriangulate() {}

  void Generate() override {
    if (GetInput(0) != nullptr) {
      auto op0 = static_cast<OpenMeshOperator *>(GetInput(0));
      m_DataCache = op0->m_DataCache;
      NED::openmeshutils::triangulate(m_DataCache);
    }
  }
};
};  // namespace NED

#endif  // OPENMESH_OPERATORS_H