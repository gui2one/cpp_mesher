#include "pch.h"
#include <memory>
#include "meshop/MeshGenerator.h"
#include "meshop/generators/SquareGenerator.h"
#include "MeshImporter.h"
#include "MeshExporter.h"

#include "utils.h"

#include <tiny_gltf.h>

using namespace msh;
// using namespace tinygltf;

int main() {
    tinygltf::TinyGLTF loader;
    tinygltf::Model* model = new tinygltf::Model();
    
    loader.WriteGltfSceneToFile(model, "C:/gui2one/000.glb", false, false, false, true);
    MeshImporter* mi = MeshImporter::GetInstance();
    MeshExporter me;
    std::string Filename = "C:/gui2one/3D/houdini_20_playground/geo/box_corner.glb";
    
    Mesh mesh = mi->Import(Filename.c_str());
    std::cout << mesh << std::endl;
    me.MakeScene(mesh);
    me.ExportPLY("C:/gui2one/000.ply");
    // me.ExportGLTF("C:/gui2one/000.glb");
    std::vector<std::shared_ptr<IMeshOperator>> ops;
    std::shared_ptr<MeshGenerator> gen1 = std::make_shared<MeshGenerator>();
    ops.push_back(gen1);
    gen1->setName("operator 1");
    std::shared_ptr<MeshGenerator> gen2 = std::make_shared<SquareGenerator>();
    ops.push_back(gen2);
    gen2->setName("Square Operator");
    gen2->setInput(0, gen1);
    for(auto op : ops){
        op->update();
        std::cout << op->getName() << std::endl;
        std::cout << op->mUUID << std::endl;
        if(op->getInput(0)){
            std::cout << "input 0 : " <<op->getInput(0)->getName() << std::endl;
        }
    }

    delete mi;
    return 0;
}