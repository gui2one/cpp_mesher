#include <iostream>

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <imgui.h>

#include "Mesh.h"
#include "MeshExporter.h"

#include "node_editor.h"
#include "nodes/MeshOperators.h"
// #include "nodes/node_params.h"

#include "Log.h"
#include "cpp_mesher.h"

#include "Application.h"

using namespace msh;
using namespace NodeEditor;

int main(){

    Log::Init();

    msh::Application app;
    app.Init();

    NodeManager& manager = app.GetNodeManager();

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
    // manager.SetNodesMenu([&manager](){
    //     if(ImGui::BeginMenu("Generators")){
    //       node_menu_item<Node<SquareGenerator>>(manager,"Square");
    //       node_menu_item<Node<GridGenerator>>(manager,"Grid");
    //       node_menu_item<Node<TubeGenerator>>(manager,"Tube");
    //       ImGui::EndMenu();
    //     }

    //     if(ImGui::BeginMenu("Modifiers")){
    //       node_menu_item<Node<NormalModifier>>(manager,"Normal");
    //       node_menu_item<Node<TransformModifier>>(manager,"Transform");
    //       node_menu_item<Node<NoiseDisplaceModifier>>(manager,"Noise Displace");
    //       node_menu_item<Node<MeshMerger>>(manager,"Merge");
    //       ImGui::EndMenu();
    //     }
    //     node_menu_item<Node<NullMeshOperator>>(manager,"Null");
    // });
    
 
    manager.Evaluate();
    app.ExportTempMesh();
    app.Run();

    std::cout << "__ALL_DONE__ " << std::endl;
    
    return 0;
}