#ifndef OPENMESH_OPERATORS_H
#define OPENMESH_OPERATORS_H
#pragma once
#include <ImGuiNode.h>

#include "../custom_params.h"
#include "MeshImporter.h"
#include "openmesh_utils.h"
#include "param_utils.h"
#include "utils.h"
namespace NED {

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
    color = NODE_COLOR::YELLOW;
    SetNumAvailableInputs(0);

    ramp_p = CREATE_PARAM(NED::ParamFloatRamp, "FloatRamp", this);
    ramp_p->Init();
    auto float_p = CREATE_PARAM(NED::ParamFloat, "Float", this);
    float_p->Set(0.0f, -5.0f, 5.0f);
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
    color = NODE_COLOR::YELLOW;
    SetNumAvailableInputs(0);

    compute_uvs_p = CREATE_PARAM(NED::ParamBool, "Compute UVs", this);
    compute_uvs_p->Set(false);
    m_ParamLayout.Append(compute_uvs_p);
  }
  ~OpenMeshSquareGenerator() {}
  void Generate() override {
    auto square = openmeshutils::openmesh_square(compute_uvs_p->Eval());
    m_DataCache = square;
  }

 public:
  std::shared_ptr<ParamBool> compute_uvs_p;
};

class OpenMeshGridGenerator : public OpenMeshOperator {
 public:
  OpenMeshGridGenerator() : OpenMeshOperator() {
    color = NODE_COLOR::YELLOW;
    SetNumAvailableInputs(0);
    width = CREATE_PARAM(NED::ParamFloat, "Width", this);
    width->Set(1.0f, 0.0f, 10.0f);
    length = CREATE_PARAM(NED::ParamFloat, "length", this);
    length->Set(1.0f, 0.0f, 10.0f);
    cols = CREATE_PARAM(NED::ParamInt, "cols", this);
    cols->Set(32, 2, 100);
    cols->min_val = 1;
    rows = CREATE_PARAM(NED::ParamInt, "rows", this);
    rows->Set(32, 2, 100);
    rows->min_val = 1;

    compute_uvs_p = CREATE_PARAM(NED::ParamBool, "Compute UVs", this);

    m_ParamLayout.params = {compute_uvs_p, width, length, cols, rows};

    icon_name = "grid";
  }
  ~OpenMeshGridGenerator() {}
  void Generate() override {
    m_DataCache =
        openmeshutils::openmesh_grid(width->Eval(), length->Eval(), cols->Eval(), rows->Eval(), compute_uvs_p->Eval());
  }

 public:
  std::shared_ptr<ParamFloat> width;
  std::shared_ptr<ParamFloat> length;
  std::shared_ptr<ParamInt> cols;
  std::shared_ptr<ParamInt> rows;

  std::shared_ptr<ParamBool> compute_uvs_p;
};

class OpenMeshTorusGenerator : public OpenMeshOperator {
 public:
  OpenMeshTorusGenerator() : OpenMeshOperator() {
    color = NODE_COLOR::YELLOW;
    SetNumAvailableInputs(0);
    radius1 = CREATE_PARAM(NED::ParamFloat, "Big Radius", this);
    radius1->Set(1.0f, 0.0f, 1.0f);
    radius2 = CREATE_PARAM(NED::ParamFloat, "Small Radius", this);
    radius2->Set(0.9f, 0.0f, 1.0f);
    cols = CREATE_PARAM(NED::ParamInt, "Cols", this);
    cols->Set(32, 0, 200);
    cols->min_val = 1;
    rows = CREATE_PARAM(NED::ParamInt, "Rows", this);
    rows->Set(32, 0, 200);
    rows->min_val = 1;

    m_ParamLayout.params = {radius1, radius2, cols, rows};

    icon_name = "torus";
  }
  ~OpenMeshTorusGenerator() {}
  void Generate() override {
    m_DataCache = openmeshutils::openmesh_torus(radius1->Eval(), radius2->Eval(), cols->Eval(), rows->Eval());
  }

 public:
  std::shared_ptr<ParamFloat> radius1;
  std::shared_ptr<ParamFloat> radius2;
  std::shared_ptr<ParamInt> cols;
  std::shared_ptr<ParamInt> rows;
};

