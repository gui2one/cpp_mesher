#include "param_utils.h"

#include "ImGuiNode.h"
#include "NodeParam.h"

namespace msh {
void TransformParams::Init(NED::AbstractNode *_node) {
  node = _node;

  translate = CREATE_PARAM(NED::ParamVec3, "translate", node);
  translate->Set(glm::vec3(0.0f));
  translate->default_val = glm::vec3(0.0f);

  rotate = CREATE_PARAM(NED::ParamVec3, "rotate", node);
  rotate->Set(glm::vec3(0.0f));
  rotate->default_val = glm::vec3(0.0f);

  scale = CREATE_PARAM(NED::ParamVec3, "scale", node);
  scale->Set(glm::vec3(1.0f));
  scale->default_val = glm::vec3(1.0f);

  transform_order = CREATE_PARAM(NED::ParamComboBox, "transform order", node);
  transform_order->SetChoices({"T/R/S", "T/S/R", "R/T/S", "R/S/T", "S/T/R", "S/R/T"});
  transform_order->Set(0);

  axis_order = CREATE_PARAM(NED::ParamComboBox, "Rotate Axis Order", node);
  axis_order->SetChoices({"XYZ", "XZY", "YXZ", "YZX", "ZXY", "ZYX"});
  axis_order->Set(0);
}

void NoiseParams::Init(NED::AbstractNode *_node) {
  noise_type = CREATE_PARAM(NED::ParamComboBox, "Noise Type", _node);
  noise_type->SetChoices({"Simplex", "Perlin", "Cellular"});
  noise_type->Set(0);

  lacunarity = CREATE_PARAM(NED::ParamFloat, "Lacunarity", _node);
  lacunarity->Set(2.7f, 0.f, 4.0f);

  gain = CREATE_PARAM(NED::ParamFloat, "Gain", _node);
  gain->Set(0.65f, 0.f, 1.0f);

  amplitude = CREATE_PARAM(NED::ParamFloat, "Amplitude", _node);
  amplitude->Set(0.1f, 0.0f, 3.0f);

  frequency = CREATE_PARAM(NED::ParamFloat, "Frequency", _node);
  frequency->Set(1.0f, 0.0f, 5.0f);

  weightedStrength = CREATE_PARAM(NED::ParamFloat, "Weighted Strength", _node);
  weightedStrength->Set(1.0f, 0.f, 5.0f);

  offset = CREATE_PARAM(NED::ParamVec3, "Offset", _node);
  offset->Set(glm::vec3(0.0f));

  seed = CREATE_PARAM(NED::ParamInt, "Seed", _node);
  seed->Set(0, 0, 1024);

  octaves = CREATE_PARAM(NED::ParamInt, "Octaves", _node);
  octaves->Set(4, 0, 8);

  precompute_normals = CREATE_PARAM(NED::ParamBool, "Precompute Normals", _node);
  precompute_normals->Set(false);
}
};  // namespace msh