#ifndef MESH_OPERATORS_H
#define MESH_OPERATORS_H
#pragma once

#include <stdint.h>

#include <iostream>
#include <string>
#include <type_traits>

#include "ImguiNode.h"
#include "MeshImporter.h"
#include "NodeParam.h"
#include "ParamFactory.h"
#include "mesh.h"
#include "mesh_utils.h"

namespace NED {


struct TransformParams{
 
  TransformParams(){
    translate = CREATE_PARAM(NED::Param<glm::vec3>, "translate");
    translate->value = glm::vec3(0.0f);
    rotate = CREATE_PARAM(NED::Param<glm::vec3>, "rotate");
    rotate->value = glm::vec3(0.0f);
    scale = CREATE_PARAM(NED::Param<glm::vec3>, "scale");
    scale->value = glm::vec3(1.0f);

    translate->default_val = glm::vec3(0.0f);
    rotate->default_val = glm::vec3(0.0f);
    scale->default_val = glm::vec3(1.0f);

    transform_order = CREATE_PARAM(NED::ParamComboBox, "transform order");
    transform_order->SetChoices({"T/R/S", "T/S/R", "R/T/S", "R/S/T", "S/T/R", "S/R/T"});

    axis_order = CREATE_PARAM(NED::ParamComboBox, "Rotate Axis Order");
    axis_order->SetChoices({"XYZ", "XZY", "YXZ", "YZX", "ZXY", "ZYX"});    
  }
  ~TransformParams() = default;

  std::shared_ptr<Param<glm::vec3>> translate;
  std::shared_ptr<Param<glm::vec3>> rotate;
  std::shared_ptr<Param<glm::vec3>> scale;
  std::shared_ptr<ParamComboBox> transform_order;
  std::shared_ptr<ParamComboBox> axis_order;
};


class MeshOperator : public ImGuiNode<msh::Mesh> {
 public:
  MeshOperator() : ImGuiNode("default") {};
  virtual ~MeshOperator() = default;
  virtual void Generate() = 0;
};

class MeshSubnetOperator : public SubnetNode<msh::Mesh> {
 public:
  MeshSubnetOperator() : SubnetNode() {};
  ~MeshSubnetOperator() = default;

  void Generate() override {
    if (node_network.outuput_node != nullptr) {
      // node_network.outuput_node->Update();
      auto op = std::dynamic_pointer_cast<MeshOperator>(node_network.outuput_node);
      if (op != nullptr) {
        std::cout << "Generate Subnet Data cache ????" << std::endl;
        m_DataCache = op->m_DataCache;
      }
    }
  }

 public:
  // msh::Mesh m_MeshCache;
};

class MeshGenerator : public MeshOperator {
 public:
  MeshGenerator() : MeshOperator() {
    SetNumAvailableInputs(0);
    color = NODE_COLOR::DARK_GREEN;
  };
  ~MeshGenerator() {};
  virtual void Generate() = 0;

 private:
};

class SquareGenerator : public MeshGenerator {
 public:
  SquareGenerator() : MeshGenerator() {};
  ~SquareGenerator() {};

  void Generate() override { m_DataCache = msh::meshutils::generate_grid(1.0f, 1.0f, 1, 1); }

 private:
};

class GridGenerator : public MeshGenerator {
 public:
  GridGenerator() : MeshGenerator() {

    width = CREATE_PARAM(NED::Param<float>, "Width");
    width->value = 1.0f;
    length = CREATE_PARAM(NED::Param<float>, "length");
    length->value = 1.0f;
    cols = CREATE_PARAM(NED::Param<int>,"cols");
    cols->value = 32;
    cols->min_val = 1;
    rows = CREATE_PARAM(NED::Param<int>,"rows");
    rows->value = 32;
    rows->min_val = 1;

    m_ParamLayout.params = {width, length, cols, rows};

    icon_name = "grid";
  };
  ~GridGenerator() {};

  void Generate() override {
    m_DataCache = msh::meshutils::generate_grid(width->Eval(), length->Eval(), cols->Eval(), rows->Eval());
  }

  std::shared_ptr<Param<float>> width;
  std::shared_ptr<Param<float>> length;
  std::shared_ptr<Param<int>> cols;
  std::shared_ptr<Param<int>> rows;

