#ifndef NORMALMODIFIER_H
#define NORMALMODIFIER_H
#pragma once

#include "../IMeshOperator.h"
namespace msh{
    
    class NormalModifier : public IMeshOperator, public INode
    {
    public:
        NormalModifier();
        ~NormalModifier();

        void update() override;

    private:

    };
}

#endif