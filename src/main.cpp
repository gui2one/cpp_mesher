#include "pch.h"
#include <memory>
#include "meshop/MeshGenerator.h"
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
    std::shared_ptr<MeshGenerator> gen2 = std::make_shared<MeshGenerator>();
    ops.push_back(gen2);

    for(auto op : ops){
        op->update();
    }   
    return 0;
}