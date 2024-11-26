#include "Application.h"
namespace msh {

Application::Application() {}

Application::~Application() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  glfwDestroyWindow(m_NativeWindow);
  glfwTerminate();
}
void THEME_CatpuccinMochaColors() {
        ImGuiStyle &style = ImGui::GetStyle();
        ImVec4 *colors = style.Colors;

        // Base colors inspired by Catppuccin Mocha
        colors[ImGuiCol_Text] = ImVec4(0.90f, 0.89f, 0.88f, 1.00f);         // Latte
        colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.56f, 0.52f, 1.00f); // Surface2
        colors[ImGuiCol_WindowBg] = ImVec4(0.17f, 0.14f, 0.20f, 1.00f);     // Base
        colors[ImGuiCol_ChildBg] = ImVec4(0.18f, 0.16f, 0.22f, 1.00f);      // Mantle
        colors[ImGuiCol_PopupBg] = ImVec4(0.17f, 0.14f, 0.20f, 1.00f);      // Base
        colors[ImGuiCol_Border] = ImVec4(0.27f, 0.23f, 0.29f, 1.00f);       // Overlay0
        colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_FrameBg] = ImVec4(0.21f, 0.18f, 0.25f, 1.00f);              // Crust
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.20f, 0.29f, 1.00f);       // Overlay1
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.22f, 0.31f, 1.00f);        // Overlay2
        colors[ImGuiCol_TitleBg] = ImVec4(0.14f, 0.12f, 0.18f, 1.00f);              // Mantle
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.17f, 0.15f, 0.21f, 1.00f);        // Mantle
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.14f, 0.12f, 0.18f, 1.00f);     // Mantle
        colors[ImGuiCol_MenuBarBg] = ImVec4(0.17f, 0.15f, 0.22f, 1.00f);            // Base
        colors[ImGuiCol_ScrollbarBg] = ImVec4(0.17f, 0.14f, 0.20f, 1.00f);          // Base
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.21f, 0.18f, 0.25f, 1.00f);        // Crust
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.24f, 0.20f, 0.29f, 1.00f); // Overlay1
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.26f, 0.22f, 0.31f, 1.00f);  // Overlay2
        colors[ImGuiCol_CheckMark] = ImVec4(0.95f, 0.66f, 0.47f, 1.00f);            // Peach
        colors[ImGuiCol_SliderGrab] = ImVec4(0.82f, 0.61f, 0.85f, 1.00f);           // Lavender
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.89f, 0.54f, 0.79f, 1.00f);     // Pink
        colors[ImGuiCol_Button] = ImVec4(0.65f, 0.34f, 0.46f, 1.00f);               // Maroon
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.71f, 0.40f, 0.52f, 1.00f);        // Red
        colors[ImGuiCol_ButtonActive] = ImVec4(0.76f, 0.46f, 0.58f, 1.00f);         // Pink
        colors[ImGuiCol_Header] = ImVec4(0.65f, 0.34f, 0.46f, 1.00f);               // Maroon
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.71f, 0.40f, 0.52f, 1.00f);        // Red
        colors[ImGuiCol_HeaderActive] = ImVec4(0.76f, 0.46f, 0.58f, 1.00f);         // Pink
        colors[ImGuiCol_Separator] = ImVec4(0.27f, 0.23f, 0.29f, 1.00f);            // Overlay0
        colors[ImGuiCol_SeparatorHovered] = ImVec4(0.95f, 0.66f, 0.47f, 1.00f);     // Peach
        colors[ImGuiCol_SeparatorActive] = ImVec4(0.95f, 0.66f, 0.47f, 1.00f);      // Peach
        colors[ImGuiCol_ResizeGrip] = ImVec4(0.82f, 0.61f, 0.85f, 1.00f);           // Lavender
        colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.89f, 0.54f, 0.79f, 1.00f);    // Pink
        colors[ImGuiCol_ResizeGripActive] = ImVec4(0.92f, 0.61f, 0.85f, 1.00f);     // Mauve
        colors[ImGuiCol_Tab] = ImVec4(0.21f, 0.18f, 0.25f, 1.00f);                  // Crust
        colors[ImGuiCol_TabHovered] = ImVec4(0.82f, 0.61f, 0.85f, 1.00f);           // Lavender
        colors[ImGuiCol_TabActive] = ImVec4(0.76f, 0.46f, 0.58f, 1.00f);            // Pink
        colors[ImGuiCol_TabUnfocused] = ImVec4(0.18f, 0.16f, 0.22f, 1.00f);         // Mantle
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.21f, 0.18f, 0.25f, 1.00f);   // Crust
        colors[ImGuiCol_DockingPreview] = ImVec4(0.95f, 0.66f, 0.47f, 0.70f);       // Peach
        colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);       // Base
        colors[ImGuiCol_PlotLines] = ImVec4(0.82f, 0.61f, 0.85f, 1.00f);            // Lavender
        colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.89f, 0.54f, 0.79f, 1.00f);     // Pink
        colors[ImGuiCol_PlotHistogram] = ImVec4(0.82f, 0.61f, 0.85f, 1.00f);        // Lavender
        colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.89f, 0.54f, 0.79f, 1.00f); // Pink
        colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);        // Mantle
        colors[ImGuiCol_TableBorderStrong] = ImVec4(0.27f, 0.23f, 0.29f, 1.00f);    // Overlay0
        colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);     // Surface2
        colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);  // Surface0
        colors[ImGuiCol_TextSelectedBg] = ImVec4(0.82f, 0.61f, 0.85f, 0.35f); // Lavender
        colors[ImGuiCol_DragDropTarget] = ImVec4(0.95f, 0.66f, 0.47f, 0.90f); // Peach
        colors[ImGuiCol_NavHighlight] = ImVec4(0.82f, 0.61f, 0.85f, 1.00f);   // Lavender
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

        // Style adjustments
        style.WindowRounding = 6.0f;
        style.FrameRounding = 4.0f;
        style.ScrollbarRounding = 4.0f;
        style.GrabRounding = 3.0f;
        style.ChildRounding = 4.0f;

        style.WindowTitleAlign = ImVec2(0.50f, 0.50f);
        style.WindowPadding = ImVec2(8.0f, 8.0f);
        style.FramePadding = ImVec2(5.0f, 4.0f);
        style.ItemSpacing = ImVec2(6.0f, 6.0f);
        style.ItemInnerSpacing = ImVec2(6.0f, 6.0f);
        style.IndentSpacing = 22.0f;

        style.ScrollbarSize = 14.0f;
        style.GrabMinSize = 10.0f;

        style.AntiAliasedLines = true;
        style.AntiAliasedFill = true;
}
void THEME_ModernColors() {
  ImGuiStyle &style = ImGui::GetStyle();
  ImVec4 *colors = style.Colors;

  // Base color scheme
  colors[ImGuiCol_Text] = ImVec4(0.92f, 0.92f, 0.92f, 1.00f);
  colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
  colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
  colors[ImGuiCol_ChildBg] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
  colors[ImGuiCol_PopupBg] = ImVec4(0.10f, 0.10f, 0.11f, 0.94f);
  colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
  colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
  colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.21f, 0.22f, 1.00f);
  colors[ImGuiCol_FrameBgHovered] = ImVec4(0.25f, 0.26f, 0.27f, 1.00f);
  colors[ImGuiCol_FrameBgActive] = ImVec4(0.18f, 0.19f, 0.20f, 1.00f);
  colors[ImGuiCol_TitleBg] = ImVec4(0.15f, 0.15f, 0.16f, 1.00f);
  colors[ImGuiCol_TitleBgActive] = ImVec4(0.15f, 0.15f, 0.16f, 1.00f);
  colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.15f, 0.15f, 0.16f, 1.00f);
  colors[ImGuiCol_MenuBarBg] = ImVec4(0.20f, 0.20f, 0.21f, 1.00f);
  colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.21f, 0.22f, 1.00f);
  colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.28f, 0.28f, 0.29f, 1.00f);
  colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.33f, 0.34f, 0.35f, 1.00f);
  colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.40f, 0.40f, 0.41f, 1.00f);
  colors[ImGuiCol_CheckMark] = ImVec4(0.76f, 0.76f, 0.76f, 1.00f);
  colors[ImGuiCol_SliderGrab] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
  colors[ImGuiCol_SliderGrabActive] = ImVec4(0.37f, 0.61f, 1.00f, 1.00f);
  colors[ImGuiCol_Button] = ImVec4(0.20f, 0.25f, 0.30f, 1.00f);
  colors[ImGuiCol_ButtonHovered] = ImVec4(0.30f, 0.35f, 0.40f, 1.00f);
  colors[ImGuiCol_ButtonActive] = ImVec4(0.25f, 0.30f, 0.35f, 1.00f);
  colors[ImGuiCol_Header] = ImVec4(0.25f, 0.25f, 0.25f, 0.80f);
  colors[ImGuiCol_HeaderHovered] = ImVec4(0.30f, 0.30f, 0.30f, 0.80f);
  colors[ImGuiCol_HeaderActive] = ImVec4(0.35f, 0.35f, 0.35f, 0.80f);
  colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
  colors[ImGuiCol_SeparatorHovered] = ImVec4(0.33f, 0.67f, 1.00f, 1.00f);
  colors[ImGuiCol_SeparatorActive] = ImVec4(0.33f, 0.67f, 1.00f, 1.00f);
  colors[ImGuiCol_ResizeGrip] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
  colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.37f, 0.61f, 1.00f, 1.00f);
  colors[ImGuiCol_ResizeGripActive] = ImVec4(0.37f, 0.61f, 1.00f, 1.00f);
  colors[ImGuiCol_Tab] = ImVec4(0.15f, 0.18f, 0.22f, 1.00f);
  colors[ImGuiCol_TabHovered] = ImVec4(0.38f, 0.48f, 0.69f, 1.00f);
  colors[ImGuiCol_TabActive] = ImVec4(0.28f, 0.38f, 0.59f, 1.00f);
  colors[ImGuiCol_TabUnfocused] = ImVec4(0.15f, 0.18f, 0.22f, 1.00f);
  colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.15f, 0.18f, 0.22f, 1.00f);
  colors[ImGuiCol_DockingPreview] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
  colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
  colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
  colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
  colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
  colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
  colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
  colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
  colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
  colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
  colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
  colors[ImGuiCol_TextSelectedBg] = ImVec4(0.28f, 0.56f, 1.00f, 0.35f);
  colors[ImGuiCol_DragDropTarget] = ImVec4(0.28f, 0.56f, 1.00f, 0.90f);
  colors[ImGuiCol_NavHighlight] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
  colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
  colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
  colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

  // Style adjustments
  style.WindowRounding = 5.3f;
  style.FrameRounding = 2.3f;
  style.ScrollbarRounding = 0;

  style.WindowTitleAlign = ImVec2(0.50f, 0.50f);
  style.WindowPadding = ImVec2(8.0f, 8.0f);
  style.FramePadding = ImVec2(5.0f, 5.0f);
  style.ItemSpacing = ImVec2(6.0f, 6.0f);
  style.ItemInnerSpacing = ImVec2(6.0f, 6.0f);
  style.IndentSpacing = 25.0f;
    
}
void THEME_GlassTheme() {
  ImGuiStyle &style = ImGui::GetStyle();
  ImVec4 *colors = style.Colors;

  // Setting up a dark theme base
  colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1f, 0.1f, 0.6f); // Semi-transparent dark background
  colors[ImGuiCol_ChildBg] = ImVec4(0.1f, 0.1f, 0.1f, 0.4f);
  colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.8f);
  colors[ImGuiCol_Border] = ImVec4(0.8f, 0.8f, 0.8f, 0.2f);

  // Text and frames
  colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
  colors[ImGuiCol_FrameBg] = ImVec4(0.2f, 0.2f, 0.2f, 0.5f); // Semi-transparent for frosted look
  colors[ImGuiCol_FrameBgHovered] = ImVec4(0.3f, 0.3f, 0.3f, 0.7f);
  colors[ImGuiCol_FrameBgActive] = ImVec4(0.3f, 0.3f, 0.3f, 0.9f);

  // Header
  colors[ImGuiCol_Header] = ImVec4(0.3f, 0.3f, 0.3f, 0.7f);
  colors[ImGuiCol_HeaderHovered] = ImVec4(0.4f, 0.4f, 0.4f, 0.8f);
  colors[ImGuiCol_HeaderActive] = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);

  // Buttons
  colors[ImGuiCol_Button] = ImVec4(0.3f, 0.3f, 0.3f, 0.6f);
  colors[ImGuiCol_ButtonHovered] = ImVec4(0.4f, 0.4f, 0.4f, 0.8f);
  colors[ImGuiCol_ButtonActive] = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);

  // Adjust window rounding and padding to enhance the glass look
  style.WindowRounding = 10.0f;
  style.FrameRounding = 5.0f;
  style.WindowPadding = ImVec2(10, 10);
  style.FramePadding = ImVec2(5, 5);
}
void THEME_DarkThemeColors() {
  auto &colors = ImGui::GetStyle().Colors;
  colors[ImGuiCol_WindowBg] = ImVec4{0.1f, 0.105f, 0.11f, 1.0f};

  // Headers
  colors[ImGuiCol_Header] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
  colors[ImGuiCol_HeaderHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
  colors[ImGuiCol_HeaderActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

  // Buttons
  colors[ImGuiCol_Button] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
  colors[ImGuiCol_ButtonHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
  colors[ImGuiCol_ButtonActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

  // Frame BG
  colors[ImGuiCol_FrameBg] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
  colors[ImGuiCol_FrameBgHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
  colors[ImGuiCol_FrameBgActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

  // Tabs
  colors[ImGuiCol_Tab] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
  colors[ImGuiCol_TabHovered] = ImVec4{0.38f, 0.3805f, 0.381f, 1.0f};
  colors[ImGuiCol_TabActive] = ImVec4{0.28f, 0.2805f, 0.281f, 1.0f};
  colors[ImGuiCol_TabUnfocused] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
  colors[ImGuiCol_TabUnfocusedActive] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};

  // Title
  colors[ImGuiCol_TitleBg] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
  colors[ImGuiCol_TitleBgActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
  colors[ImGuiCol_TitleBgCollapsed] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
}
void THEME_BessDarkThemeColors() {
  ImGuiStyle &style = ImGui::GetStyle();
  ImVec4 *colors = style.Colors;

  // Base colors for a pleasant and modern dark theme with dark accents
  colors[ImGuiCol_Text] = ImVec4(0.92f, 0.93f, 0.94f, 1.00f);                  // Light grey text for readability
  colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.52f, 0.54f, 1.00f);          // Subtle grey for disabled text
  colors[ImGuiCol_WindowBg] = ImVec4(0.14f, 0.14f, 0.16f, 1.00f);              // Dark background with a hint of blue
  colors[ImGuiCol_ChildBg] = ImVec4(0.16f, 0.16f, 0.18f, 1.00f);               // Slightly lighter for child elements
  colors[ImGuiCol_PopupBg] = ImVec4(0.18f, 0.18f, 0.20f, 1.00f);               // Popup background
  colors[ImGuiCol_Border] = ImVec4(0.28f, 0.29f, 0.30f, 0.60f);                // Soft border color
  colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);          // No border shadow
  colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.22f, 0.24f, 1.00f);               // Frame background
  colors[ImGuiCol_FrameBgHovered] = ImVec4(0.22f, 0.24f, 0.26f, 1.00f);        // Frame hover effect
  colors[ImGuiCol_FrameBgActive] = ImVec4(0.24f, 0.26f, 0.28f, 1.00f);         // Active frame background
  colors[ImGuiCol_TitleBg] = ImVec4(0.14f, 0.14f, 0.16f, 1.00f);               // Title background
  colors[ImGuiCol_TitleBgActive] = ImVec4(0.16f, 0.16f, 0.18f, 1.00f);         // Active title background
  colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.14f, 0.14f, 0.16f, 1.00f);      // Collapsed title background
  colors[ImGuiCol_MenuBarBg] = ImVec4(0.20f, 0.20f, 0.22f, 1.00f);             // Menu bar background
  colors[ImGuiCol_ScrollbarBg] = ImVec4(0.16f, 0.16f, 0.18f, 1.00f);           // Scrollbar background
  colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.24f, 0.26f, 0.28f, 1.00f);         // Dark accent for scrollbar grab
  colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.28f, 0.30f, 0.32f, 1.00f);  // Scrollbar grab hover
  colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.32f, 0.34f, 0.36f, 1.00f);   // Scrollbar grab active
  colors[ImGuiCol_CheckMark] = ImVec4(0.46f, 0.56f, 0.66f, 1.00f);             // Dark blue checkmark
  colors[ImGuiCol_SliderGrab] = ImVec4(0.36f, 0.46f, 0.56f, 1.00f);            // Dark blue slider grab
  colors[ImGuiCol_SliderGrabActive] = ImVec4(0.40f, 0.50f, 0.60f, 1.00f);      // Active slider grab
  colors[ImGuiCol_Button] = ImVec4(0.24f, 0.34f, 0.44f, 1.00f);                // Dark blue button
  colors[ImGuiCol_ButtonHovered] = ImVec4(0.28f, 0.38f, 0.48f, 1.00f);         // Button hover effect
  colors[ImGuiCol_ButtonActive] = ImVec4(0.32f, 0.42f, 0.52f, 1.00f);          // Active button
  colors[ImGuiCol_Header] = ImVec4(0.24f, 0.34f, 0.44f, 1.00f);                // Header color similar to button
  colors[ImGuiCol_HeaderHovered] = ImVec4(0.28f, 0.38f, 0.48f, 1.00f);         // Header hover effect
  colors[ImGuiCol_HeaderActive] = ImVec4(0.32f, 0.42f, 0.52f, 1.00f);          // Active header
  colors[ImGuiCol_Separator] = ImVec4(0.28f, 0.29f, 0.30f, 1.00f);             // Separator color
  colors[ImGuiCol_SeparatorHovered] = ImVec4(0.46f, 0.56f, 0.66f, 1.00f);      // Hover effect for separator
  colors[ImGuiCol_SeparatorActive] = ImVec4(0.46f, 0.56f, 0.66f, 1.00f);       // Active separator
  colors[ImGuiCol_ResizeGrip] = ImVec4(0.36f, 0.46f, 0.56f, 1.00f);            // Resize grip
  colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.40f, 0.50f, 0.60f, 1.00f);     // Hover effect for resize grip
  colors[ImGuiCol_ResizeGripActive] = ImVec4(0.44f, 0.54f, 0.64f, 1.00f);      // Active resize grip
  colors[ImGuiCol_Tab] = ImVec4(0.20f, 0.22f, 0.24f, 1.00f);                   // Inactive tab
  colors[ImGuiCol_TabHovered] = ImVec4(0.28f, 0.38f, 0.48f, 1.00f);            // Hover effect for tab
  colors[ImGuiCol_TabActive] = ImVec4(0.24f, 0.34f, 0.44f, 1.00f);             // Active tab color
  colors[ImGuiCol_TabUnfocused] = ImVec4(0.20f, 0.22f, 0.24f, 1.00f);          // Unfocused tab
  colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.24f, 0.34f, 0.44f, 1.00f);    // Active but unfocused tab
  colors[ImGuiCol_DockingPreview] = ImVec4(0.24f, 0.34f, 0.44f, 0.70f);        // Docking preview
  colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.14f, 0.14f, 0.16f, 1.00f);        // Empty docking background
  colors[ImGuiCol_PlotLines] = ImVec4(0.46f, 0.56f, 0.66f, 1.00f);             // Plot lines
  colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.46f, 0.56f, 0.66f, 1.00f);      // Hover effect for plot lines
  colors[ImGuiCol_PlotHistogram] = ImVec4(0.36f, 0.46f, 0.56f, 1.00f);         // Histogram color
  colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.40f, 0.50f, 0.60f, 1.00f);  // Hover effect for histogram
  colors[ImGuiCol_TableHeaderBg] = ImVec4(0.20f, 0.22f, 0.24f, 1.00f);         // Table header background
  colors[ImGuiCol_TableBorderStrong] = ImVec4(0.28f, 0.29f, 0.30f, 1.00f);     // Strong border for tables
  colors[ImGuiCol_TableBorderLight] = ImVec4(0.24f, 0.25f, 0.26f, 1.00f);      // Light border for tables
  colors[ImGuiCol_TableRowBg] = ImVec4(0.20f, 0.22f, 0.24f, 1.00f);            // Table row background
  colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.22f, 0.24f, 0.26f, 1.00f);         // Alternate row background
  colors[ImGuiCol_TextSelectedBg] = ImVec4(0.24f, 0.34f, 0.44f, 0.35f);        // Selected text background
  colors[ImGuiCol_DragDropTarget] = ImVec4(0.46f, 0.56f, 0.66f, 0.90f);        // Drag and drop target
  colors[ImGuiCol_NavHighlight] = ImVec4(0.46f, 0.56f, 0.66f, 1.00f);          // Navigation highlight
  colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f); // Windowing highlight
  colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);     // Dim background for windowing
  colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);      // Dim background for modal windows

  // Style adjustments
  style.WindowRounding = 8.0f;    // Softer rounded corners for windows
  style.FrameRounding = 4.0f;     // Rounded corners for frames
  style.ScrollbarRounding = 6.0f; // Rounded corners for scrollbars
  style.GrabRounding = 4.0f;      // Rounded corners for grab elements
  style.ChildRounding = 4.0f;     // Rounded corners for child windows

  style.WindowTitleAlign = ImVec2(0.50f, 0.50f); // Centered window title
  style.WindowPadding = ImVec2(10.0f, 10.0f);    // Comfortable padding
  style.FramePadding = ImVec2(6.0f, 4.0f);       // Frame padding
  style.ItemSpacing = ImVec2(8.0f, 8.0f);        // Item spacing
  style.ItemInnerSpacing = ImVec2(8.0f, 6.0f);   // Inner item spacing
  style.IndentSpacing = 22.0f;                   // Indentation spacing

  style.ScrollbarSize = 16.0f; // Scrollbar size
  style.GrabMinSize = 10.0f;   // Minimum grab size

  style.AntiAliasedLines = true; // Enable anti-aliased lines
  style.AntiAliasedFill = true;  // Enable anti-aliased fill
}
void THEME_setFluentUITheme() {
  ImGuiStyle &style = ImGui::GetStyle();
  ImVec4 *colors = style.Colors;

  // General window settings
  style.WindowRounding = 5.0f;
  style.FrameRounding = 5.0f;
  style.ScrollbarRounding = 5.0f;
  style.GrabRounding = 5.0f;
  style.TabRounding = 5.0f;
  style.WindowBorderSize = 1.0f;
  style.FrameBorderSize = 1.0f;
  style.PopupBorderSize = 1.0f;
  style.PopupRounding = 5.0f;

  // Setting the colors
  colors[ImGuiCol_Text] = ImVec4(0.95f, 0.95f, 0.95f, 1.00f);
  colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
  colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.13f, 0.13f, 1.00f);
  colors[ImGuiCol_ChildBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
  colors[ImGuiCol_PopupBg] = ImVec4(0.18f, 0.18f, 0.18f, 1.f);
  colors[ImGuiCol_Border] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
  colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
  colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
  colors[ImGuiCol_FrameBgHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
  colors[ImGuiCol_FrameBgActive] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
  colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
  colors[ImGuiCol_TitleBgActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
  colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
  colors[ImGuiCol_MenuBarBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
  colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
  colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
  colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
  colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);

  // Accent colors changed to darker olive-green/grey shades
  colors[ImGuiCol_CheckMark] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);        // Dark gray for check marks
  colors[ImGuiCol_SliderGrab] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);       // Dark gray for sliders
  colors[ImGuiCol_SliderGrabActive] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f); // Slightly lighter gray when active
  colors[ImGuiCol_Button] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);           // Button background (dark gray)
  colors[ImGuiCol_ButtonHovered] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);    // Button hover state
  colors[ImGuiCol_ButtonActive] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);     // Button active state
  colors[ImGuiCol_Header] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);           // Dark gray for menu headers
  colors[ImGuiCol_HeaderHovered] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);    // Slightly lighter on hover
  colors[ImGuiCol_HeaderActive] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);     // Lighter gray when active
  colors[ImGuiCol_Separator] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);        // Separators in dark gray
  colors[ImGuiCol_SeparatorHovered] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
  colors[ImGuiCol_SeparatorActive] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
  colors[ImGuiCol_ResizeGrip] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f); // Resize grips in dark gray
  colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
  colors[ImGuiCol_ResizeGripActive] = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);
  colors[ImGuiCol_Tab] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);        // Tabs background
  colors[ImGuiCol_TabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f); // Darker gray on hover
  colors[ImGuiCol_TabActive] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
  colors[ImGuiCol_TabUnfocused] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
  colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
  colors[ImGuiCol_DockingPreview] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f); // Docking preview in gray
  colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f); // Empty dock background
  // Additional styles
  style.FramePadding = ImVec2(8.0f, 4.0f);
  style.ItemSpacing = ImVec2(8.0f, 4.0f);
  style.IndentSpacing = 20.0f;
  style.ScrollbarSize = 16.0f;
}
void THEME_FluentUILightTheme() {
  ImGuiStyle &style = ImGui::GetStyle();
  ImVec4 *colors = style.Colors;

  // General window settings
  style.WindowRounding = 5.0f;
  style.FrameRounding = 5.0f;
  style.ScrollbarRounding = 5.0f;
  style.GrabRounding = 5.0f;
  style.TabRounding = 5.0f;
  style.WindowBorderSize = 1.0f;
  style.FrameBorderSize = 1.0f;
  style.PopupBorderSize = 1.0f;
  style.PopupRounding = 5.0f;

  // Setting the colors (Light version)
  colors[ImGuiCol_Text] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
  colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
  colors[ImGuiCol_WindowBg] = ImVec4(0.95f, 0.95f, 0.95f, 1.00f); // Light background
  colors[ImGuiCol_ChildBg] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
  colors[ImGuiCol_PopupBg] = ImVec4(0.98f, 0.98f, 0.98f, 1.00f);
  colors[ImGuiCol_Border] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
  colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
  colors[ImGuiCol_FrameBg] = ImVec4(0.85f, 0.85f, 0.85f, 1.00f); // Light frame background
  colors[ImGuiCol_FrameBgHovered] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);
  colors[ImGuiCol_FrameBgActive] = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
  colors[ImGuiCol_TitleBg] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
  colors[ImGuiCol_TitleBgActive] = ImVec4(0.85f, 0.85f, 0.85f, 1.00f);
  colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
  colors[ImGuiCol_MenuBarBg] = ImVec4(0.95f, 0.95f, 0.95f, 1.00f);
  colors[ImGuiCol_ScrollbarBg] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
  colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);
  colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
  colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);

  // Accent colors with a soft pastel gray-green
  colors[ImGuiCol_CheckMark] = ImVec4(0.55f, 0.65f, 0.55f, 1.00f); // Soft gray-green for check marks
  colors[ImGuiCol_SliderGrab] = ImVec4(0.55f, 0.65f, 0.55f, 1.00f);
  colors[ImGuiCol_SliderGrabActive] = ImVec4(0.60f, 0.70f, 0.60f, 1.00f);
  colors[ImGuiCol_Button] = ImVec4(0.85f, 0.85f, 0.85f, 1.00f); // Light button background
  colors[ImGuiCol_ButtonHovered] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);
  colors[ImGuiCol_ButtonActive] = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
  colors[ImGuiCol_Header] = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
  colors[ImGuiCol_HeaderHovered] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
  colors[ImGuiCol_HeaderActive] = ImVec4(0.65f, 0.65f, 0.65f, 1.00f);
  colors[ImGuiCol_Separator] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
  colors[ImGuiCol_SeparatorHovered] = ImVec4(0.65f, 0.65f, 0.65f, 1.00f);
  colors[ImGuiCol_SeparatorActive] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
  colors[ImGuiCol_ResizeGrip] = ImVec4(0.55f, 0.65f, 0.55f, 1.00f); // Accent color for resize grips
  colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.60f, 0.70f, 0.60f, 1.00f);
  colors[ImGuiCol_ResizeGripActive] = ImVec4(0.65f, 0.75f, 0.65f, 1.00f);
  colors[ImGuiCol_Tab] = ImVec4(0.85f, 0.85f, 0.85f, 1.00f); // Tabs background
  colors[ImGuiCol_TabHovered] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);
  colors[ImGuiCol_TabActive] = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
  colors[ImGuiCol_TabUnfocused] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
  colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
  colors[ImGuiCol_DockingPreview] = ImVec4(0.55f, 0.65f, 0.55f, 1.00f); // Docking preview in gray-green
  colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);

  // Additional styles
  style.FramePadding = ImVec2(8.0f, 4.0f);
  style.ItemSpacing = ImVec2(8.0f, 4.0f);
  style.IndentSpacing = 20.0f;
  style.ScrollbarSize = 16.0f;
}
bool Application::Init() {
  
  if (!glfwInit()) {
    printf("problem with GLFW\n");
    return false;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  m_NativeWindow = glfwCreateWindow(m_WindowData.width, m_WindowData.height,
                                    m_WindowData.title, NULL, NULL);
  m_NodeManager.SetGLFWWindow(m_NativeWindow);

  if (m_NativeWindow == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return false;
  }

  glfwMakeContextCurrent(m_NativeWindow);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return false;
  }

  glfwSetWindowUserPointer(m_NativeWindow, &m_ApplicationData);


  m_NodeManager.InitGLFWEvents();
  m_NodeManager.InitIcons();
  m_NodeManager.AddIcon("grid", "mesher_resources/icons/grid.png");
  m_NodeManager.AddIcon("tube", "mesher_resources/icons/tube.png");

  m_NodeManager.SetFileExtension("ney");
  glViewport(0, 0, m_WindowData.width, m_WindowData.height);
  ImGuiInit(m_NativeWindow);

  m_NodeManager.CreateAllNodes();
  // THEME_ModernColors();
  // THEME_CatpuccinMochaColors();
  THEME_DarkThemeColors();

  glfwSwapInterval(0);

  return true;
}

