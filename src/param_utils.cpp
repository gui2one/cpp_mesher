#include "param_utils.h"

#include "ImGuiNode.h"
#include "NodeParam.h"

namespace msh {
void TransformParams::Init(NED::AbstractNode *_node) {
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

void NoiseParams::Init(NED::AbstractNode *_node) {
  noise_type = CREATE_PARAM(NED::ParamComboBox, "Noise Type", _node);
  noise_type->SetChoices({"Simplex", "Perlin", "Cellular"});

  lacunarity = CREATE_PARAM(NED::ParamFloat, "Lacunarity", _node);
  lacunarity->value = 2.7f;

  gain = CREATE_PARAM(NED::ParamFloat, "Gain", _node);
  gain->value = 0.65f;

  amplitude = CREATE_PARAM(NED::ParamFloat, "Amplitude", _node);
  amplitude->value = 0.1f;

  frequency = CREATE_PARAM(NED::ParamFloat, "Frequency", _node);
  frequency->value = 1.0f;

  weightedStrength = CREATE_PARAM(NED::ParamFloat, "Weighted Strength", _node);
  weightedStrength->value = 1.0f;

  offset = CREATE_PARAM(NED::ParamVec3, "Offset", _node);
  offset->value = glm::vec3(0.0f);

  seed = CREATE_PARAM(NED::ParamInt, "Seed", _node);
  seed->value = 0;

  octaves = CREATE_PARAM(NED::ParamInt, "Octaves", _node);
  octaves->value = 4;

  precompute_normals = CREATE_PARAM(NED::ParamBool, "Precompute Normals", _node);
  precompute_normals->value = false;
}

};  // namespace msh