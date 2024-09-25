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
    result = generateCylinder(0.1f, 5.0f, 60, 10);
    result.ComputeNormals();
    mMeshCache = result;
}
}