 private:
};

class TubeGenerator : public MeshGenerator {
 public:
  TubeGenerator() : MeshGenerator() {
    radius1 = CREATE_PARAM(NED::Param<float>,"radius1");
    radius1->value = 1.0f;

    radius2 = CREATE_PARAM(NED::Param<float>,"radius2");
    radius2->value = 1.0f;
    
    height = CREATE_PARAM(NED::Param<float>,"height");
    height->value = 5.0f;
    
    cols = CREATE_PARAM(NED::Param<int>,"cols");
    cols->value = 32;
    cols->min_val = 1;
    
    rows = CREATE_PARAM(NED::Param<int>,"rows");
    rows->value = 32;
    rows->min_val = 1;

    m_ParamLayout.params = {radius1, radius2, height, cols, rows};

    icon_name = "tube";
  };
  ~TubeGenerator() {};

  void Generate() override {
    m_DataCache =
        msh::meshutils::generate_tube(radius1->Eval(), radius2->Eval(), height->Eval(), cols->Eval(), rows->Eval());
  }

  std::shared_ptr<Param<float>> radius1;
  std::shared_ptr<Param<float>> radius2;
  std::shared_ptr<Param<float>> height;
  std::shared_ptr<Param<int>> cols;
  std::shared_ptr<Param<int>> rows;

 private:
};

class TorusGenerator : public MeshGenerator {
 public:
  TorusGenerator() : MeshGenerator() {
    radius1 = CREATE_PARAM(NED::Param<float>, "Radius 1");
    radius1->value = 1.0f;

    radius2 = CREATE_PARAM(NED::Param<float>, "Radius 2");
    radius2->value = 1.0f;

    cols = CREATE_PARAM(NED::Param<int>, "Cols");
    cols->value = 32;
    cols->min_val = 1;

    rows = CREATE_PARAM(NED::Param<int>, "Rows");
    rows->value = 32;
    rows->min_val = 1;

    m_ParamLayout.params = {radius1, radius2, cols, rows};

    icon_name = "tube";
  };
  ~TorusGenerator() {};

  void Generate() override {
    m_DataCache =
        msh::meshutils::generate_torus(radius1->Eval(), radius2->Eval(), cols->Eval(), rows->Eval());
  }

  std::shared_ptr<Param<float>> radius1;
  std::shared_ptr<Param<float>> radius2;
  std::shared_ptr<Param<int>> cols;
  std::shared_ptr<Param<int>> rows;

 private:
};


class MeshModifier : public MeshOperator {
 public:
  MeshModifier() : MeshOperator() { color = NODE_COLOR::MAROON; };
  ~MeshModifier() {};
  virtual void Generate() = 0;

 private:
};

class NormalModifier : public MeshModifier {
 public:
  NormalModifier() : MeshModifier() { SetNumAvailableInputs(1); };
  ~NormalModifier() {};

  void Generate() override {
    if (GetInput(0) != nullptr) {
      auto op0 = static_cast<MeshOperator *>(GetInput(0).get());

      m_DataCache = op0->m_DataCache;
      m_DataCache.ComputeNormals();
    }
  }
};

class TransformModifier : public MeshModifier {
 public:
  TransformModifier() : MeshModifier() {
    SetNumAvailableInputs(1);

    m_ParamLayout.params = {tr.transform_order, tr.axis_order, tr.translate, tr.rotate, tr.scale};
  };

  ~TransformModifier() {};

  void Generate() override {
    if (GetInput(0) != nullptr) {
      auto op0 = static_cast<MeshOperator *>(GetInput(0).get());

      m_DataCache = op0->m_DataCache;
      msh::meshutils::transform(m_DataCache, tr.translate->Eval(), glm::radians(tr.rotate->Eval()), tr.scale->Eval(),
                                (msh::meshutils::TRANSFORM_ORDER)tr.transform_order->Eval(),
                                (msh::meshutils::AXIS_ORDER)tr.axis_order->Eval());
    }
  }

