#ifndef SQUAREGENERATOR_H
#define SQUAREGENERATOR_H

#pragma once

#include "../MeshGenerator.h"
class SquareGenerator : public MeshGenerator
{
public:
    SquareGenerator();
    ~SquareGenerator();

    void update() override;
private:

};

#endif