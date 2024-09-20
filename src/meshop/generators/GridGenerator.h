#ifndef GRIDGENERATOR_H
#define GRIDGENERATOR_H

#pragma once
#include "meshop/MeshGenerator.h"
namespace msh{

class GridGenerator : public MeshGenerator
{
public:
    GridGenerator();
    ~GridGenerator();

    void update() override;

private:

};
}

#endif