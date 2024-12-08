#ifndef CPP_MESHER_CUSTOM_PARAMS_H
#define CPP_MESHER_CUSTOM_PARAMS_H
#pragma once
#include <yaml-cpp/yaml.h>

#include "NodeParam.h"

namespace NED {

struct FloatRampPoint {
  float pos;
  float val;
};

struct FloatRamp {
  float value;
  std::vector<FloatRampPoint> points;
  std::string name;
};

};  // namespace NED

namespace YAML {

template <>
struct convert<NED::FloatRamp> {
  static Node encode(const NED::FloatRamp& rhs) {
    // std::cout << "Encoding Wstring to yaml" << std::endl;

    Node node;
    node["value"] = rhs.value;
    node["name"] = rhs.name;

    return node;
  }

  static bool decode(const Node& node, NED::FloatRamp& rhs) {
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

// template <>
// class Param<FloatRamp> : public NodeParam {
//  public:
//   FloatRamp value;
//   FloatRamp temp_value;
//   FloatRamp old_value;
//
//  public:
//   Param() = default;
//   Param(const char* _name, FloatRamp _value) : NodeParam(_name), value(_value) {};
//   ~Param() {};
//
//   float Eval(float t) { return 0.5f; }
//
//   void Set(FloatRamp _value) {
//     value = _value;
//     temp_value = _value;
//   }
// };

class ParamFloatRamp : public Param<FloatRamp> {
 public:
  // FloatRamp value;
  // FloatRamp temp_value;
  // FloatRamp old_value;

 public:
  // ParamFloatRamp() {};
  // ParamFloatRamp(FloatRamp _value) : Param<FloatRamp>("", _value) {};
  // ParamFloatRamp(const char* _name, FloatRamp _value) : Param<FloatRamp>(_name, _value) {}
  //~ParamFloatRamp() {};

  void Init() {
    FloatRampPoint p1{0.0f, 0.0f};
    FloatRampPoint p2{1.0f, 1.0f};

    value.points.push_back(p1);
    value.points.push_back(p2);
  }
  // float Eval(float t) { return 0.5f; }

  // void Set(FloatRamp _value) {
  //   value = _value;
  //   temp_value = _value;
  // }
  void Display() {
    DISPLAY_PARAM_TEMPLATE("FloatRamp", [this]() {
      auto canvas_p0 = ImGui::GetCursorScreenPos();
      ImDrawList* draw_list = ImGui::GetWindowDrawList();
      float height = 50.0f;
      ImGui::Dummy(ImVec2(ImGui::GetContentRegionAvail().x, height));
      draw_list->AddRectFilled(canvas_p0, canvas_p0 + ImVec2(ImGui::GetContentRegionAvail().x, height),
                               IM_COL32(50, 50, 50, 255));

      // draw ramp points
      for (const auto& pt : value.points) {
        // std::cout << "drawing point" << std::endl;
        float x = (pt.pos * ImGui::GetContentRegionAvail().x) + canvas_p0.x;
        float y = (1.0f - pt.val) * height + canvas_p0.y;
        draw_list->AddCircleFilled(ImVec2(x, y), 5.0f, IM_COL32(0, 255, 255, 255));
      }
    });
  }
  // NODE_EDITOR_PARAM_YAML_SERIALIZE_FUNC();

  // void YAMLDeserialize(YAML::Node yaml_node) {}
};  // end Param<FloatRamp>
}  // namespace NED
#endif  // CPP_MESHER_CUSTOM_PARAMS_H