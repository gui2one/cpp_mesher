#include "SquareGenerator.h"
namespace msh {
SquareGenerator::SquareGenerator() : IMeshOperator()
{

}

SquareGenerator::~SquareGenerator()
{

}

void SquareGenerator::update()
{
    Mesh result;
    result = generateGrid(1.0f, 1.0f, 1, 1);
    mMeshCache = result;
}
}
