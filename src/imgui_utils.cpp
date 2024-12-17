#include "imgui_utils.h"
namespace msh::ui {
void green_text(const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
  ImGui::TextV(fmt, args);
  ImGui::PopStyleColor();
}

};  // namespace msh::ui