void Application::ImGuiInit(GLFWwindow *window) {

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  
  m_RegularFont = io.Fonts->AddFontFromFileTTF("resources/fonts/JetBrainsMono-Regular.ttf", 16);
  m_BoldFont = io.Fonts->AddFontFromFileTTF("resources/fonts/JetBrainsMono-ExtraBold.ttf", 16);
  io.Fonts->Build();
  io.FontDefault = m_RegularFont;
  m_NodeManager.SetFonts(m_RegularFont, m_BoldFont);
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;


  /* not sure why I need to do that now. But it avoid a ImGui crash complaining about io.DisplaySize values of 0*/
  io.DisplaySize.x = 1280.0f;
  io.DisplaySize.y = 720.0f;

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  const char *glsl_version = "#version 330";
  ImGui_ImplOpenGL3_Init(glsl_version);

}

void Application::ImGuiBeginFrame() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  ImGui::DockSpaceOverViewport(
      NULL, NULL,
      ImGuiDockNodeFlags_None |
          ImGuiDockNodeFlags_PassthruCentralNode /*|ImGuiDockNodeFlags_NoResize*/);
}

void Application::ImGuiEndFrame() {
  ImGui::Render();

  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  ImGui::EndFrame();

  ImGuiIO &io = ImGui::GetIO();

  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    GLFWwindow *backup_current_context = glfwGetCurrentContext();

    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();

    glfwMakeContextCurrent(backup_current_context);
  }
}