class OpenMeshComputeNormals : public OpenMeshOperator {
 public:
  OpenMeshComputeNormals() : OpenMeshOperator() {
    color = NODE_COLOR::DARK_GREEN;
    SetNumAvailableInputs(1);

    reverse = CREATE_PARAM(NED::ParamBool, "Reverse", this);
    reverse->Set(false);
    m_ParamLayout.params = {reverse};
  }
  ~OpenMeshComputeNormals() {}
  void Generate() override {
    if (GetInput(0) != nullptr) {
      auto op0 = static_cast<OpenMeshOperator *>(GetInput(0));
      m_DataCache = op0->m_DataCache;
      NED::openmeshutils::compute_normals(m_DataCache, reverse->Eval());
    }
  }

 public:
  std::shared_ptr<ParamBool> reverse;
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
      m_DataCache = NED::openmeshutils::triangulate(op0->m_DataCache);
    }
  }
};

class OpenMeshFileImport : public OpenMeshOperator {
 public:
  OpenMeshFileImport() : OpenMeshOperator() {
    color = NODE_COLOR::YELLOW;
    SetNumAvailableInputs(0);

    file_p = CREATE_PARAM(NED::ParamFile, "File", this);

    m_ParamLayout.params = {file_p};

    icon_name = "load_file";
  }
  ~OpenMeshFileImport() {}

  void Generate() override {
    auto _str = wide_to_utf8(file_p->value);
    auto file_path = fs::path(_str);
    if (file_path.empty()) {
      m_DataCache = GMesh();
      return;
    }

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

      m_DataCache = openmeshutils::transform(m_DataCache, tr_params.translate->Eval(),
                                             glm::radians(tr_params.rotate->Eval()), tr_params.scale->Eval(),
                                             (openmeshutils::TRANSFORM_ORDER)tr_params.transform_order->Eval(),
                                             (openmeshutils::AXIS_ORDER)tr_params.axis_order->Eval());
    }
  }

  msh::TransformParams tr_params;
};

class OpenMeshNoiseDisplace : public OpenMeshOperator {
 public:
  OpenMeshNoiseDisplace() : OpenMeshOperator() {
    color = NODE_COLOR::DARK_GREEN;
    SetNumAvailableInputs(1);

    noise_p.Init(this);

    m_ParamLayout.Append(noise_p.GetParams());
  }
  ~OpenMeshNoiseDisplace() {}

  void Generate() override {
    if (GetInput(0) != nullptr) {
      auto op0 = static_cast<OpenMeshOperator *>(GetInput(0));
      m_DataCache = op0->m_DataCache;
      openmeshutils::NoiseParamsStruct noiseParamsS;
      noiseParamsS.noise_type = (openmeshutils::NoiseType)noise_p.noise_type->Eval();
      noiseParamsS.lacunarity = noise_p.lacunarity->Eval();
      noiseParamsS.gain = noise_p.gain->Eval();
      noiseParamsS.amplitude = noise_p.amplitude->Eval();
      noiseParamsS.frequency = noise_p.frequency->Eval();
      noiseParamsS.weightedStrength = noise_p.weightedStrength->Eval();
      noiseParamsS.offset = noise_p.offset->Eval();
      noiseParamsS.seed = noise_p.seed->Eval();
      noiseParamsS.octaves = noise_p.octaves->Eval();

      if (noise_p.precompute_normals->Eval()) NED::openmeshutils::compute_normals(m_DataCache);

      m_DataCache = openmeshutils::noise_displace(m_DataCache, noiseParamsS);
    }
  }

  msh::NoiseParams noise_p;
};

class OpenMeshAddProperty : public OpenMeshOperator {
 public:
  OpenMeshAddProperty() : OpenMeshOperator() {
    color = NODE_COLOR::ORANGE;
    SetNumAvailableInputs(1);

    name_p = CREATE_PARAM(NED::ParamString, "Name", this);
    name_p->Set("default");
    type_p = CREATE_PARAM(NED::ParamComboBox, "Type", this);
    type_p->SetChoices({"Float", "Int", "Vec2f", "Vec3f"});
    type_p->Set(0);
    m_ParamLayout.params = {name_p, type_p};
  }
  ~OpenMeshAddProperty() {}

