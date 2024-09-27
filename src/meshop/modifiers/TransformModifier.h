#ifndef TRANSFORMMODIFIER_H
#define TRANSFORMMODIFIER_H

#pragma once
#include "../IMeshOperator.h"
#include "mesh_utils.h"
namespace msh{
    
class TransformModifier : public IMeshOperator
{
public:
    TransformModifier();
    ~TransformModifier();

    virtual void update() override;
public:
    meshutils::AXYS_ORDER mOrder = meshutils::AXYS_ORDER::XYZ;

};

}
#endif