 public:
  TransformParams tr;
};

class NoiseDisplaceModifier : public MeshModifier {
 public:
  NoiseDisplaceModifier() : MeshModifier() {
    SetNumAvailableInputs(1);

    noise_type = CREATE_PARAM(NED::ParamComboBox, "Noise Type");
    noise_type->SetChoices({ "Simplex", "Perlin", "Cellular" });

    lacunarity = CREATE_PARAM(NED::Param<float>, "Lacunarity");
    lacunarity->value = 2.7f;

    gain = CREATE_PARAM(NED::Param<float>, "Gain");
    gain->value = 0.65f;
    
    amplitude = CREATE_PARAM(NED::Param<float>, "Amplitude");
    amplitude->value = 0.1f;

    frequency = CREATE_PARAM(NED::Param<float>, "Frequency");
    frequency->value = 1.0f;
    
    weightedStrength = CREATE_PARAM(NED::Param<float>, "Weighted Strength");
    weightedStrength->value = 1.0f;

    offset = CREATE_PARAM(NED::Param<glm::vec3>, "Offset");
    offset->value = glm::vec3(0.0f);

    seed = CREATE_PARAM(NED::Param<int>, "Seed");
    seed->value = 0;

    octaves = CREATE_PARAM(NED::Param<int>, "Octaves");
    octaves->value = 4;

    precompute_normals = CREATE_PARAM(NED::Param<bool>, "Precompute Normals");
    precompute_normals->value = false;
    m_ParamLayout.params = {
                            noise_type,
                            lacunarity,
                            gain,
                            amplitude,
                            frequency,
                            weightedStrength,
                            offset,
                            seed,
                            octaves,
                            precompute_normals};
  };
  ~NoiseDisplaceModifier() {};

  void Generate() override {
    if (GetInput(0) != nullptr) {
      auto op0 = static_cast<MeshOperator *>(GetInput(0).get());

      m_DataCache = op0->m_DataCache;
      msh::meshutils::NoiseParams noiseParams;
      noiseParams.noise_type = (msh::meshutils::NoiseType)noise_type->Eval();
      noiseParams.lacunarity = lacunarity->Eval();
      noiseParams.gain = gain->Eval();
      noiseParams.amplitude = amplitude->Eval();
      noiseParams.frequency = frequency->Eval();
      noiseParams.weightedStrength = weightedStrength->Eval();
      noiseParams.offset = offset->Eval();
      noiseParams.seed = seed->Eval();
      noiseParams.octaves = octaves->Eval();

      if (precompute_normals->Eval()) {
        m_DataCache.ComputeNormals();
      }
      msh::meshutils::noise_displace(m_DataCache, noiseParams);
    }
  }

  std::shared_ptr<ParamComboBox> noise_type;
  std::shared_ptr<Param<float>> lacunarity;
  std::shared_ptr<Param<float>> gain;
  std::shared_ptr<Param<float>> amplitude;
  std::shared_ptr<Param<float>> frequency;
  std::shared_ptr<Param<float>> weightedStrength;
  std::shared_ptr<Param<glm::vec3>> offset;
  std::shared_ptr<Param<int>> seed;
  std::shared_ptr<Param<int>> octaves;

  std::shared_ptr<Param<bool>> precompute_normals;
};

class MeshMerger : public MeshModifier {
 public:
  MeshMerger() : MeshModifier() {
    SetNumAvailableInputs(2);
    color = NODE_COLOR::DARK_GREY;
  };
  ~MeshMerger() {};

  void Generate() override {
    if (GetInput(0) != nullptr && GetInput(1) != nullptr) {
      auto op0 = static_cast<MeshOperator *>(GetInput(0).get());
      auto op1 = static_cast<MeshOperator *>(GetInput(1).get());
      m_DataCache = msh::meshutils::merge(op0->m_DataCache, op1->m_DataCache);
    }
  }
};

class MeshMergerMulti : public MeshModifier {
 public:
  MeshMergerMulti() : MeshModifier() {
    ActivateMultiInput();
    SetNumAvailableInputs(0);
    color = NODE_COLOR::DARK_GREY;
  };
  ~MeshMergerMulti() {};

  void Generate() override {
    msh::Mesh result;
    for (size_t i = 0; i < GetMultiInputCount(); i++) {
      if (GetMultiInput(i) != nullptr) {
        result = msh::meshutils::merge(result, static_cast<MeshOperator *>(GetMultiInput(i).get())->m_DataCache);
      }
    }

    m_DataCache = result;
  }
};

class MeshTwister : public MeshModifier {
 public:
  MeshTwister() : MeshModifier() {
    SetNumAvailableInputs(1);
    color = NODE_COLOR::DARK_GREY;
    turns = CREATE_PARAM(NED::Param<float>, "Turns");
    turns->value = 1.0f;
    m_ParamLayout.params = {turns};
  }

  ~MeshTwister() {};

