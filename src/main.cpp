#include "cpp_mesher.h"
// #include "meshop/generators/SquareGenerator.h"
// #include "meshop/generators/GridGenerator.h"
// #include "meshop/generators/CylinderGenerator.h"
// #include "meshop/modifiers/NormalModifier.h"
// #include "meshop/modifiers/TransformModifier.h"
// #include "OperatorNetwork.h"

#include "MeshImporter.h"
#include "MeshExporter.h"
#include "utils.h"
#include "mesh_utils.h"

#include "gltf_utils.h"
#include "Mesh.h"
#include "formatters.h"

#include "node_editor.h"
#include "nodes/MeshOperators.h"

using namespace msh;
using namespace NodeEditor;
MeshExporter me;
void export_temp_mesh(Mesh& mesh){
    fs::path path = fs::temp_directory_path() / "temp_mesh.ply";

    me.MakeScene(mesh);
    me.ExportPLY(path.string().c_str());
}
int main() {

    Log::Init();

    Application app;
    if(!app.Init()){
        std::cout << "Problem !!!" << std::endl;
        
        return -1;
    }

    auto& manager = app.GetNodeManager();


    auto null_node = std::make_shared<Node<NullMeshOperator>>("Null");
    null_node->position = ImVec2(650, 600);
    manager.AddNode(null_node);

    manager.SetNodesMenu([&manager](){
      if (ImGui::MenuItem("Square", NULL, false, true)) {
        manager.AddNode(std::make_shared<Node<SquareGenerator>>("Square"));
      }
      if (ImGui::MenuItem("Tube", NULL, false, true)) {
        manager.AddNode(std::make_shared<Node<TubeGenerator>>("Tube"));
      }
      if (ImGui::MenuItem("Merge", NULL, false, true)) {
        manager.AddNode(
            std::make_shared<Node<MeshMerger>>("Merge"));
      }
      if (ImGui::MenuItem("Repeater", NULL, false, true)) {
        manager.AddNode(std::make_shared<Node<GridGenerator>>("Grid"));
      }
    });

    EventManager::GetInstance().Subscribe(
      EventType::NodeConnection, [&app, &null_node](const Event &event) {
        auto &manager = app.GetNodeManager();
        manager.Evaluate();
        auto op = static_cast<MeshOperator *>(manager.GetOutputNode().get());
        std::cout << "Connection Update -> " << op->m_MeshCache << std::endl;
        export_temp_mesh(op->m_MeshCache);
      });

    manager.SetOutputNode(null_node);
    app.GetNodeManager().Evaluate();
    app.Run();

    // OperatorNetwork net; 
    // Ref<GridGenerator> grid = MakeRef<GridGenerator>(5.0f, 5.0f, 50, 10);
    
    // grid->setName("Grid Operator");
    // Ref<CylinderGenerator> cylinder = MakeRef<CylinderGenerator>();
    // cylinder->setName("Cylinder Operator");
    // cylinder->mRadius = 2.0f;
    // cylinder->mHeight = 5.0f;

    // net.addOperator(cylinder);
    
    // Ref<NormalModifier> normal_mod = MakeRef<NormalModifier>();
    // normal_mod->setName("Normal Modifier");
    // normal_mod->setInput(0, net.getOperators()[0]);
    // net.addOperator(normal_mod);


    // Ref<TransformModifier> transform_mod = MakeRef<TransformModifier>();
    // transform_mod->setName("Transform Modifier");
    // transform_mod->setInput(0, net.getOperators()[1]);

    // transform_mod->mTransformOrder = meshutils::TRANSFORM_ORDER::TRS;
    // transform_mod->mRot = glm::vec3(0.0f, PI / 2.0f, 0.0f);

    // net.addOperator(transform_mod);
    // net.evaluate();

    // Mesh result = net.getOperators()[net.getOperators().size() - 1]->mMeshCache;


    // result = meshutils::generateTube2(1.0f, 5.0f, 40, 5);
    // result.ComputeNormals();

    // LOG_INFO("result : {}", result);

    // export_temp_mesh(result);

    std::cout << "All Done !!" << std::endl;
    
    return 0;
}

