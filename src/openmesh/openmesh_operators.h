#ifndef OPENMESH_OPERATORS_H
#define OPENMESH_OPERATORS_H
#pragma once
#include <ImGuiNode.h>

#include "../custom_params.h"
#include "MeshImporter.h"
#include "NodeParam.h"
// #include "nodes/MeshOperators.h"
#include "openmesh_utils.h"
#include "utils.h"

namespace NED {
struct TransformParams {
  TransformParams() {}
  ~TransformParams() = default;
  inline void Init(AbstractNode *_node) {
    node = _node;
    translate = CREATE_PARAM(NED::ParamVec3, "translate", node);
    translate->Set(glm::vec3(0.0f));
    rotate = CREATE_PARAM(NED::ParamVec3, "rotate", node);
    rotate->Set(glm::vec3(0.0f));
    scale = CREATE_PARAM(NED::ParamVec3, "scale", node);
    scale->Set(glm::vec3(1.0f));

    translate->default_val = glm::vec3(0.0f);
    rotate->default_val = glm::vec3(0.0f);
    scale->default_val = glm::vec3(1.0f);

    transform_order = CREATE_PARAM(NED::ParamComboBox, "transform order", node);
    transform_order->SetChoices({"T/R/S", "T/S/R", "R/T/S", "R/S/T", "S/T/R", "S/R/T"});
    transform_order->Set(0);

    axis_order = CREATE_PARAM(NED::ParamComboBox, "Rotate Axis Order", node);
    axis_order->SetChoices({"XYZ", "XZY", "YXZ", "YZX", "ZXY", "ZYX"});
    axis_order->Set(0);
  }
  std::vector<std::shared_ptr<NodeParam>> GetParams() {
    return {transform_order, axis_order, translate, rotate, scale};
  }
  AbstractNode *node;
  std::shared_ptr<ParamVec3> translate;
  std::shared_ptr<ParamVec3> rotate;
  std::shared_ptr<ParamVec3> scale;
  std::shared_ptr<ParamComboBox> transform_order;
  std::shared_ptr<ParamComboBox> axis_order;
};
class OpenMeshOperator : public ImGuiNode<GMesh> {
 public:
  OpenMeshOperator() : ImGuiNode("default") {}
  virtual ~OpenMeshOperator() = default;
  virtual void Generate() = 0;
  void ClearCache() { m_DataCache = GMesh(); }
};

class OpenMeshCubeGenerator : public OpenMeshOperator {
 public:
  OpenMeshCubeGenerator() : OpenMeshOperator() {
    color = NODE_COLOR::DARK_GREEN;
    SetNumAvailableInputs(0);

    ramp_p = CREATE_PARAM(NED::ParamFloatRamp, "FloatRamp", this);
    ramp_p->Init();
    auto float_p = CREATE_PARAM(NED::ParamFloat, "Float", this);
    m_ParamLayout.params = {ramp_p, float_p};
  }
  ~OpenMeshCubeGenerator() {}
  void Generate() override { m_DataCache = openmeshutils::openmesh_cube(); }

 public:
  std::shared_ptr<ParamFloatRamp> ramp_p;
};

class OpenMeshSquareGenerator : public OpenMeshOperator {
 public:
  OpenMeshSquareGenerator() : OpenMeshOperator() {
    color = NODE_COLOR::MAROON;
    SetNumAvailableInputs(0);
  }
  ~OpenMeshSquareGenerator() {}
  void Generate() override {
    auto square = openmeshutils::openmesh_square();
    m_DataCache = square;
  }
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

class OpenMeshFileImport : public OpenMeshOperator {
 public:
  OpenMeshFileImport() : OpenMeshOperator() {
    color = NODE_COLOR::DARK_GREEN;
    SetNumAvailableInputs(0);

    file_p = CREATE_PARAM(NED::ParamFile, "File", this);

    m_ParamLayout.params = {file_p};
  }
  ~OpenMeshFileImport() {}

  void Generate() override {
    auto _str = wide_to_utf8(file_p->value);
    m_DataCache = msh::MeshImporter::GetInstance()->GMeshImport(_str.c_str());
    //= openmeshutils::import_mesh_file(file_path);
  }

  std::shared_ptr<NED::ParamFile> file_p;
};

class OpenMeshTransform : public OpenMeshOperator {
 public:
  OpenMeshTransform() : OpenMeshOperator() {
    color = NODE_COLOR::DARK_GREEN;
    SetNumAvailableInputs(1);

    tr_params.Init(this);

    m_ParamLayout.Append(tr_params.GetParams());
  }
  ~OpenMeshTransform() {}

  void Generate() override {
    if (GetInput(0) != nullptr) {
      auto op0 = static_cast<OpenMeshOperator *>(GetInput(0));
      m_DataCache = op0->m_DataCache;
      // m_DataCache = openmeshutils::translate(m_DataCache, tr_params.translate->Eval());
      // m_DataCache = openmeshutils::rotate(m_DataCache, glm::radians(tr_params.rotate->Eval()),
      //                                     (openmeshutils::AXIS_ORDER)tr_params.axis_order->Eval());
      // m_DataCache = openmeshutils::scale(m_DataCache, tr_params.scale->Eval());

      m_DataCache = openmeshutils::transform(m_DataCache, tr_params.translate->Eval(),
                                             glm::radians(tr_params.rotate->Eval()), tr_params.scale->Eval(),
                                             (openmeshutils::TRANSFORM_ORDER)tr_params.transform_order->Eval(),
                                             (openmeshutils::AXIS_ORDER)tr_params.axis_order->Eval());
    }
  }

  NED::TransformParams tr_params;
};

};  // namespace NED

#endif  // OPENMESH_OPERATORS_H