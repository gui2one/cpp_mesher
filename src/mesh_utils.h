#ifndef MESH_UTILS_H
#define MESH_UTILS_H
#pragma once    

#include <Mesh.h>
namespace msh::meshutils
{
    const enum AXYS_ORDER{
        XYZ = 0,
        XZY = 1,
        YXZ = 2,
        YZX = 3,
        ZXY = 4,
        ZYX = 5
    };

    Mesh generateGrid(float width, float length, uint32_t cols, uint32_t rows);
    Mesh generateTube(float radius, float height, uint32_t cols, uint32_t rows);
    Mesh generateTube2(float radius, float height, uint32_t cols, uint32_t rows);
    Mesh generateDisc(float radius, uint32_t segs = 32);
    Mesh merge(Mesh& mesh1, Mesh& mesh2);

    void translate(Mesh& mesh, glm::vec3 translation);
    void rotate(Mesh& mesh, glm::vec3 radians, AXYS_ORDER order = AXYS_ORDER::XYZ);
}


#endif