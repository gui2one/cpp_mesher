#define EXIT_ON_OPENGL_ERROR

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <imgui.h>

#include <OpenMesh/Core/IO/MeshIO.hh>
#include <iostream>

#include "Application.h"
#include "Log.h"
#include "Mesh.h"
#include "MeshExporter.h"
#include "cpp_mesher.h"
#include "formatters.h"
#include "imgui_utils.h"
#include "node_editor.h"
#include "nodes/MeshOperators.h"
#include "opengl_renderer/glanderer.h"
#include "openmesh/openmesh_operators.h"
using namespace msh;
using namespace NED;

void show_mesh_info();
void show_mesh_detail();

void init_renderer(Application &app);
void show_opengl_renderer();
std::string gmesh_tostring(GMesh &gmesh);

GMesh OUTPUT_MESH = GMesh();

std::shared_ptr<GLR::OpenGLRenderer> opengl_renderer;
std::shared_ptr<GLR::Camera> camera;
std::shared_ptr<GLR::Layer> main_layer;
GLR::Scene main_scene;
std::shared_ptr<GLR::SpotLight> main_light;

int main(int argc, char *argv[]) {
  std::filesystem::path file_to_load = "";
  std::filesystem::path exe_path = argv[0];

#ifdef _WIN32
  SetCurrentDirectoryA(exe_path.parent_path().string().c_str());
#endif
  if (argc > 1) {
    file_to_load = argv[1];
  }
  Log::Init();

  CREATE_UTILITY_CLASSES(GMesh, "OpenMesh/utils");
  REGISTER_NODE_TYPE(NED::OpenMeshCubeGenerator, "Cube", "OpenMesh/generators");
  REGISTER_NODE_TYPE(NED::OpenMeshSquareGenerator, "Square", "OpenMesh/generators");
  REGISTER_NODE_TYPE(NED::OpenMeshFileImport, "Load File", "OpenMesh/generators");
  REGISTER_NODE_TYPE(NED::OpenMeshGridGenerator, "Grid", "OpenMesh/generators");
  REGISTER_NODE_TYPE(NED::OpenMeshTorusGenerator, "Torus", "OpenMesh/generators");

  REGISTER_NODE_TYPE(NED::OpenMeshComputeNormals, "Compute Normals", "OpenMesh/modifiers");
  REGISTER_NODE_TYPE(NED::OpenMeshSetNormals, "Set Normals", "OpenMesh/modifiers");
  REGISTER_NODE_TYPE(NED::OpenMeshTriangulate, "Triangulate", "OpenMesh/modifiers");
  REGISTER_NODE_TYPE(NED::OpenMeshTransform, "Transform", "OpenMesh/modifiers");
  REGISTER_NODE_TYPE(NED::OpenMeshNoiseDisplace, "Noise Displace", "OpenMesh/modifiers");
  REGISTER_NODE_TYPE(NED::OpenMeshAddProperty, "Add Property", "OpenMesh/modifiers");
  REGISTER_NODE_TYPE(NED::OpenMeshSubdivide, "Subdivide", "OpenMesh/modifiers");
  REGISTER_NODE_TYPE(NED::OpenMeshCombine, "Combine", "OpenMesh/modifiers");
  REGISTER_NODE_TYPE(NED::OpenMeshDuplicate, "Duplicate", "OpenMesh/modifiers");

  // register custom params
  REGISTER_PARAM_TYPE(NED::ParamFloatRamp);

  msh::Application app;
  if (!app.Init()) {
    std::cout << "App Init() Error ..." << std::endl;
    return -1;
  };

  GLR::ShaderManager *shader_manager = GLR::ShaderManager::GetInstance();
  init_renderer(app);

  NodeManager &manager = app.GetNodeManager();
  manager.ParamChangeSubscribe<NED::FloatRamp>();

  manager.CreateAllNodes();

  app.UserFunction([&]() {
    show_mesh_detail();
    show_mesh_info();

    show_opengl_renderer();
  });

  manager.AddIcon("grid", "mesher_resources/icons/grid.png");
  manager.AddIcon("tube", "mesher_resources/icons/tube.png");
  manager.AddIcon("torus", "mesher_resources/icons/torus.png");
  manager.AddIcon("load_file", "mesher_resources/icons/load_file.png");

  static EventDispatcher &dispatcher = EventManager::GetInstance();

  dispatcher.Subscribe(EventType::ParamChanged, [&app](const Event &event) {
    const ManagerUpdateEvent ev = static_cast<const ManagerUpdateEvent &>(event);
    dispatcher.Dispatch(ev);
  });
  dispatcher.Subscribe(EventType::ManagerUpdate, [&](const Event &event) {
    auto &manager = app.GetNodeManager();
    manager.Evaluate();
    if (manager.GetOutputNode() != nullptr) {
      auto openmesh_op = std::dynamic_pointer_cast<ImGuiNode<GMesh>>(manager.GetOutputNode());
      if (openmesh_op != nullptr) {
        GMesh mesh = openmesh_op->m_DataCache;
        OUTPUT_MESH = mesh;
        LOG_INFO("{}", mesh);

        // openmeshutils::list_vertex_properties(mesh);
        app.ExportTempMesh();
      } else {
        std::cout << "can't convert to Operator" << std::endl;
      }
    }

    glfwPostEmptyEvent();
  });

  if (file_to_load.empty() == false) {
    app.GetNodeManager().LoadFromFile(file_to_load);
  }
  app.Run();

  std::cout << "__ALL_DONE__ " << std::endl;

  return 0;
}

