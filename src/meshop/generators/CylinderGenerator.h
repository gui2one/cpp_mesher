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

private:

};
}

#endif