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
private:

};

}
#endif