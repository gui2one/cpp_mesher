// #include "cpp_mesher.h"

// #include "MeshImporter.h"
// #include "MeshExporter.h"
// #include "utils.h"
// #include "mesh_utils.h"

// #include "gltf_utils.h"
// #include "Mesh.h"
// #include "formatters.h"

// #include "node_editor.h"
// #include "nodes/MeshOperators.h"
// // #include "nodes/node_params.h"



// using namespace msh;
// using namespace NodeEditor;

// void export_temp_mesh(Mesh& mesh){
//     fs::path path = fs::temp_directory_path() / "temp_mesh.ply";
//     MeshExporter me;
//     me.MakeScene(mesh);
//     me.ExportPLY(path.string().c_str());
// }

// int main() {


  
//   Log::Init();

//   Application app;
//   if(!app.Init()){
//       std::cout << "Problem !!!" << std::endl;
      
//       return -1;
//   }

//   auto& manager = app.GetNodeManager();


//   auto grid = std::make_shared<Node<GridGenerator>>("Grid");
//   grid->position = ImVec2(650, 500);
//   manager.AddNode(grid);
//   auto noise_node = std::make_shared<Node<NoiseDisplaceModifier>>("Noise Displace");
//   noise_node->position = grid->position + ImVec2(0, 100);
//   noise_node->SetInput(0, grid);
//   manager.AddNode(noise_node);
//   auto null_node = std::make_shared<Node<NullMeshOperator>>("Null");
//   null_node->position = noise_node->position + ImVec2(0, 100);
//   null_node->SetInput(0, noise_node);
//   manager.AddNode(null_node);

//   // manager.SetNodesMenu([&manager](){
//   //   if(ImGui::BeginMenu("Generators")){

//   //     node_menu_item<Node<SquareGenerator>>(manager,"Square");
//   //     node_menu_item<Node<GridGenerator>>(manager,"Grid");
//   //     node_menu_item<Node<TubeGenerator>>(manager,"Tube");
//   //     ImGui::EndMenu();
//   //   }

//   //   if(ImGui::BeginMenu("Modifiers")){
//   //     node_menu_item<Node<NormalModifier>>(manager,"Normal");
//   //     node_menu_item<Node<TransformModifier>>(manager,"Transform");
//   //     node_menu_item<Node<NoiseDisplaceModifier>>(manager,"Noise Displace");
//   //     ImGui::EndMenu();
//   //   }
//   //   node_menu_item<Node<MeshMerger>>(manager,"Merge");

//   // });

//   EventManager::GetInstance().Subscribe(
//     EventType::NodeConnection, [&app](const Event &event) {
//       auto &manager = app.GetNodeManager();
//       manager.Evaluate();
//       auto op = static_cast<MeshOperator *>(manager.GetOutputNode().get());
//       std::cout << "Connection Update -> " << op->m_MeshCache << std::endl;
//       export_temp_mesh(op->m_MeshCache);
//     });
//   EventManager::GetInstance().Subscribe(
//     EventType::ParamChanged, [&app](const Event &event) {
//       auto &manager = app.GetNodeManager();
//       manager.Evaluate();
//       auto op = static_cast<MeshOperator *>(manager.GetOutputNode().get());
//       std::cout << "ParamChanged Event -> " << op->m_MeshCache << std::endl;
//       export_temp_mesh(op->m_MeshCache);
//     });


//   manager.SetOutputNode(null_node);
//   app.GetNodeManager().Evaluate();
//   app.Run();

//   std::cout << "All Done !!" << std::endl;
  
//   return 0;
// }