  void Generate() override {
    if (GetInput(0) != nullptr) {
      auto op0 = static_cast<MeshOperator *>(GetInput(0).get());
      m_DataCache = op0->m_DataCache;
      msh::meshutils::twist(m_DataCache, turns->Eval());
    }
  }

 public:
  std::shared_ptr<Param<float>> turns;
};

class MeshCenter : public MeshModifier {
 public:
  MeshCenter() : MeshModifier() {
    SetNumAvailableInputs(1);
    color = NODE_COLOR::DARK_GREY;

    center_on_x = CREATE_PARAM(NED::ParamComboBox, "X");
    center_on_x->SetChoices({"Center", "Min", "Max"});
    center_on_x->value = 0;

    center_on_y = CREATE_PARAM(NED::ParamComboBox, "Y");
    center_on_y->SetChoices({"Center", "Min", "Max"});
    center_on_y->value = 0;

    center_on_z = CREATE_PARAM(NED::ParamComboBox, "Z");
    center_on_z->SetChoices({"Center", "Min", "Max"});
    center_on_z->value = 0;

    m_ParamLayout.params = {center_on_x, center_on_y, center_on_z};
  }
  ~MeshCenter() {};

  void Generate() override {
    if (GetInput(0) != nullptr) {
      auto op0 = static_cast<MeshOperator *>(GetInput(0).get());
      m_DataCache = op0->m_DataCache;
      msh::BoundingBox bbox = m_DataCache.ComputeAABB();
      auto center_x = bbox.position.x + bbox.size.x / 2.0f;
      auto center_y = bbox.position.y + bbox.size.y / 2.0f;
      auto center_z = bbox.position.z + bbox.size.z / 2.0f;
      
      auto min_x = bbox.position.x;
      auto min_y = bbox.position.y;
      auto min_z = bbox.position.z;

      auto max_x = bbox.position.x + bbox.size.x;
      auto max_y = bbox.position.y + bbox.size.y;
      auto max_z = bbox.position.z + bbox.size.z;
      if (center_on_x->Eval() == 0) {
        msh::meshutils::translate(m_DataCache, glm::vec3(-center_x, 0.0f, 0.0f));
      }else if(center_on_x->Eval() == 1){
        msh::meshutils::translate(m_DataCache, glm::vec3(-min_x, 0.0f, 0.0f));
      }else if(center_on_x->Eval() == 2){
        msh::meshutils::translate(m_DataCache, glm::vec3(-max_x, 0.0f, 0.0f));
      }

      if (center_on_y->Eval() == 0) {
        msh::meshutils::translate(m_DataCache, glm::vec3(0.0f, -center_y,  0.0f));
      }else if(center_on_y->Eval() == 1){
        msh::meshutils::translate(m_DataCache, glm::vec3(0.0f, -min_y, 0.0f));
      }else if(center_on_y->Eval() == 2){
        msh::meshutils::translate(m_DataCache, glm::vec3( 0.0f,-max_x, 0.0f));
      }
      if (center_on_z->Eval() == 0) {
        msh::meshutils::translate(m_DataCache, glm::vec3(0.0f, 0.0f, -center_z));
      }else if(center_on_z->Eval() == 1){
        msh::meshutils::translate(m_DataCache, glm::vec3(0.0f, 0.0f, -min_z));
      }else if(center_on_z->Eval() == 2){
        msh::meshutils::translate(m_DataCache, glm::vec3( 0.0f, 0.0f, -max_z));
      }
    }
  }

 public:
  std::shared_ptr<ParamComboBox> center_on_x;
  std::shared_ptr<ParamComboBox> center_on_y;
  std::shared_ptr<ParamComboBox> center_on_z;
};

class NullMeshOperator : public MeshModifier {
 public:
  NullMeshOperator() : MeshModifier() {
    SetNumAvailableInputs(1);
    color = NODE_COLOR::ORANGE;
  };
  ~NullMeshOperator() {};

  void Generate() override {
    if (GetInput(0) != nullptr) {
      auto op0 = static_cast<MeshOperator *>(GetInput(0).get());
      m_DataCache = op0->m_DataCache;
    }
  }
};

class FusePoints : public MeshModifier {
 public:
  FusePoints() : MeshModifier() { SetNumAvailableInputs(1); }

  ~FusePoints() {};

