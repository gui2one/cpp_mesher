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
void show_mesh_detail();
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

  NodeManager &manager = app.GetNodeManager();
  manager.ParamChangeSubscribe<NED::FloatRamp>();

  manager.CreateAllNodes();

  app.UserFunction([&]() {
    show_mesh_detail();
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
  ImGui::Begin("GMesh detail");

  if (ImGui::BeginTable("Mesh detail", 2, ImGuiTableFlags_Borders)) {
    ImGui::TableNextRow();
    ImGui::TableNextColumn();
    ImGui::Text("Vertices");
    ImGui::Text("Faces");
    ImGui::TableNextColumn();

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

    if (ImGui::BeginTable("Vertices Details", num_cols, ImGuiTableFlags_Borders)) {
      ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed);
      ImGui::TableSetupColumn("pos[0]", ImGuiTableColumnFlags_WidthFixed);
      ImGui::TableSetupColumn("pos[1]", ImGuiTableColumnFlags_WidthFixed);
      ImGui::TableSetupColumn("pos[2]", ImGuiTableColumnFlags_WidthFixed);

      for (auto &prop : OUTPUT_MESH.vertex_props) {
        if (strcmp(prop.type_name, "vec3f") == 0) {
          std::string name0 = std::format("{}[0]", prop.name);
          ImGui::TableSetupColumn(name0.c_str(), ImGuiTableColumnFlags_WidthFixed);
          std::string name1 = std::format("{}[1]", prop.name);
          ImGui::TableSetupColumn(name1.c_str(), ImGuiTableColumnFlags_WidthFixed);
          std::string name2 = std::format("{}[2]", prop.name);
          ImGui::TableSetupColumn(name2.c_str(), ImGuiTableColumnFlags_WidthFixed);
        } else if (strcmp(prop.type_name, "vec2f") == 0) {
          std::string name0 = std::format("{}[0]", prop.name);
          ImGui::TableSetupColumn(name0.c_str(), ImGuiTableColumnFlags_WidthFixed);
          std::string name1 = std::format("{}[1]", prop.name);
          ImGui::TableSetupColumn(name1.c_str(), ImGuiTableColumnFlags_WidthFixed);
        } else if (strcmp(prop.type_name, "float") == 0 || strcmp(prop.type_name, "int") == 0) {
          ImGui::TableSetupColumn(prop.name.c_str(), ImGuiTableColumnFlags_WidthFixed);
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
          ImGui::Text("%.3f", pt[0]);
          ImGui::TableNextColumn();
          ImGui::Text("%.3f", pt[1]);
          ImGui::TableNextColumn();
          ImGui::Text("%.3f", pt[2]);

          for (auto &prop : OUTPUT_MESH.vertex_props) {
            if (strcmp(prop.type_name, "int") == 0) {
              ImGui::TableNextColumn();
              ImGui::Text("%d", vh.idx());
            } else if (strcmp(prop.type_name, "float") == 0) {
              ImGui::TableNextColumn();
              ImGui::Text("%.3f", 3.14f);
            } else if (strcmp(prop.type_name, "vec3f") == 0) {
              auto prop_value =
                  OUTPUT_MESH.property(std::get<OpenMesh::VPropHandleT<OpenMesh::Vec3f>>(prop.handle), vh);
              ImGui::TableNextColumn();
              ImGui::Text("%.3f", prop_value[0]);
              ImGui::TableNextColumn();
              ImGui::Text("%.3f", prop_value[1]);
              ImGui::TableNextColumn();
              ImGui::Text("%.3f", prop_value[2]);
            }
          }
        }
      }
      ImGui::EndTable();
    }
    ImGui::EndTable();
  }

  ImGui::End();
}

std::string gmesh_tostring(GMesh &gmesh) {
  std::stringstream ss;
  ss << gmesh.n_vertices() << " Vertices\n" << gmesh.n_faces() << " Faces";
  return ss.str();
}