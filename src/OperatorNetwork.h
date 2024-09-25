#ifndef OPERATORNETWORK_H
#define OPERATORNETWORK_H

#pragma once

#include "meshop/IMeshOperator.h"
namespace msh
{
    

class OperatorNetwork
{
public:
    OperatorNetwork();
    ~OperatorNetwork();

    inline void addOperator(std::shared_ptr<IMeshOperator> op){
        mOperators.push_back(op);
    }

    inline std::vector<std::shared_ptr<IMeshOperator>> getOperators(){
        return mOperators;
    }

    void evaluate();

private:
    std::vector<std::shared_ptr<IMeshOperator>> mOperators;
};
} // namespace msh

#endif