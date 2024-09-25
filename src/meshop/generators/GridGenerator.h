#ifndef GRIDGENERATOR_H
#define GRIDGENERATOR_H
#pragma once

#include "../IMeshOperator.h"
#include "mesh_utils.h"
namespace msh{

class GridGenerator : public IMeshOperator
{
public:
    GridGenerator();
    GridGenerator(float width, float length, uint32_t cols = 3 , uint32_t rows = 3);
    ~GridGenerator();

    void update() override;

public:

    float mWidth;
    float mLength;

    uint32_t mCols;
    uint32_t mRows;

};
}

#endif