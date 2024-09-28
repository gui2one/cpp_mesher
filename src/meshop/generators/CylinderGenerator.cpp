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
    Mesh result;
    result = meshutils::generateTube(mRadius, mHeight, mSegsU, mSegsV);
    Mesh cap = meshutils::generateDisc(mRadius, mSegsU);
    meshutils::translate(cap,glm::vec3(0.0f, 0.0f, mHeight));
    result = meshutils::merge(result, cap);

    Mesh cap2 = meshutils::generateDisc(mRadius, mSegsU);
    cap2.GetFaces()[0].reverse();
    result = meshutils::merge(result, cap2);
    
    mMeshCache = result;
}
}