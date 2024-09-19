#ifndef MESHGENERATOR_H
#define MESHGENERATOR_H

#pragma once


#include "IMeshOperator.h"
namespace msh {
class MeshGenerator : public IMeshOperator
{
public:
    MeshGenerator();
    ~MeshGenerator();

    void update() override;

private:

};
}
#endif