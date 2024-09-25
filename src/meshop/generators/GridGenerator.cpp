#include "GridGenerator.h"
namespace msh{

GridGenerator::GridGenerator() : IMeshOperator() , mWidth(1.0f), mLength(1.0f), mCols(10), mRows(10)
{

}

GridGenerator::GridGenerator(float width, float length, uint32_t cols, uint32_t rows) : IMeshOperator()
{
    mWidth = width;
    mLength = length;
    mCols = cols;
    mRows = rows;
}

GridGenerator::~GridGenerator()
{

}
void GridGenerator::update()
{

    Mesh result = meshutils::generateGrid(mWidth, mLength, mCols, mRows);
    mMeshCache = result;
}
}