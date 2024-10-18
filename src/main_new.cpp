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

    // auto tube = meshutils::generateTube(1.0f, 1.0f, 1.0f, 25, 2);
    // meshutils::fusePoints(tube, 0.005f);

    // return 0;
    REGISTER_NODE_TYPE(NodeEditor::GridGenerator, "Grid", "Generators");
    REGISTER_NODE_TYPE(NodeEditor::TubeGenerator, "Tube", "Generators");
    REGISTER_NODE_TYPE(NodeEditor::SquareGenerator, "Square", "Generators");

    REGISTER_NODE_TYPE(NodeEditor::NormalModifier, "Normal", "Modifiers");
    REGISTER_NODE_TYPE(NodeEditor::TransformModifier, "Transform", "Modifiers");
    REGISTER_NODE_TYPE(NodeEditor::NoiseDisplaceModifier, "Noise Displace", "Modifiers");
    REGISTER_NODE_TYPE(NodeEditor::MeshMerger, "Merge", "Modifiers");
    REGISTER_NODE_TYPE(NodeEditor::MeshMergerMulti, "Merge Multi", "Modifiers");
    REGISTER_NODE_TYPE(NodeEditor::MeshTwister, "Twister", "Modifiers");
    REGISTER_NODE_TYPE(NodeEditor::MeshCenter, "Center", "Modifiers");

    REGISTER_NODE_TYPE(NodeEditor::NullMeshOperator, "Null", "Utility");

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
 
    manager.Evaluate();
    app.ExportTempMesh();
    app.Run();

    std::cout << "__ALL_DONE__ " << std::endl;
    
    return 0;
}