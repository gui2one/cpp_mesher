#ifndef CYLINDERGENERATOR_H
#define CYLINDERGENERATOR_H

#pragma once
#include "../IMeshOperator.h"
#include "mesh_utils.h"
namespace msh{
    
class CylinderGenerator : public IMeshOperator
{
public:
    CylinderGenerator();
    ~CylinderGenerator();

    virtual void update() override;

public:

    float mRadius = 1.0f;
    float mHeight = 3.0f;
    uint32_t mSegsU = 32;
    uint32_t mSegsV = 1;

};
}

#endif