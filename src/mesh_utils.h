#ifndef MESH_UTILS_H
#define MESH_UTILS_H
#pragma once    

#include <Mesh.h>
#include <FastNoise/FastNoise.h>
#include <glm/gtx/hash.hpp>
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

    void Twist(Mesh& mesh, float turns = 1.0f);

// Custom hash function that respects the tolerance
struct Vec3Hash {
    float tolerance;

    Vec3Hash(float _tolerance = 0.01f) : tolerance(_tolerance) {}

    std::size_t operator()(const glm::vec3& v) const {
        // You can round the values to multiples of the tolerance to ensure vectors that are
        // "close enough" have the same hash.
        auto x = std::round(v.x / tolerance);
        auto y = std::round(v.y / tolerance);
        auto z = std::round(v.z / tolerance);
        std::size_t h1 = std::hash<int>()(static_cast<int>(x));
        std::size_t h2 = std::hash<int>()(static_cast<int>(y));
        std::size_t h3 = std::hash<int>()(static_cast<int>(z));
        return h1 ^ (h2 << 1) ^ (h3 << 2); // Combine the hashes
    }
};
    struct Vec3Equal {
        float tolerance = 0.01f;

        Vec3Equal(float _tolerance = 0.01f) : tolerance(_tolerance) {}
        bool operator()(const glm::vec3& v1, const glm::vec3& v2) const {
            // float _tolerance = 0.01f;
            return std::abs(v1.x - v2.x) < tolerance &&
                std::abs(v1.y - v2.y) < tolerance &&
                std::abs(v1.z - v2.z) < tolerance;
        }
    };

    void fusePoints(Mesh& mesh, float tolerance = 0.001f);
}


#endif