void Application::Run() {
  while (!glfwWindowShouldClose(m_NativeWindow)) {

    glfwWaitEvents();

    ImGuiBeginFrame();

    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    static bool showDemoWindow = false;
    if (showDemoWindow) {
      ImGui::ShowDemoWindow(&showDemoWindow);
    }

    ImGui::BeginMainMenuBar();
     if (ImGui::BeginMenu("File")) {
      if (ImGui::MenuItem("New", "Ctrl+N")) {
        m_NodeManager.GetRootNetwork().nodes.clear();
        m_NodeManager.GetRootNetwork().outuput_node = nullptr;
        m_NodeManager.m_SavePath = std::filesystem::path("");
        glfwSetWindowTitle(m_NodeManager.GetGLFWWindow(), m_NodeManager.m_SavePath.string().c_str());
      }
      if (ImGui::MenuItem("Save", "Ctrl+S")) {

        m_NodeManager.SaveAll();

      }
      if( ImGui::MenuItem("Load", "Ctrl+O")) {
        m_NodeManager.LoadAll();
      }
      ImGui::Separator();
      if(ImGui::MenuItem("Clear All Nodes")) {
        m_NodeManager.GetNodes().clear();
      }

      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("View")) {

      if (ImGui::MenuItem("Center All", "F")) {
        m_NodeManager.ViewFrameAll();
      }
      ImGui::MenuItem("Show Grid", NULL, &m_NodeManager.m_ViewProps.display_grid);
      ImGui::MenuItem("Show Mouse Coords", NULL, &m_NodeManager.m_ViewProps.show_mouse_coords);
      ImGui::Separator();
      if(ImGui::MenuItem("Goto Root")) {
        m_NodeManager.GotoRootNetwork();
      }
      ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();

    m_NodeManager.DisplayNodeParams(m_NodeManager.m_CurrentNode);
    
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    static bool first_opened = true;

    ImGui::Begin("Canvas test");

    m_NodeManager.DrawCanvas();
    if (first_opened) {
      m_NodeManager.ViewFrameAll();
      first_opened = false;
    }
    ImGui::End();
    ImGui::PopStyleVar();

    ImGuiEndFrame();

    glfwSwapBuffers(m_NativeWindow);

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
}

void Application::ExportTempMesh() {

  fs::path path = fs::temp_directory_path() / "temp_mesh.fbx";
  MeshExporter me;
  auto mesh_op = std::dynamic_pointer_cast<NED::MeshOperator>(m_NodeManager.GetOutputNode());
  auto openmesh_op = std::dynamic_pointer_cast<NED::ImGuiNode<GMesh>>(m_NodeManager.GetOutputNode());
  if( mesh_op != nullptr){
    me.MakeScene(mesh_op->m_DataCache);
    me.Export(path.string().c_str());
  }else if(openmesh_op != nullptr){
    me.MakeScene(openmesh_op->m_DataCache);
    me.Export(path.string().c_str());
  }

}

}; // namespace NED