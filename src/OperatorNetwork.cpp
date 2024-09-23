#include "OperatorNetwork.h"

namespace msh
{
    
OperatorNetwork::OperatorNetwork()
{

}

OperatorNetwork::~OperatorNetwork()
{

}

void OperatorNetwork::evaluate()
{
    std::cout << "--- Evaluating Network ---" << std::endl;
    for(auto op : mOperators){
        op->update();
    }
}
}