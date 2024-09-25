#include "meshop/generators/SquareGenerator.h"
#include "meshop/generators/GridGenerator.h"
#include "meshop/modifiers/NormalModifier.h"
#include "MeshImporter.h"
#include "MeshExporter.h"
#include "OperatorNetwork.h"

#include "utils.h"

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

    Ref<GridGenerator> grid = MakeRef<GridGenerator>(1.0f, 2.0f);
    grid->setName("Square Operator");

    net.addOperator(grid);
    
    Ref<NormalModifier> normal_mod = MakeRef<NormalModifier>();
    normal_mod->setName("Normal Modifier");
    normal_mod->setInput(0, grid);
    net.addOperator(normal_mod);

    net.evaluate();

    Mesh result = normal_mod->mMeshCache;
    export_temp_mesh(result);

    LOG_INFO("result : {}", result);


    std::cout << "All Done !!" << std::endl;
    
    return 0;
}

