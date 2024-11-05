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
  ImGuiInit(m_NativeWindow);

  glViewport(0, 0, m_WindowData.width, m_WindowData.height);
  glfwSwapInterval(0);

  return true;
}

// void Application::InitEvents() {
//   using namespace NodeEditor;
//   static auto &dispatcher = NodeEditor::EventManager::GetInstance();
// }

void Application::ImGuiInit(GLFWwindow *window) {
  // init ImGui
  // Setup Dear ImGui context

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
  // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  const char *glsl_version = "#version 330";
  ImGui_ImplOpenGL3_Init(glsl_version);

  ////////////
  // end imgui config
  ///////////
}

void Application::ImGuiBeginFrame() {
  // Start the Dear ImGui frame
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
        // std::cout << "New file Not Implemented Yet" << std::endl;

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

  fs::path path = fs::temp_directory_path() / "temp_mesh.ply";
  MeshExporter me;
  auto op = static_cast<NodeEditor::MeshOperator *>(m_NodeManager.GetOutputNode().get());
  me.MakeScene(op->m_DataCache);
  me.ExportPLY(path.string().c_str());

}

}; // namespace NodeEditor