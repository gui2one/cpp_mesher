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
namespace fs = std::filesystem;

void export_temp_mesh(Mesh& mesh);
int main() {

    Log::Init();
    OperatorNetwork net; 

    Ref<SquareGenerator> square = MakeRef<SquareGenerator>();
    square->setName("Square Operator");
    net.addOperator(square);
    
    Ref<NormalModifier> normal_mod = MakeRef<NormalModifier>();
    normal_mod->setName("Normal Modifier");
    normal_mod->setInput(0, square);
    net.addOperator(normal_mod);

    net.evaluate();

    Mesh result = normal_mod->mMeshCache;
    export_temp_mesh(result);

    std::cout << "Result: "<< result << std::endl;
    LOG_TRACE("result : {}", result);
    LOG_INFO("result : {}", result);
    LOG_WARN("result : {}", result);
    LOG_ERROR("result : {}", result);
    LOG_CRITICAL("result : {}", result);

    std::cout << "All Done !!" << std::endl;
    
    return 0;
}

void export_temp_mesh(Mesh& mesh){
    fs::path path = fs::temp_directory_path() / "temp_mesh.ply";
    MeshExporter me;
    me.MakeScene(mesh);
    me.ExportPLY(path.string().c_str());
}