void show_mesh_info() {
  ImGui::Begin("GMesh info");
  static bool geo_opened = true;
  ImGuiTreeNodeFlags flags = 0;
  flags |= ImGuiTreeNodeFlags_DefaultOpen;
  if (ImGui::CollapsingHeader("Geometry", flags)) {
    auto n_vertices = OUTPUT_MESH.n_vertices();
    auto n_faces = OUTPUT_MESH.n_faces();

    ImGui::Text("Vertices");
    ImGui::SameLine();
    ui::green_text("%d", n_vertices);

    ImGui::Text("Faces   ");
    ImGui::SameLine();
    ui::green_text("%d", n_faces);

    // ImGui::TreePop();
  }

  if (ImGui::CollapsingHeader("Properties", flags)) {
    std::string str = std::format("Vertex properties ({})", OUTPUT_MESH.vertex_props.size());
    const char *name = str.c_str();
    if (ImGui::TreeNodeEx(name, ImGuiTreeNodeFlags_DefaultOpen)) {
      ImGui::Indent(15.0f);
      for (auto prop : OUTPUT_MESH.vertex_props) {
        ui::green_text("%s", prop.name.c_str());
        ImGui::SameLine();
        ImGui::Text("(%s)", prop.type_name);
      }
      ImGui::TreePop();
    }
    ImGui::Separator();
  }
  ImGui::End();
}

void show_mesh_detail() {
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
  ImGui::Begin("GMesh detail");

  int num_cols = 4; /* 1 for id, 3 for position */
  for (size_t i = 0; i < OUTPUT_MESH.vertex_props.size(); i++) {
    auto prop = OUTPUT_MESH.vertex_props[i];
    if (strcmp(prop.type_name, "vec3f") == 0) {
      num_cols += 3;
    } else if (strcmp(prop.type_name, "vec2f") == 0) {
      num_cols += 2;
    } else if (strcmp(prop.type_name, "float") == 0 || strcmp(prop.type_name, "int") == 0) {
      num_cols += 1;
    }
  }

  ImGuiTableFlags table_flags = ImGuiTableFlags_Borders | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY;
  ImGuiTableColumnFlags column_flags = 0;
  column_flags |= ImGuiTableColumnFlags_WidthFixed;

  if (ImGui::BeginTable("Vertices Details", num_cols, table_flags)) {
    ImGui::TableSetupScrollFreeze(0, 1); /* always show headers */
    ImGui::TableSetupColumn("ID", column_flags);
    ImGui::TableSetupColumn("pos[0]", column_flags);
    ImGui::TableSetupColumn("pos[1]", column_flags);
    ImGui::TableSetupColumn("pos[2]", column_flags);

    for (auto &prop : OUTPUT_MESH.vertex_props) {
      if (strcmp(prop.type_name, "vec3f") == 0) {
        std::string name0 = std::format("{}[0]", prop.name);
        ImGui::TableSetupColumn(name0.c_str(), column_flags);
        std::string name1 = std::format("{}[1]", prop.name);
        ImGui::TableSetupColumn(name1.c_str(), column_flags);
        std::string name2 = std::format("{}[2]", prop.name);
        ImGui::TableSetupColumn(name2.c_str(), column_flags);
      } else if (strcmp(prop.type_name, "vec2f") == 0) {
        std::string name0 = std::format("{}[0]", prop.name);
        ImGui::TableSetupColumn(name0.c_str(), column_flags);
        std::string name1 = std::format("{}[1]", prop.name);
        ImGui::TableSetupColumn(name1.c_str(), column_flags);
      } else if (strcmp(prop.type_name, "float") == 0 || strcmp(prop.type_name, "int") == 0) {
        ImGui::TableSetupColumn(prop.name.c_str(), column_flags);
      }
    }
    ImGui::TableHeadersRow();
    ImGuiListClipper clipper;
    clipper.Begin((int)OUTPUT_MESH.n_vertices());
    while (clipper.Step()) {
      for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; row++) {
        ImGui::TableNextRow();
        auto vh = OUTPUT_MESH.vertex_handle(row);
        auto pt = OUTPUT_MESH.point(vh);
        ImGui::TableNextColumn();
        ImGui::Text("%d", vh.idx());
        ImGui::TableNextColumn();
        const char *pattern = (pt[0] >= 0.0f) ? " %.3f" : "%.3f";
        ImGui::Text(pattern, pt[0]);
        ImGui::TableNextColumn();
        pattern = (pt[1] >= 0.0f) ? " %.3f" : "%.3f";
        ImGui::Text(pattern, pt[1]);
        ImGui::TableNextColumn();
        pattern = (pt[2] >= 0.0f) ? " %.3f" : "%.3f";
        ImGui::Text(pattern, pt[2]);

        for (auto &prop : OUTPUT_MESH.vertex_props) {
          if (strcmp(prop.type_name, "int") == 0) {
            auto prop_value = OUTPUT_MESH.GetVertexPropValue<int>(prop, vh);
            ImGui::TableNextColumn();
            ImGui::Text("%d", prop_value);
          } else if (strcmp(prop.type_name, "float") == 0) {
            auto prop_value = OUTPUT_MESH.GetVertexPropValue<float>(prop, vh);
            ImGui::TableNextColumn();
            ImGui::Text("%.3f", prop_value);
          } else if (strcmp(prop.type_name, "vec2f") == 0) {
            auto prop_value = OUTPUT_MESH.GetVertexPropValue<OpenMesh::Vec2f>(prop, vh);
            ImGui::TableNextColumn();
            const char *pattern = (prop_value[0] >= 0.0f) ? " %.3f" : "%.3f";
            ImGui::Text(pattern, prop_value[0]);
            ImGui::TableNextColumn();
            pattern = (prop_value[1] >= 0.0f) ? " %.3f" : "%.3f";
            ImGui::Text(pattern, prop_value[1]);
          } else if (strcmp(prop.type_name, "vec3f") == 0) {
            auto prop_value = OUTPUT_MESH.GetVertexPropValue<OpenMesh::Vec3f>(prop, vh);
            ImGui::TableNextColumn();
            const char *pattern = (prop_value[0] >= 0.0f) ? " %.3f" : "%.3f";
            ImGui::Text(pattern, prop_value[0]);
            ImGui::TableNextColumn();
            pattern = (prop_value[1] >= 0.0f) ? " %.3f" : "%.3f";
            ImGui::Text(pattern, prop_value[1]);
            ImGui::TableNextColumn();
            pattern = (prop_value[2] >= 0.0f) ? " %.3f" : "%.3f";
            ImGui::Text(pattern, prop_value[2]);
          }
        }
      }
    }
    ImGui::EndTable();
  }

  ImGui::End();
  ImGui::PopStyleVar();
}

