#include "pch.h"

// #include "meshop/MeshGenerator.h"
#include "meshop/generators/SquareGenerator.h"
#include "meshop/generators/GridGenerator.h"
#include "meshop/modifiers/NormalModifier.h"
#include "MeshImporter.h"
#include "MeshExporter.h"
#include "OperatorNetwork.h"

#include "utils.h"

#include "gltf_utils.h"
#include "Mesh.h"

using namespace msh;
namespace fs = std::filesystem;

void export_temp_mesh(Mesh& mesh);
int main() {

    OperatorNetwork net; 

    std::shared_ptr<SquareGenerator> square = std::make_shared<SquareGenerator>();
    square->setName("Square Operator");
    net.addOperator(square);
    
    std::shared_ptr<NormalModifier> normal_mod = std::make_shared<NormalModifier>();
    normal_mod->setName("Normal Modifier");
    normal_mod->setInput(0, square);
    net.addOperator(normal_mod);

    net.evaluate();

    Mesh result = normal_mod->mMeshCache;
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