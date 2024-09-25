#ifndef SQUAREGENERATOR_H
#define SQUAREGENERATOR_H

#pragma once

#include "../IMeshOperator.h"
namespace msh {
    
class SquareGenerator : public IMeshOperator
{
public:
    SquareGenerator();
    ~SquareGenerator();

    void update() override;
private:

};
}

#endif