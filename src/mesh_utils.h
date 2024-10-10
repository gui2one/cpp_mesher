#ifndef MESH_UTILS_H
#define MESH_UTILS_H
#pragma once    

#include <Mesh.h>
#include <FastNoise/FastNoise.h>
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

    const enum TRANSFORM_ORDER{
        TRS = 0,
        TSR = 1,
        RST = 2,
        RTS = 3,
        SRT = 4,
        STR = 5
    };
    

    Mesh generateGrid(float width, float length, uint32_t cols, uint32_t rows);
    Mesh generateTube(float radius1, float radius2, float height, uint32_t cols, uint32_t rows);
    Mesh generateTube2(float radius, float height, uint32_t cols, uint32_t rows);
    Mesh generateDisc(float radius, uint32_t segs = 32);
    Mesh merge(Mesh& mesh1, Mesh& mesh2);

    void translate(Mesh& mesh, glm::vec3 translation);
    void rotate(Mesh& mesh, glm::vec3 radians, AXYS_ORDER order = AXYS_ORDER::XYZ);
    void scale(Mesh& mesh, glm::vec3 scale);

    struct NoiseParams{
        float lacunarity = 1.0f;
        float gain = 1.0f;
        float amplitude = 1.0f;
        float frequency = 1.0f;
        float weightedStrength = 1.0f;
        glm::vec3 offset = glm::vec3(0.0f, 0.0f, 0.0f);

        uint32_t seed = 0;
        uint32_t octaves = 1;
    };
    void NoiseDisplace(Mesh& mesh, NoiseParams params);
}


#endif