void init_renderer(Application &app) {
  opengl_renderer = std::make_shared<GLR::OpenGLRenderer>();
  opengl_renderer->Init();

  main_layer = GLR::Layer::CreateStandardLayout();
  // main_scene = std::make_shared<GLR::Scene>();
  camera = std::make_shared<GLR::Camera>(glm::radians(60.0f), 1.0f);
  camera->SetPosition(glm::vec3(0.f, 3.f, 5.f));

  int w, h;
  glfwGetWindowSize(app.GetNativeWindow(), &w, &h);
  camera->SetScreenRatio((float)w / (float)h);
  camera->SetNear(0.1f);
  camera->SetFar(100.0f);
  main_scene.Add(camera);

  auto mesh_test = GLR::MeshUtils::MakeQuadSphere(1.0f);
  auto box_obj = std::make_shared<GLR::MeshObject>();
  box_obj->m_Material = opengl_renderer->GetDefaultMaterial();
  box_obj->SetMesh(std::make_shared<GLR::Mesh>(mesh_test));
  box_obj->InitRenderData();

  main_scene.Add(box_obj);

  glm::vec3 spot_pos = glm::vec3(8.0f, 5.0f, -8.0f);
  glm::vec3 target_pos = glm::vec3(0.0f, 0.0f, 0.0f);
  float spot_angle = 45.0f;
  float spot_intensity = 300.0f;
  main_light = std::make_shared<GLR::SpotLight>();
  main_light->SetPosition(spot_pos);
  main_light->SetTargetPosition(target_pos);
  main_light->m_ShadowBias = 0.001f;
  main_light->m_Intensity = spot_intensity;
  main_light->m_Color = glm::vec3(1.0f, 1.0f, 1.0f);
  main_light->SetAngle(spot_angle);
  main_scene.Add(main_light);
}
void show_opengl_renderer() {
  ImGui::Begin("OpenGL Renderer");
  ImVec2 avail_size = ImGui::GetContentRegionAvail();

  main_layer->SetSize((uint32_t)avail_size.x, (uint32_t)avail_size.y);
  camera->SetScreenRatio((float)avail_size.x / (float)avail_size.y);
  opengl_renderer->Render(main_layer, main_scene, camera);

  auto &shader = GLR::ShaderManager::GetInstance()->screen_shader;
  shader->UseProgram();

  ImGui::Image((ImTextureID)(intptr_t)main_layer->GetColorAttachementID(), avail_size, ImVec2(0, 1), ImVec2(1, 0));
  ImGui::End();

  glUseProgram(0);
}
std::string gmesh_tostring(GMesh &gmesh) {
  std::stringstream ss;
  ss << gmesh.n_vertices() << " Vertices\n" << gmesh.n_faces() << " Faces";
  return ss.str();
}