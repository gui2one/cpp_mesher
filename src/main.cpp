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
#include "openmesh/openmesh_operators.h"
using namespace msh;
using namespace NED;

void show_mesh_info();
std::string gmesh_tostring(GMesh &gmesh);

GMesh OUTPUT_MESH = GMesh();
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

  // REGISTER_NODE_TYPE(NED::GridGenerator, "Grid", "Generators");
  // REGISTER_NODE_TYPE(NED::TubeGenerator, "Tube", "Generators");
  // REGISTER_NODE_TYPE(NED::TorusGenerator, "Torus", "Generators");
  // REGISTER_NODE_TYPE(NED::SquareGenerator, "Square", "Generators");
  // REGISTER_NODE_TYPE(NED::MeshFileLoader, "File Loader", "Generators");

  // REGISTER_NODE_TYPE(NED::NormalModifier, "Normal", "Modifiers");
  // REGISTER_NODE_TYPE(NED::TransformModifier, "Transform", "Modifiers");
  // REGISTER_NODE_TYPE(NED::NoiseDisplaceModifier, "Noise Displace", "Modifiers");
  // REGISTER_NODE_TYPE(NED::MeshMerger, "Merge", "Modifiers");
  // REGISTER_NODE_TYPE(NED::MeshMergerMulti, "Merge Multi", "Modifiers");
  // REGISTER_NODE_TYPE(NED::MeshTwister, "Twister", "Modifiers");
  // REGISTER_NODE_TYPE(NED::MeshCenter, "Center", "Modifiers");
  // REGISTER_NODE_TYPE(NED::FusePoints, "Fuse Points", "Modifiers");
  // REGISTER_NODE_TYPE(NED::MeshSubdivide, "Subdivide", "Modifiers");
  // REGISTER_NODE_TYPE(NED::MeshTriangulate, "Triangulate", "Modifiers");
  // REGISTER_NODE_TYPE(NED::MeshDuplicate, "Duplicate", "Modifiers");

  // REGISTER_NODE_TYPE(NED::NullMeshOperator, "Null", "Utility");
  // REGISTER_NODE_TYPE(NED::MeshSubnetOperator, "Subnet", "Utility");
  // CREATE_SUBNET_INPUT_NODE_CLASS(msh::Mesh, "Subnet input", "Utility");

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

  // register custom params
  REGISTER_PARAM_TYPE(NED::ParamFloatRamp);
  msh::Application app;
  if (!app.Init()) {
    std::cout << "App Init() Error ..." << std::endl;
    return -1;
  };

  NodeManager &manager = app.GetNodeManager();
  manager.ParamChangeSubscribe<NED::FloatRamp>();

  manager.CreateAllNodes();

  app.UserFunction([&]() {
    // ImGui::ShowDemoWindow();
    ImGui::Begin("user window");
    for (auto node : app.GetNodeManager().GetNodes()) {
      ImGui::Text("%s", node->title.c_str());
    }
    ImGui::End();

    show_mesh_info();
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
        auto mesh = openmesh_op->m_DataCache;
        OUTPUT_MESH = mesh;
        LOG_INFO("{}", mesh);

        openmeshutils::list_vertex_properties(mesh);
        app.ExportTempMesh();
      } else {
        std::cout << "can't convert to Operator" << std::endl;
      }
    }
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
    const char *name = std::format("Vertex properties ({})", OUTPUT_MESH.vertex_props.size()).c_str();
    ImGui::Indent(15.0f);
    if (ImGui::TreeNodeEx(name, ImGuiTreeNodeFlags_DefaultOpen)) {
      for (auto prop : OUTPUT_MESH.vertex_props) {
        ui::green_text("%s", prop.name);
        ImGui::SameLine();
        ImGui::Text("(%s)", prop.type_name);
      }
      ImGui::TreePop();
    }
    ImGui::Separator();
  }
  ImGui::End();
}

std::string gmesh_tostring(GMesh &gmesh) {
  std::stringstream ss;
  ss << gmesh.n_vertices() << " Vertices\n" << gmesh.n_faces() << " Faces";
  return ss.str();
}