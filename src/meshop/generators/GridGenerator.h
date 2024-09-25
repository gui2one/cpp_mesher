#ifndef GRIDGENERATOR_H
#define GRIDGENERATOR_H
#pragma once

#include "../IMeshOperator.h"

namespace msh{

class GridGenerator : public IMeshOperator, public INode
{
public:
    GridGenerator();
    ~GridGenerator();

    void update() override;

public:

    float mWidth;
    float mLength;

};
}

#endif