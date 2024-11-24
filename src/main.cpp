#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <imgui.h>

#include <iostream>

#include "Mesh.h"
#include "MeshExporter.h"


#include "node_editor.h"
#include "nodes/MeshOperators.h"
#include "openmesh/openmesh_operators.h"




#include "Application.h"
#include "Log.h"
#include "cpp_mesher.h"
#include "formatters.h"

#include <OpenMesh/Core/IO/MeshIO.hh>

using namespace msh;
using namespace NED;

void save_openmesh_result(OMesh &mesh) {
  fs::path path = fs::temp_directory_path() / "temp_mesh.ply";

  std::cout << "mesh.has_vertex_texcoords2D()" << mesh.has_vertex_texcoords2D() << std::endl;
  
  MeshExporter me;
  me.MakeScene(mesh);
  me.ExportPLY(path.string().c_str());
  // OpenMesh::IO::Options options;
  // options += OpenMesh::IO::Options::VertexTexCoord;
  // if(!OpenMesh::IO::write_mesh(mesh, path.string().c_str())){
  //   std::cout << "Problem writing file" << std::endl;
    
  // }
}

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

  REGISTER_NODE_TYPE(NED::OpenMeshCubeGenerator, "Cube", "OMesh_generators");
  REGISTER_NODE_TYPE(NED::OpenMeshSquareGenerator, "Square", "OMesh_generators");

  msh::Application app;
  app.Init();

  NodeManager &manager = app.GetNodeManager();

  // add_example_nodes(manager);

  static EventDispatcher &dispatcher = EventManager::GetInstance();

  dispatcher.Subscribe(EventType::NodeConnection, [&app](const Event &event) {
    auto &manager = app.GetNodeManager();
    manager.Evaluate();
    if (manager.GetOutputNode() != nullptr) {
      auto op = static_cast<MeshOperator *>(manager.GetOutputNode().get());
      // std::cout << "Connection Update -> " << op->m_DataCache << std::endl;
      dispatcher.Dispatch(ManagerUpdateEvent());
    }
  });
  dispatcher.Subscribe(EventType::ParamChanged, [&app](const Event &event) {
    auto &manager = app.GetNodeManager();
    manager.m_OneParamChanged = true;
  });
  dispatcher.Subscribe(EventType::ManagerUpdate, [&app](const Event &event) {
    auto &manager = app.GetNodeManager();
    manager.Evaluate();
    if (manager.GetOutputNode() != nullptr) {
      auto subnet_op = std::dynamic_pointer_cast<MeshSubnetOperator>(manager.GetOutputNode());
      auto subnet_input_op = std::dynamic_pointer_cast<SubnetInputNode<msh::Mesh>>(manager.GetOutputNode());
      auto op = std::dynamic_pointer_cast<ImGuiNode<msh::Mesh>>(manager.GetOutputNode());
      auto openmesh_op = std::dynamic_pointer_cast<ImGuiNode<OMesh>>(manager.GetOutputNode());
      if (subnet_op != nullptr) {
        if (subnet_op->node_network.outuput_node != nullptr) {
          auto output_op = std::dynamic_pointer_cast<ImGuiNode<msh::Mesh>>(subnet_op->node_network.outuput_node);
          std::cout << "Want Subnet Data Cache !!!!!!!!" << std::endl;
          // std::cout << "m_DataCache -> " << output_op->m_DataCache << std::endl;
        }
      } else if (op != nullptr) {
        // std::cout << "ManagerUpdate Event -> " << op->m_DataCache << std::endl;
        LOG_INFO("ManagerUpdate Event -> {}", op->m_DataCache);
        app.ExportTempMesh();
      } else if (subnet_input_op != nullptr) {
        auto op2 = static_cast<ImGuiNode<msh::Mesh> *>(subnet_input_op->parent_node->GetInput(0).get());
        // std::cout << "Subnet input Operator -> " <<  op2->m_DataCache << std::endl;

      } else if (openmesh_op != nullptr) {
        auto mesh = openmesh_op->m_DataCache;
        save_openmesh_result(mesh);
        auto vertices = mesh.vertices();
        for(auto& v : vertices) { 
          OMesh::Point pt = mesh.point(v);
          std::cout << v << std::endl;
        }
        
        // std::cout << "Subnet input Operator -> " <<  op2->m_DataCache << std::endl;

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