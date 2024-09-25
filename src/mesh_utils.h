#ifndef MESH_UTILS_H
#define MESH_UTILS_H
#pragma once    

#include <Mesh.h>
namespace msh
{
    Mesh generateGrid(float width, float length, uint32_t cols, uint32_t rows);
}

#endif