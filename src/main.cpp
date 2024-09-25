#include "meshop/generators/SquareGenerator.h"
#include "meshop/generators/GridGenerator.h"
#include "meshop/generators/CylinderGenerator.h"
#include "meshop/modifiers/NormalModifier.h"
#include "MeshImporter.h"
#include "MeshExporter.h"
#include "OperatorNetwork.h"

#include "utils.h"
#include "mesh_utils.h"

#include "gltf_utils.h"
#include "Mesh.h"
#include "formatters.h"

using namespace msh;
MeshExporter me;
void export_temp_mesh(Mesh& mesh){
    fs::path path = fs::temp_directory_path() / "temp_mesh.ply";

    me.MakeScene(mesh);
    me.ExportPLY(path.string().c_str());
}
int main() {

    Log::Init();
    OperatorNetwork net; 

    Ref<SquareGenerator> square = MakeRef<SquareGenerator>();
    Ref<GridGenerator> grid = MakeRef<GridGenerator>(5.0f, 1.0f, 50, 10);
    grid->setName("Grid Operator");
    Ref<CylinderGenerator> cylinder = MakeRef<CylinderGenerator>();
    cylinder->setName("Cylinder Operator");

    net.addOperator(cylinder);
    
    Ref<NormalModifier> normal_mod = MakeRef<NormalModifier>();
    normal_mod->setName("Normal Modifier");
    normal_mod->setInput(0, net.getOperators()[0]);
    net.addOperator(normal_mod);

    net.evaluate();
    grid->update();
    square->update();
    Mesh merged = meshutils::mergeMeshes(grid->mMeshCache, cylinder->mMeshCache);
    Mesh result = merged;
    result.ComputeNormals();
    export_temp_mesh(result);

    LOG_INFO("result : {}", result);


    std::cout << "All Done !!" << std::endl;
    
    return 0;
}

