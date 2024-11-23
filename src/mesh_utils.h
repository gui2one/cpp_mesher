#ifndef MESH_UTILS_H
#define MESH_UTILS_H
#pragma once    

#include "Mesh.h"
#include "formatters.h"
#include <FastNoise/FastNoise.h>
#include <glm/gtx/hash.hpp>
/* needed for <opensubdiv/far/primvarRefiner.h> */
#include <corecrt_math_defines.h>

#include <opensubdiv/far/topologyDescriptor.h>
#include <opensubdiv/far/stencilTableFactory.h>
#include <opensubdiv/far/primvarRefiner.h>
#include <opensubdiv/osd/cpuEvaluator.h>
#include <opensubdiv/osd/cpuVertexBuffer.h>



namespace msh::meshutils
{
    const enum AXIS_ORDER{
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
        RTS = 2,
        RST = 3,
        STR = 4,
        SRT = 5
    };
    struct osd_Point3 {

        // Minimal required interface ----------------------
        osd_Point3() { }

        void Clear( void * =0 ) {
            _point[0]=_point[1]=_point[2]=0.0f;
        }

        void AddWithWeight(osd_Point3 const & src, float weight) {
            _point[0]+=weight*src._point[0];
            _point[1]+=weight*src._point[1];
            _point[2]+=weight*src._point[2];
        }

        // Public interface ------------------------------------
        void SetPoint(float x, float y, float z) {
            _point[0]=x;
            _point[1]=y;
            _point[2]=z;
        }

        const float * GetPoint() const {
            return _point;
        }

    private:
        float _point[3];
    };  

    struct osd_DATA{
        std::vector<osd_Point3> positions;
        std::vector<osd_Point3> uvs;
        int nverts;
        int nfaces;
        std::vector<int> vertsperface;
        std::vector<int> vertIndices;
    };
 

    const enum SubdivSchema { 
        CatmullClark, 
        Loop, 
        Bilinear 
    };

    Mesh generate_grid(float width, float length, uint32_t cols, uint32_t rows);
    Mesh generate_tube(float radius1, float radius2, float height, uint32_t cols, uint32_t rows);
    Mesh generate_tube2(float radius, float height, uint32_t cols, uint32_t rows);
    Mesh generate_torus(float radius1, float radius2, uint32_t cols, uint32_t rows);
    Mesh generate_disc(float radius, uint32_t segs = 32);
    Mesh merge(Mesh& mesh1, Mesh& mesh2);
    Mesh triangulate(Mesh& mesh);

    Mesh subdivide(Mesh& mesh, int max_level = 1, SubdivSchema schema = CatmullClark);

    void translate(Mesh& mesh, glm::vec3 translation);
    void rotate(Mesh& mesh, glm::vec3 radians, AXIS_ORDER order = AXIS_ORDER::XYZ);
    void scale(Mesh& mesh, glm::vec3 scale);

    void transform(Mesh& mesh, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale_, TRANSFORM_ORDER tr_order = TRS, AXIS_ORDER axys_order = XYZ);
    
    enum NoiseType{
        Simplex = 0,
        Perlin = 1,
        Cellular = 2
    };
    struct NoiseParams{
        NoiseType noise_type = NoiseType::Cellular;
        float lacunarity = 1.0f;
        float gain = 1.0f;
        float amplitude = 1.0f;
        float frequency = 1.0f;
        float weightedStrength = 1.0f;
        glm::vec3 offset = glm::vec3(0.0f, 0.0f, 0.0f);

        uint32_t seed = 0;
        uint32_t octaves = 1;
    };
    
    void noise_displace(Mesh& mesh, NoiseParams params);

    void twist(Mesh& mesh, float turns = 1.0f);

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

    void fuse_points(Mesh& mesh, float tolerance = 0.001f);
}


#endif