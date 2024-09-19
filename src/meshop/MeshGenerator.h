#ifndef MESHGENERATOR_H
#define MESHGENERATOR_H

#pragma once


#include "IMeshOperator.h"
namespace MSH {
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