  void Generate() override {
    if (GetInput(0) != nullptr) {
      auto op0 = static_cast<MeshOperator *>(GetInput(0).get());
      m_DataCache = op0->m_DataCache;
      msh::meshutils::fuse_points(m_DataCache);
    }
  }
};

class MeshFileLoader : public MeshGenerator {
 public:
  MeshFileLoader() : MeshGenerator() {
    SetNumAvailableInputs(0);

    file_param = CREATE_PARAM(NED::ParamFile, "File");
    file_param->value = L"";
    m_ParamLayout.params = {file_param};
  }

  ~MeshFileLoader() {};

  void Generate() override {
    std::string converted_str = wide_to_utf8(file_param->Eval());
    if (converted_str != "") {
      msh::MeshImporter *importer = msh::MeshImporter::GetInstance();
      auto imported_mesh = importer->Import(converted_str.c_str());
      m_DataCache = imported_mesh;
    } else {
      m_DataCache = msh::Mesh();
    }
  }

 public:
  std::shared_ptr<ParamFile> file_param;
};

class MeshSubdivide : public MeshModifier {
 public:
  MeshSubdivide() : MeshModifier() {
    SetNumAvailableInputs(1);

    subdiv_schema_p = CREATE_PARAM(NED::ParamComboBox, "Schema");
    subdiv_schema_p->SetChoices({"Catmull-Clark", "Loop", "Bilinear"});

    max_level_p = CREATE_PARAM(NED::Param<int>, "Subdivide Level");
    max_level_p->value = 1;
    max_level_p->min_val = 1;
    m_ParamLayout.params = {subdiv_schema_p, max_level_p};
  };
  ~MeshSubdivide() {};
  void Generate() override {
    if (GetInput(0) != nullptr) {
      auto op0 = static_cast<MeshOperator *>(GetInput(0).get());
      m_DataCache = op0->m_DataCache;
      m_DataCache = msh::meshutils::subdivide(m_DataCache, max_level_p->Eval(),
                                              (msh::meshutils::SubdivSchema)subdiv_schema_p->GetChoice());
    }
  }

 public:
  std::shared_ptr<ParamComboBox> subdiv_schema_p;
  std::shared_ptr<Param<int>> max_level_p;
};
class MeshTriangulate : public MeshModifier {
 public:
  MeshTriangulate() : MeshModifier() {
    SetNumAvailableInputs(1);
    m_ParamLayout.params = {};
  };
  ~MeshTriangulate() {};
  void Generate() override {
    if (GetInput(0) != nullptr) {
      auto op0 = static_cast<MeshOperator *>(GetInput(0).get());
      m_DataCache = op0->m_DataCache;
      m_DataCache = msh::meshutils::triangulate(m_DataCache);
    }
  }

 public:
};


class MeshDuplicate : public MeshModifier {
 public:
  MeshDuplicate() : MeshModifier() {
    SetNumAvailableInputs(1);
    num_copies_p = CREATE_PARAM(NED::Param<int>, "Num Copies");
    num_copies_p->value = 1;
    num_copies_p->min_val = 0;
    m_ParamLayout.params = {num_copies_p, tr.translate, tr.rotate, tr.scale, tr.transform_order, tr.axis_order};
  }

  ~MeshDuplicate() {}

  void Generate() override {
    if (GetInput(0) != nullptr) {
      auto op0 = static_cast<MeshOperator *>(GetInput(0).get());
      msh::Mesh merged = op0->m_DataCache;

      int num = num_copies_p->Eval();
      glm::vec3 scale_mult = glm::vec3(1.0f);

      for (int i = 0; i < num; i++) {
        scale_mult *= tr.scale->Eval();
        msh::Mesh src = op0->m_DataCache;

        auto pos = tr.translate->Eval() * (float)(i + 1);
        auto rot = glm::radians(tr.rotate->Eval() * (float)(i + 1));
        auto sc = scale_mult;
        msh::meshutils::transform(src, pos, rot, sc, (msh::meshutils::TRANSFORM_ORDER)tr.transform_order->Eval(),
                                  (msh::meshutils::AXIS_ORDER)tr.axis_order->Eval());

        merged = msh::meshutils::merge(merged, src);
      }
      m_DataCache = merged;
    }
  }

 public:
  TransformParams tr;
  std::shared_ptr<Param<int>> num_copies_p;
};

};  // end namespace NED

#endif  // MESHOPERATORS_H