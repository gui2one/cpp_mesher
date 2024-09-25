#include "CylinderGenerator.h"
namespace msh{
    
CylinderGenerator::CylinderGenerator()
{

}

CylinderGenerator::~CylinderGenerator()
{

}
void CylinderGenerator::update()
{
    float radius = 1.0f;
    float height = 3.0f;
    uint32_t segs = 32;
    Mesh result;
    result = meshutils::generateTube(radius, height, segs, 10);
    Mesh cap = meshutils::generateDisc(radius, segs);
    meshutils::translate(cap,glm::vec3(0.0f, 0.0f, height));
    result = meshutils::merge(result, cap);

    Mesh cap2 = meshutils::generateDisc(radius, segs);
    cap2.GetFaces()[0].reverse();
    result = meshutils::merge(result, cap2);
    result.ComputeNormals();
    mMeshCache = result;
}
}