#include <iostream>

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <imgui.h>

#include "Mesh.h"
#include "MeshExporter.h"
// #include "MeshImporter.h"

#include "node_editor.h"
#include "nodes/MeshOperators.h"
// #include "nodes/node_params.h"

#include "Log.h"
#include "cpp_mesher.h"

#include "Application.h"


using namespace msh;
using namespace NodeEditor;
void add_example_nodes(NodeManager &manager);
int main(int argc, char *argv[]) {
  std::filesystem::path file_to_load = "";
  std::filesystem::path exe_path = argv[0];

#ifdef _WIN32
  SetCurrentDirectoryA(exe_path.parent_path().string().c_str());
#endif
  if(argc > 1){
    file_to_load = argv[1];
  }
    Log::Init();

    auto grid = meshutils::generateGrid(1.0f, 1.0f, 1, 1);
    auto subd = meshutils::subdivide(grid);

    std::cout << subd << std::endl;
    
    // return 0;

    REGISTER_NODE_TYPE(NodeEditor::GridGenerator, "Grid", "Generators");
    REGISTER_NODE_TYPE(NodeEditor::TubeGenerator, "Tube", "Generators");
    REGISTER_NODE_TYPE(NodeEditor::SquareGenerator, "Square", "Generators");
    REGISTER_NODE_TYPE(NodeEditor::MeshFileLoader, "File Loader", "Generators");

    REGISTER_NODE_TYPE(NodeEditor::NormalModifier, "Normal", "Modifiers");
    REGISTER_NODE_TYPE(NodeEditor::TransformModifier, "Transform", "Modifiers");
    REGISTER_NODE_TYPE(NodeEditor::NoiseDisplaceModifier, "Noise Displace", "Modifiers");
    REGISTER_NODE_TYPE(NodeEditor::MeshMerger, "Merge", "Modifiers");
    REGISTER_NODE_TYPE(NodeEditor::MeshMergerMulti, "Merge Multi", "Modifiers");
    REGISTER_NODE_TYPE(NodeEditor::MeshTwister, "Twister", "Modifiers");
    REGISTER_NODE_TYPE(NodeEditor::MeshCenter, "Center", "Modifiers");
    REGISTER_NODE_TYPE(NodeEditor::FusePoints, "Fuse Points", "Modifiers");

    REGISTER_NODE_TYPE(NodeEditor::NullMeshOperator, "Null", "Utility");
    REGISTER_NODE_TYPE(NodeEditor::MeshSubnetOperator, "Subnet", "Utility");
    CREATE_SUBNET_INPUT_NODE_CLASS(msh::Mesh, "Subnet input", "Utility");

    msh::Application app;
    app.Init();

    NodeManager& manager = app.GetNodeManager();


  // add_example_nodes(manager);

  static EventDispatcher& dispatcher = EventManager::GetInstance();

  dispatcher.Subscribe(
      EventType::NodeConnection, [&app](const Event &event) {
        auto &manager = app.GetNodeManager();
        manager.Evaluate();
        if(manager.GetOutputNode() != nullptr){
          auto op = static_cast<MeshOperator *>(manager.GetOutputNode().get());
          std::cout << "Connection Update -> " << op->m_DataCache << std::endl;
          dispatcher.Dispatch(ManagerUpdateEvent());
        }
      });
  dispatcher.Subscribe(
      EventType::ParamChanged, [&app](const Event &event) {
        auto &manager = app.GetNodeManager();
        manager.m_OneParamChanged = true;
      });
  dispatcher.Subscribe(
      EventType::ManagerUpdate, [&app](const Event &event) {
        auto &manager = app.GetNodeManager();
        manager.Evaluate();
        if(manager.GetOutputNode() != nullptr){
          auto subnet_op = std::dynamic_pointer_cast<MeshSubnetOperator>(manager.GetOutputNode());
          auto subnet_input_op = std::dynamic_pointer_cast<SubnetInputNode<msh::Mesh>>(manager.GetOutputNode());
          auto op = std::dynamic_pointer_cast<ImGuiNode<msh::Mesh>>(manager.GetOutputNode());
          if(subnet_op != nullptr){
            if(subnet_op->node_network.outuput_node != nullptr){
              auto output_op = std::dynamic_pointer_cast<ImGuiNode<msh::Mesh>>(subnet_op->node_network.outuput_node);
              std::cout << "Want Subnet Data Cache !!!!!!!!" << std::endl;
              std::cout << "m_DataCache -> " << output_op->m_DataCache << std::endl;
              
            }
          }else if(op != nullptr){
            std::cout << "ManagerUpdate Event -> " << op->m_DataCache << std::endl;
            app.ExportTempMesh();
          }else if(subnet_input_op != nullptr){
            auto op2 = static_cast<ImGuiNode<msh::Mesh>*>(subnet_input_op->parent_node->GetInput(0).get());
            std::cout << "Subnet input Operator -> " <<  op2->m_DataCache << std::endl;
            
          }else{
            std::cout << "can't convert to Operator" << std::endl;
          }
        }

      });

    if(file_to_load.empty() == false){
      app.GetNodeManager().LoadFromFile(file_to_load);
    }
    app.Run();

    std::cout << "__ALL_DONE__ " << std::endl;
    
    return 0;
}

void add_example_nodes(NodeManager &manager){

    auto grid_node = std::make_shared<Node<GridGenerator>>("Grid");
    grid_node->position = ImVec2(0,0);
    grid_node->width->value = 1.0f;
    grid_node->length->value = 1.0f;
    grid_node->cols->value = 100;
    grid_node->rows->value = 100;
    manager.AddNode(grid_node);

    auto noise_node = std::make_shared<Node<NoiseDisplaceModifier>>("Noise Displace");
    noise_node->position = grid_node->position + ImVec2(0, 100);
    noise_node->SetInput(0, grid_node);
    manager.AddNode(noise_node);

    auto normal_node = std::make_shared<Node<NormalModifier>>("Normal");
    normal_node->position = noise_node->position + ImVec2(0, 100);
    normal_node->SetInput(0, noise_node);
    manager.AddNode(normal_node);

    auto output_node = std::make_shared<Node<NullMeshOperator>>("Null");
    output_node->position = normal_node->position + ImVec2(0, 100);
    output_node->SetInput(0, normal_node);
    manager.AddNode(output_node);

    manager.SetOutputNode(output_node);
}