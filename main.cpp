#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <assimp/cimport.h>
#include <assimp/Importer.hpp>
#include "assimp/scene.h"
#include <assimp/postprocess.h>
#include <filesystem>
int main() {
    glm::vec3 pos = glm::vec3(); 
    std::cout << "Hello" << std::endl;
    std::cout << glm::to_string(pos) << std::endl;

    std::string Filename = "C:/gui2one/3D/houdini_20_playground/geo/box_corner.glb";

    bool Ret = false;
    Assimp::Importer Importer;

    const aiScene* pScene = Importer.ReadFile(Filename, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);

    if (pScene) {
        // Ret = InitFromScene(pScene, Filename);
        printf("What the fluke ? '%s': '%s'\n", Filename.c_str(), Importer.GetErrorString());
        printf("Num Meshes: '%d'\n", pScene->mNumMeshes);
        printf("Vertex Count: '%d'\n", pScene->mMeshes[0]->mNumVertices);
        
    }
    else {
        printf("Error parsing '%s': '%s'\n", Filename.c_str(), Importer.GetErrorString());
    }
    
    return 0;
}