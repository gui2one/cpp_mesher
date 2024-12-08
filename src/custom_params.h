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
    for (const auto& pt : rhs.points) {
      Node pt_node;
      pt_node["pos"] = pt.pos;
      pt_node["val"] = pt.val;
      node["points"].push_back(pt_node);
    }

    // node["points"]
    return node;
  }

  static bool decode(const Node& node, NED::FloatRamp& rhs) {
    if (!node.IsMap()) {
      return false;
    }
    rhs.value = node["value"].as<float>();
    rhs.name = node["name"].as<std::string>();
    for (const auto& pt_node : node["points"]) {
      NED::FloatRampPoint pt;
      pt.pos = pt_node["pos"].as<float>();
      pt.val = pt_node["val"].as<float>();
      rhs.points.push_back(pt);
    }
    return true;
  }
};
}  // namespace YAML
namespace NED {

class ParamFloatRamp : public Param<FloatRamp> {
 public:
  int current_point = -1;

 public:
  void Init() {
    AddPoint(0.0f, 0.0f);
    AddPoint(1.0f, 1.0f);
  }

  void AddPoint(float pos, float val) {
    value.points.push_back({pos, val});
    temp_value = value;
  }
  void Display() {
    DISPLAY_PARAM_TEMPLATE("FloatRamp", [this]() {
      auto canvas_p0 = ImGui::GetCursorScreenPos();
      ImDrawList* draw_list = ImGui::GetWindowDrawList();
      float height = 50.0f;
      ImGui::Dummy(ImVec2(ImGui::GetContentRegionAvail().x, height));
      draw_list->AddRectFilled(canvas_p0, canvas_p0 + ImVec2(ImGui::GetContentRegionAvail().x, height),
                               IM_COL32(50, 50, 50, 255));

      // draw ramp points
      auto pt_color = IM_COL32(0, 255, 255, 255);
      auto cur_pt_color = IM_COL32(255, 255, 0, 255);

      for (size_t i = 0; i < temp_value.points.size(); i++) {
        auto cur_pt = temp_value.points[i];
        float x = (cur_pt.pos * ImGui::GetContentRegionAvail().x) + canvas_p0.x;
        float y = (1.0f - cur_pt.val) * height + canvas_p0.y;
        auto color = pt_color;
        if (current_point == i) {
          color = cur_pt_color;
        }
        draw_list->AddCircleFilled(ImVec2(x, y), 5.0f, color);
        draw_list->AddCircleFilled(ImVec2(x, y), 5.0f, color);
      }

      if (ImGui::InputInt("Current point", &current_point)) {
      }

      if (current_point >= 0 && current_point < temp_value.points.size()) {
        ImGui::SliderFloat("Pos", &temp_value.points[current_point].pos, 0.0f, 1.0f, "%.3f");

        if (ImGui::IsItemDeactivatedAfterEdit()) {
          old_value = value;
          value = temp_value;
          DISPATCH_PARAM_CHANGE_EVENT(FloatRamp, m_Node, m_Label, value, old_value);
        }

        ImGui::SliderFloat("Val", &temp_value.points[current_point].val, 0.0f, 1.0f, "%.3f");

        if (ImGui::IsItemDeactivatedAfterEdit()) {
          old_value = value;
          value = temp_value;
          DISPATCH_PARAM_CHANGE_EVENT(FloatRamp, m_Node, m_Label, value, old_value);
        }
      }
      ImGui::Separator();
      // for (const auto& pt : value.points) {

      //  float x = (pt.pos * ImGui::GetContentRegionAvail().x) + canvas_p0.x;
      //  float y = (1.0f - pt.val) * height + canvas_p0.y;
      //  draw_list->AddCircleFilled(ImVec2(x, y), 5.0f, IM_COL32(0, 255, 255, 255));
      //}
    });
  }
};
}  // namespace NED
#endif  // CPP_MESHER_CUSTOM_PARAMS_H