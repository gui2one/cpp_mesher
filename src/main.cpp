#include "cpp_mesher.h"
#include "meshop/generators/SquareGenerator.h"
#include "meshop/generators/GridGenerator.h"
#include "meshop/generators/CylinderGenerator.h"
#include "meshop/modifiers/NormalModifier.h"
#include "meshop/modifiers/TransformModifier.h"
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
    Ref<GridGenerator> grid = MakeRef<GridGenerator>(5.0f, 5.0f, 50, 10);
    
    grid->setName("Grid Operator");
    Ref<CylinderGenerator> cylinder = MakeRef<CylinderGenerator>();
    cylinder->setName("Cylinder Operator");
    cylinder->mHeight = 15.0f;

    net.addOperator(cylinder);
    
    Ref<NormalModifier> normal_mod = MakeRef<NormalModifier>();
    normal_mod->setName("Normal Modifier");
    normal_mod->setInput(0, net.getOperators()[0]);
    net.addOperator(normal_mod);


    Ref<TransformModifier> transform_mod = MakeRef<TransformModifier>();
    transform_mod->setName("Transform Modifier");
    transform_mod->setInput(0, net.getOperators()[1]);

    transform_mod->mTransformOrder = meshutils::TRANSFORM_ORDER::TRS;
    transform_mod->mRot = glm::vec3(0.0f, PI / 2.0f, 0.0f);

    net.addOperator(transform_mod);
    net.evaluate();

    Mesh result = net.getOperators()[net.getOperators().size() - 1]->mMeshCache;


    // Mesh result = meshutils::generateGrid(1.0f, 1.0f, 1, 1);
    // Mesh copy = result;
    // meshutils::rotate(result, glm::vec3(0.0, PI / 4.0, 0.0), meshutils::AXYS_ORDER::XYZ);
    // meshutils::translate(copy, glm::vec3(0.0f, 0.0f, 1.0f));
    // result = meshutils::merge(result, copy);
    // LOG_INFO("result : {}", result);
    result.ComputeNormals();

    LOG_INFO("result : {}", result);

    export_temp_mesh(result);

    std::cout << "All Done !!" << std::endl;
    
    return 0;
}

