#ifndef MESH_OPERATORS_H
#define MESH_OPERATORS_H
#pragma once

#include "ImguiNode.h"
#include "NodeParam.h"
#include <iostream>
#include <stdint.h>
#include <string>
#include <type_traits>

#include "MeshImporter.h"
#include "mesh.h"
#include "mesh_utils.h"

namespace NodeEditor {

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
      auto op =
          std::dynamic_pointer_cast<MeshOperator>(node_network.outuput_node);
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

  void Generate() override {
    m_DataCache = msh::meshutils::generateGrid(1.0f, 1.0f, 1, 1);
  }

private:
};

class GridGenerator : public MeshGenerator {
public:
  GridGenerator() : MeshGenerator() {

    width = std::make_shared<Param<float>>("width", 1.0f);
    length = std::make_shared<Param<float>>("length", 1.0f);
    cols = std::make_shared<Param<int>>("cols", 32);
    rows = std::make_shared<Param<int>>("rows", 32);

    m_ParamLayout.params = {width, length, cols, rows};

    icon_name = "grid";
  };
  ~GridGenerator() {};

  void Generate() override {
    m_DataCache = msh::meshutils::generateGrid(width->Eval(), length->Eval(),
                                               cols->Eval(), rows->Eval());
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

    radius1 = std::make_shared<Param<float>>("radius1", 1.0f);
    radius2 = std::make_shared<Param<float>>("radius2", 1.0f);
    height = std::make_shared<Param<float>>("height", 2.0f);
    cols = std::make_shared<Param<int>>("cols", 32);
    rows = std::make_shared<Param<int>>("rows", 32);

    m_ParamLayout.params = {radius1, radius2, height, cols, rows};

    icon_name = "tube";
  };
  ~TubeGenerator() {};

  void Generate() override {
    m_DataCache = msh::meshutils::generateTube(radius1->Eval(), radius2->Eval(),
                                               height->Eval(), cols->Eval(),
                                               rows->Eval());
  }

  std::shared_ptr<Param<float>> radius1;
  std::shared_ptr<Param<float>> radius2;
  std::shared_ptr<Param<float>> height;
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

    transform_order = std::make_shared<ParamComboBox>("transform order");
    transform_order->SetChoices(
        {"T/R/S", "T/S/R", "R/T/S", "R/S/T", "S/T/R", "S/R/T"});


    axis_order = std::make_shared<ParamComboBox>("Rotate axis order");
    axis_order->SetChoices({
      "XYZ",
      "XZY",
      "YXZ",
      "YZX",
      "ZXY",
      "ZYX"
    });
    translate = std::make_shared<Param<glm::vec3>>("translate",
                                                   glm::vec3(0.0f, 0.0f, 0.0f));
    rotate = std::make_shared<Param<glm::vec3>>("rotate",
                                                glm::vec3(0.0f, 0.0f, 0.0f));
    scale = std::make_shared<Param<glm::vec3>>("scale",
                                               glm::vec3(1.0f, 1.0f, 1.0f));

    m_ParamLayout.params = {transform_order, axis_order, translate, rotate, scale};
  };

  ~TransformModifier() {};

  void Generate() override {
    if (GetInput(0) != nullptr) {
      auto op0 = static_cast<MeshOperator *>(GetInput(0).get());

      m_DataCache = op0->m_DataCache;
      msh::meshutils::transform(
        m_DataCache, 
        translate->Eval(), 
        rotate->Eval(), 
        scale->Eval(), 
        (msh::meshutils::TRANSFORM_ORDER)transform_order->Eval(),
        (msh::meshutils::AXIS_ORDER)axis_order->Eval()
      );
    }
  }

public:
  std::shared_ptr<Param<glm::vec3>> translate;
  std::shared_ptr<Param<glm::vec3>> rotate;
  std::shared_ptr<Param<glm::vec3>> scale;
  std::shared_ptr<ParamComboBox> transform_order;
  std::shared_ptr<ParamComboBox> axis_order;
};

class NoiseDisplaceModifier : public MeshModifier {
public:
  NoiseDisplaceModifier() : MeshModifier() {
    SetNumAvailableInputs(1);
    lacunarity = std::make_shared<Param<float>>("lacunarity", 2.7f);
    gain = std::make_shared<Param<float>>("gain", 0.65f);
    amplitude = std::make_shared<Param<float>>("amplitude", 0.1f);
    frequency = std::make_shared<Param<float>>("frequency", 2.35f);
    weightedStrength = std::make_shared<Param<float>>("weightedStrength", 1.0f);
    offset = std::make_shared<Param<glm::vec3>>("offset",
                                                glm::vec3(0.0f, 0.0f, 0.0f));
    seed = std::make_shared<Param<int>>("seed", 0);
    octaves = std::make_shared<Param<int>>("octaves", 4);

    precompute_normals =
        std::make_shared<Param<bool>>("precompute normals", false);

    m_ParamLayout.params = {
        std::make_shared<ParamLabel>("Noise"),
        lacunarity,
        gain,
        amplitude,
        frequency,
        weightedStrength,
        offset,
        seed,
        octaves,
        std::make_shared<ParamSeparator>("-------------"),
        precompute_normals
    };
  };
  ~NoiseDisplaceModifier() {};

  void Generate() override {
    if (GetInput(0) != nullptr) {
      auto op0 = static_cast<MeshOperator *>(GetInput(0).get());

      m_DataCache = op0->m_DataCache;
      msh::meshutils::NoiseParams noiseParams;
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
      msh::meshutils::NoiseDisplace(m_DataCache, noiseParams);
    }
  }

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
        result = msh::meshutils::merge(
            result,
            static_cast<MeshOperator *>(GetMultiInput(i).get())->m_DataCache);
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

