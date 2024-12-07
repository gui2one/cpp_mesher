#ifndef CPP_MESHER_CUSTOM_PARAMS_H
#define CPP_MESHER_CUSTOM_PARAMS_H
#pragma once
#include <yaml-cpp/yaml.h>

#include "NodeParam.h"

namespace NED {

struct Ramp {
  float value;
  std::string name;
};

};  // namespace NED

namespace YAML {

template <>
struct convert<NED::Ramp> {
  static Node encode(const NED::Ramp& rhs) {
    // std::cout << "Encoding Wstring to yaml" << std::endl;

    Node node;
    node["value"] = rhs.value;
    node["name"] = rhs.name;

    return node;
  }

  static bool decode(const Node& node, NED::Ramp& rhs) {
    if (!node.IsMap() || node.size() != 2) {
      return false;
    }
    rhs.value = node["value"].as<float>();
    rhs.name = node["name"].as<std::string>();
    return true;
  }
};
}  // namespace YAML
namespace NED {

template <>
class Param<Ramp> : public NodeParam {
 public:
  Ramp value;
  Ramp temp_value;
  Ramp old_value;

 public:
  Param() = default;
  Param(const char* _name, Ramp _value) : NodeParam(_name), value(_value) {}
  ~Param() {};

  Ramp Eval() { return value; }

  void Set(Ramp _value) {
    value = _value;
    temp_value = _value;
  }
  void Display() {
    DISPLAY_PARAM_TEMPLATE("Ramp", [this]() { ImGui::Text("not implemented YET !"); });
  }
  NODE_EDITOR_PARAM_YAML_SERIALIZE_FUNC();

  void YAMLDeserialize(YAML::Node yaml_node) {}
};  // end Param<Ramp>
}  // namespace NED
#endif  // CPP_MESHER_CUSTOM_PARAMS_H