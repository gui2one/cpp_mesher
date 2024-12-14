#ifndef CPP_MESHER_PARAM_UTILS_H
#define CPP_MESHER_PARAM_UTILS_H
#pragma once
#include <memory>
#include <vector>

// forward delcarations
namespace NED {
class AbstractNode;
class NodeParam;
class ParamComboBox;
class ParamFloat;
class ParamInt;
class ParamBool;
class ParamVec3;
};  // namespace NED
namespace msh {

struct TransformParams {
  TransformParams() {}
  ~TransformParams() = default;
  void Init(NED::AbstractNode *_node);

  std::vector<std::shared_ptr<NED::NodeParam>> GetParams() {
    return {transform_order, axis_order, translate, rotate, scale};
  }
  NED::AbstractNode *node;
  std::shared_ptr<NED::ParamVec3> translate;
  std::shared_ptr<NED::ParamVec3> rotate;
  std::shared_ptr<NED::ParamVec3> scale;
  std::shared_ptr<NED::ParamComboBox> transform_order;
  std::shared_ptr<NED::ParamComboBox> axis_order;
};

struct NoiseParams {
  NoiseParams() {}
  ~NoiseParams() = default;
  void Init(NED::AbstractNode *_node);
  std::vector<std::shared_ptr<NED::NodeParam>> GetParams() {
    return {noise_type,       lacunarity, gain,    amplitude, frequency,
            weightedStrength, offset,     octaves, seed,      precompute_normals};
  }
  std::shared_ptr<NED::ParamComboBox> noise_type;
  std::shared_ptr<NED::ParamFloat> lacunarity;
  std::shared_ptr<NED::ParamFloat> gain;
  std::shared_ptr<NED::ParamFloat> amplitude;
  std::shared_ptr<NED::ParamFloat> frequency;
  std::shared_ptr<NED::ParamFloat> weightedStrength;
  std::shared_ptr<NED::ParamVec3> offset;
  std::shared_ptr<NED::ParamInt> octaves;
  std::shared_ptr<NED::ParamInt> seed;
  std::shared_ptr<NED::ParamBool> precompute_normals;
};

}  // namespace msh
#endif  // CPP_MESHER_PARAM_UTILS_H
