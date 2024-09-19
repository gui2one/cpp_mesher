#include "pch.h"
#include <memory>
#include "meshop/MeshGenerator.h"
#include "meshop/generators/SquareGenerator.h"

#include "utils.h"

using namespace MSH;
int main() {
    std::string Filename = "C:/gui2one/3D/houdini_20_playground/geo/box_corner.glb";

    // bool Ret = false;
    Assimp::Importer Importer;
    const aiScene* pScene = Importer.ReadFile(Filename, aiProcess_Triangulate);

    if (pScene) {
        // Ret = InitFromScene(pScene, Filename);
        printf("Sucessfully Loaded -> '%s'\n", Filename.c_str());
        printf("Num Meshes: '%d'\n", pScene->mNumMeshes);
        printf("Vertex Count: '%d'\n", pScene->mMeshes[0]->mNumVertices);
        printf("Faces Count: '%d'\n", pScene->mMeshes[0]->mNumFaces);
        
    }
    else {
        printf("Error parsing '%s': '%s'\n", Filename.c_str(), Importer.GetErrorString());
    }
    
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

    return 0;
}