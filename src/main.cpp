#include "pch.h"
int main() {

    std::string Filename = "C:/gui2one/3D/houdini_20_playground/geo/box_corner.glb";

    bool Ret = false;
    Assimp::Importer Importer;

    const aiScene* pScene = Importer.ReadFile(Filename, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);

    if (pScene) {
        // Ret = InitFromScene(pScene, Filename);
        printf("Sucessfully Loaded -> '%s'\n", Filename.c_str());
        printf("Num Meshes: '%d'\n", pScene->mNumMeshes);
        printf("Vertex Count: '%d'\n", pScene->mMeshes[0]->mNumVertices);
        
    }
    else {
        printf("Error parsing '%s': '%s'\n", Filename.c_str(), Importer.GetErrorString());
    }
    
    return 0;
}