    turns = std::make_shared<Param<float>>("turns", 1.0f);

    m_ParamLayout.params = {turns};
  }

  ~MeshTwister() {};

  void Generate() override {
    if (GetInput(0) != nullptr) {
      auto op0 = static_cast<MeshOperator *>(GetInput(0).get());
      m_DataCache = op0->m_DataCache;
      msh::meshutils::Twist(m_DataCache, turns->Eval());
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

    center_on_x = std::make_shared<Param<bool>>("center on x", true);
    center_on_y = std::make_shared<Param<bool>>("center on y", true);
    center_on_z = std::make_shared<Param<bool>>("center on z", true);

    m_ParamLayout.params = { center_on_x, center_on_y, center_on_z };
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
      if (center_on_x->Eval())
        msh::meshutils::translate(m_DataCache,
                                  glm::vec3(-center_x, 0.0f, 0.0f));
      if (center_on_y->Eval())
        msh::meshutils::translate(m_DataCache,
                                  glm::vec3(0.0f, -center_y, 0.0f));
      if (center_on_z->Eval())
        msh::meshutils::translate(m_DataCache,
                                  glm::vec3(0.0f, 0.0f, -center_z));
    }
  }

public:
  std::shared_ptr<Param<bool>> center_on_x;
  std::shared_ptr<Param<bool>> center_on_y;
  std::shared_ptr<Param<bool>> center_on_z;
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
      msh::meshutils::fusePoints(m_DataCache);
    }
  }
};

class MeshFileLoader : public MeshGenerator {
public:
  MeshFileLoader() : MeshGenerator() {
    SetNumAvailableInputs(0);

    file_param = std::make_shared<ParamFile>("file", L"");

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


class MeshSubdivide : public MeshModifier{
 public:
  MeshSubdivide() : MeshModifier() { 
    SetNumAvailableInputs(1); 
    
    subdiv_schema_p = std::make_shared<ParamComboBox>("Schema");
    subdiv_schema_p->SetChoices({"Catmull-Clark", "Loop", "Bilinear"});
    max_level_p = std::make_shared<Param<int>>("subdivide level", 1);
    m_ParamLayout.params = {subdiv_schema_p, max_level_p};
  };
  ~MeshSubdivide() {};
  void Generate() override {
    if (GetInput(0) != nullptr) {
      auto op0 = static_cast<MeshOperator *>(GetInput(0).get());
      m_DataCache = op0->m_DataCache;
      m_DataCache = msh::meshutils::subdivide(m_DataCache, max_level_p->Eval(), (msh::meshutils::SubdivSchema)subdiv_schema_p->GetChoice());
    }
  }

 public:
  std::shared_ptr<ParamComboBox> subdiv_schema_p;
  std::shared_ptr<Param<int>> max_level_p;
};
class MeshTriangulate : public MeshModifier{
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

class MeshDuplicate : public MeshModifier{
public:
  MeshDuplicate() : MeshModifier() {
    SetNumAvailableInputs(1);
    num_copies_p = std::make_shared<Param<int>>("Num Copies", 1);
    num_copies_p->min_val = 0;
    transform_order = std::make_shared<ParamComboBox>("transform order");
    transform_order->SetChoices(
        {"T/R/S", "T/S/R", "R/T/S", "R/S/T", "S/T/R", "S/R/T"});


    axis_order = std::make_shared<ParamComboBox>("Rotate axis order");
    axis_order->SetChoices({
      "XYZ",
      "XZY",
      "YXZ",
      "YZX",
      "ZXY",
      "ZYX"
    });
    translate_p = std::make_shared<Param<glm::vec3>>("translate",
                                                   glm::vec3(0.0f, 0.0f, 0.0f));
    rotate_p = std::make_shared<Param<glm::vec3>>("rotate",
                                                glm::vec3(0.0f, 0.0f, 0.0f));
    scale_p = std::make_shared<Param<glm::vec3>>("scale",
                                               glm::vec3(1.0f, 1.0f, 1.0f));
    
    m_ParamLayout.params = {
      num_copies_p,
      translate_p, 
      rotate_p, 
      scale_p,
      transform_order,
      axis_order
    };
  }

  ~MeshDuplicate(){}

  void Generate() override{
    if (GetInput(0) != nullptr) {
      auto op0 = static_cast<MeshOperator *>(GetInput(0).get());
      msh::Mesh merged = op0->m_DataCache;
      
      int num = num_copies_p->Eval();
      glm::vec3 scale_mult = glm::vec3(1.0f);

      for(int i= 0; i< num; i++){
        scale_mult *= scale_p->Eval();
        msh::Mesh src = op0->m_DataCache;

        auto pos = translate_p->Eval() * (float)(i + 1);
        auto rot = glm::radians(rotate_p->Eval() * (float)(i + 1));
        auto sc = scale_mult;
        msh::meshutils::transform(
          src, 
          pos, rot, sc, 
          (msh::meshutils::TRANSFORM_ORDER)transform_order->Eval(), 
          (msh::meshutils::AXIS_ORDER)axis_order->Eval()
        );
        
        merged = msh::meshutils::merge(merged, src);
      }
      m_DataCache = merged;
    }
  }

public:
  std::shared_ptr<Param<int>> num_copies_p;
  std::shared_ptr<Param<glm::vec3>> translate_p;
  std::shared_ptr<Param<glm::vec3>> rotate_p;
  std::shared_ptr<Param<glm::vec3>> scale_p;
  std::shared_ptr<ParamComboBox> transform_order;
  std::shared_ptr<ParamComboBox> axis_order;  
};


}; // end namespace NodeEditor

#endif // MESHOPERATORS_H