  void Generate() override {
    if (GetInput(0) != nullptr) {
      auto op0 = static_cast<OpenMeshOperator *>(GetInput(0));

      m_DataCache = op0->m_DataCache;

      if (type_p->value == 0) {
        m_DataCache.add_dynamic_property(name_p->value, PropertyType::PROP_FLOAT);
      } else if (type_p->value == 1) {
        m_DataCache.add_dynamic_property(name_p->value, PropertyType::PROP_INT);
      } else if (type_p->value == 2) {
        m_DataCache.add_dynamic_property(name_p->value, PropertyType::PROP_VEC2F);
      } else if (type_p->value == 3) {
        m_DataCache.add_dynamic_property(name_p->value, PropertyType::PROP_VEC3F);
      }
    }
  }

  std::shared_ptr<ParamString> name_p;
  std::shared_ptr<ParamComboBox> type_p;
};

class OpenMeshSubdivide : public OpenMeshOperator {
 public:
  OpenMeshSubdivide() : OpenMeshOperator() {
    color = NODE_COLOR::DARK_GREEN;
    SetNumAvailableInputs(1);
    max_level_p = CREATE_PARAM(NED::ParamInt, "Max Level", this);
    max_level_p->Set(2, 0, 10);
    max_level_p->min_val = 1;
    schema_p = CREATE_PARAM(NED::ParamComboBox, "Schema", this);
    schema_p->SetChoices({"CatmullClark", "Loop", "Bilinear"});
    schema_p->Set(0);
    m_ParamLayout.params = {max_level_p, schema_p};
  }
  ~OpenMeshSubdivide() {}
  void Generate() override {
    if (GetInput(0) != nullptr) {
      auto op0 = static_cast<OpenMeshOperator *>(GetInput(0));
      m_DataCache = op0->m_DataCache;
      m_DataCache =
          openmeshutils::subdivide(m_DataCache, max_level_p->Eval(), (openmeshutils::SubdivSchema)schema_p->Eval());
    }
  }
  std::shared_ptr<ParamInt> max_level_p;
  std::shared_ptr<ParamComboBox> schema_p;
};

class OpenMeshCombine : public OpenMeshOperator {
 public:
  OpenMeshCombine() : OpenMeshOperator() { SetNumAvailableInputs(2); }

  ~OpenMeshCombine() {}

  void Generate() override {
    if (GetInput(0) != nullptr && GetInput(1) != nullptr) {
      auto op0 = static_cast<OpenMeshOperator *>(GetInput(0));
      auto op1 = static_cast<OpenMeshOperator *>(GetInput(1));

      GMesh merged = openmeshutils::combine(op0->m_DataCache, op1->m_DataCache);
      m_DataCache = merged;
    } else {
      m_DataCache = GMesh();
    }
  }
};
class OpenMeshDuplicate : public OpenMeshOperator {
 public:
  OpenMeshDuplicate() : OpenMeshOperator() {
    SetNumAvailableInputs(1);
    tr.Init(this);
    num_copies_p = CREATE_PARAM(NED::ParamInt, "Num Copies", this);
    num_copies_p->Set(1, 0, 10);
    m_ParamLayout.params = {num_copies_p, tr.translate, tr.rotate, tr.scale, tr.transform_order, tr.axis_order};
  }

  ~OpenMeshDuplicate() {}

  void Generate() override {
    if (GetInput(0) != nullptr) {
      auto op0 = static_cast<OpenMeshOperator *>(GetInput(0));
      GMesh merged = op0->m_DataCache;

      int num = num_copies_p->Eval();
      glm::vec3 scale_mult = glm::vec3(1.0f);

      for (int i = 0; i < num; i++) {
        scale_mult *= tr.scale->Eval();
        GMesh src = op0->m_DataCache;

        auto pos = tr.translate->Eval() * (float)(i + 1);
        auto rot = glm::radians(tr.rotate->Eval() * (float)(i + 1));
        auto sc = scale_mult;
        src = openmeshutils::transform(src, pos, rot, sc, (openmeshutils::TRANSFORM_ORDER)tr.transform_order->Eval(),
                                       (openmeshutils::AXIS_ORDER)tr.axis_order->Eval());

        merged = openmeshutils::combine(merged, src);
      }
      m_DataCache = merged;
    }
  }

 public:
  msh::TransformParams tr;
  std::shared_ptr<Param<int>> num_copies_p;
};

};  // namespace NED

#endif  // OPENMESH_OPERATORS_H