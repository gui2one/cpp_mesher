#include <iostream>

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <imgui.h>

#include "Mesh.h"
#include "MeshExporter.h"

#include "node_editor.h"
#include "nodes/MeshOperators.h"
#include "nodes/node_params.h"

#include "Log.h"
#include "cpp_mesher.h"
using namespace msh;
using namespace NodeEditor;

void export_temp_mesh(Mesh& mesh){
    fs::path path = fs::temp_directory_path() / "temp_mesh.ply";
    MeshExporter me;
    me.MakeScene(mesh);
    me.ExportPLY(path.string().c_str());
}


void ImGuiInit(GLFWwindow *window) {
  // init ImGui
  // Setup Dear ImGui context

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  
//   auto m_RegularFont = io.Fonts->AddFontFromFileTTF("resources/fonts/JetBrainsMono-Regular.ttf", 16);
//   auto m_BoldFont = io.Fonts->AddFontFromFileTTF("resources/fonts/JetBrainsMono-ExtraBold.ttf", 16);
//   io.Fonts->Build();
//   m_NodeManager.SetFonts(m_RegularFont, m_BoldFont);
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  const char *glsl_version = "#version 330";
  ImGui_ImplOpenGL3_Init(glsl_version);

  ////////////
  // end imgui config
  ///////////
}
void init_events(GLFWwindow* win, NodeManager &node_manager) {
  static auto &dispatcher = EventManager::GetInstance();
  glfwSetMouseButtonCallback(
      win, [](GLFWwindow *window, int button, int action, int mods) {

        // LOG_INFO("button: {} action: {}", button, action);
        
        if (action == GLFW_PRESS) {
          MouseClickEvent clickEvent(button);
          dispatcher.Dispatch(clickEvent);
        } else if (action == GLFW_RELEASE) {
          MouseReleaseEvent releaseEvent(button);
          dispatcher.Dispatch(releaseEvent);
        }
    });

  glfwSetCursorPosCallback(win,
                           [](GLFWwindow *window, double xpos, double ypos) {

                             MouseMoveEvent moveEvent((float)xpos, (float)ypos);
                             dispatcher.Dispatch(moveEvent);
                           });

  glfwSetKeyCallback(win, [](GLFWwindow *window, int key,
                                        int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
      KeyPressEvent pressEvent(key);
      dispatcher.Dispatch(pressEvent);
    }
  });

  glfwSetFramebufferSizeCallback(
      win, [](GLFWwindow *window, int width, int height) {
        glViewport(0, 0, width, height);
      });

  // add event listeners !!!
  dispatcher.Subscribe(EventType::MouseClick,
                       [&node_manager](const NodeEditor::Event &event) {
                         node_manager.OnMouseClick(event);
                       });
  dispatcher.Subscribe(EventType::MouseRelease,
                       [&node_manager](const NodeEditor::Event &event) {
                         node_manager.OnMouseRelease(event);
                       });
  dispatcher.Subscribe(EventType::MouseMove,
                       [&node_manager](const NodeEditor::Event &event) {
                         node_manager.OnMouseMove(event);
                       });
  dispatcher.Subscribe(EventType::KeyPress,
                       [&node_manager](const NodeEditor::Event &event) {
                         node_manager.OnKeyPress(event);
                       }); 

  dispatcher.Subscribe(
    EventType::NodeConnection, [&node_manager](const Event &event) {
    //   auto &manager = app.GetNodeManager();
      node_manager.Evaluate();
      auto op = static_cast<MeshOperator *>(node_manager.GetOutputNode().get());
      std::cout << "Connection Update -> " << op->m_MeshCache << std::endl;
      export_temp_mesh(op->m_MeshCache);
    });
  dispatcher.Subscribe(
    EventType::ParamChanged, [&node_manager](const Event &event) {

      node_manager.Evaluate();
      auto op = static_cast<MeshOperator *>(node_manager.GetOutputNode().get());
      std::cout << "ParamChanged Event -> " << op->m_MeshCache << std::endl;
      export_temp_mesh(op->m_MeshCache);
    });                                
}
void ImGuiBeginFrame() {
  // Start the Dear ImGui frame
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

//   ImGui::PushFont(m_RegularFont);

  ImGui::DockSpaceOverViewport(
      NULL, NULL,
      ImGuiDockNodeFlags_None |
          ImGuiDockNodeFlags_PassthruCentralNode /*|ImGuiDockNodeFlags_NoResize*/);
}
void ImGuiEndFrame() {
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

int main(){

    Log::Init();
    NodeEditor::NodeManager manager;

    auto null_node = std::make_shared<Node<NullMeshOperator>>("Null");
    null_node->position = ImVec2(500,100);
    manager.AddNode(null_node);
    manager.SetOutputNode(null_node);
    manager.SetNodesMenu([&manager](){
        if(ImGui::BeginMenu("Generators")){

        node_menu_item<Node<SquareGenerator>>(manager,"Square");
        node_menu_item<Node<GridGenerator>>(manager,"Grid");
        node_menu_item<Node<TubeGenerator>>(manager,"Tube");
        ImGui::EndMenu();
        }

        if(ImGui::BeginMenu("Modifiers")){
        node_menu_item<Node<NormalModifier>>(manager,"Normal");
        node_menu_item<Node<TransformModifier>>(manager,"Transform");
        node_menu_item<Node<NoiseDisplaceModifier>>(manager,"Noise Displace");
        ImGui::EndMenu();
        }
        node_menu_item<Node<MeshMerger>>(manager,"Merge");

    });    
    std::cout << "Main New !!!!" << std::endl;
    if(!glfwInit()){
        std::cout << "GLFW init failed" << std::endl;
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window =  glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    
    if(!window){
        std::cout << "GLFW window init failed" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    manager.SetGLFWWindow(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_CAPTURED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    init_events(window, manager);
    ImGuiInit(window);


    glViewport(0, 0, 640, 480);
    glfwSwapInterval(0);

    while(!glfwWindowShouldClose(window)){
        glfwWaitEvents();
        ImGuiBeginFrame();
        // glViewport(0, 0, 640, 480);
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        static bool showDemoWindow = false;
        if (showDemoWindow) {
        ImGui::ShowDemoWindow(&showDemoWindow);
        }

        manager.DisplayNodeParams(manager.m_CurrentNode);
        
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("Canvas test");
        manager.DrawCanvas();

        ImGui::End();        
        ImGui::PopStyleVar();
        ImGuiEndFrame();
        glfwSwapBuffers(window);

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    glfwTerminate();

    std::cout << "__ALL_DONE__ " << std::endl;
    
    return 0;
}