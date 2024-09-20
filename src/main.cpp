#include "pch.h"
#include <memory>
#include "meshop/MeshGenerator.h"
#include "meshop/generators/SquareGenerator.h"
#include "meshop/generators/GridGenerator.h"
#include "MeshImporter.h"
#include "MeshExporter.h"
#include "OperatorNetwork.h"

#include "utils.h"

#include "gltf_utils.h"

#include <spdlog/fmt/ostr.h>

using namespace msh;
// using namespace tinygltf;
namespace fs = std::filesystem;

void export_temp_mesh(Mesh& mesh);
int main() {
    Log::Init();
    // MeshImporter* mi = MeshImporter::GetInstance();
    // MeshExporter me;
    // std::string Filename = "C:/gui2one/3D/houdini_20_playground/geo/box_corner.glb";
    
    

    // Mesh mesh = mi->Import(Filename.c_str());
    // std::cout << mesh << std::endl;

    OperatorNetwork net; 
    std::shared_ptr<MeshGenerator> square = std::make_shared<SquareGenerator>();
    
    square->setName("Square Operator");
    square->update();
    net.addOperator(square);
    std::shared_ptr<MeshGenerator> grid = std::make_shared<GridGenerator>();
    grid->setName("Grid Operator");
    grid->update();
    net.addOperator(grid);


    Mesh result = square->mMeshCache;
    net.evaluate();
    export_temp_mesh(result);

    std::cout << "Result: "<< result << std::endl;
    return 0;
}

void export_temp_mesh(Mesh& mesh){
    fs::path path = fs::temp_directory_path() / "temp_mesh.ply";
    MeshExporter me;
    me.MakeScene(mesh);
    me.ExportPLY(path.string().c_str());
}