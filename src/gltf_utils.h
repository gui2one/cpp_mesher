#pragma once

#include <tiny_gltf.h>
#include "Mesh.h"
namespace msh
{
    
    void WriteMeshToGLTF(const Mesh &mesh, const std::string &outputFile);
} // namespace msh
