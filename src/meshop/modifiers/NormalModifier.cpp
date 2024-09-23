#include "NormalModifier.h"
namespace msh{
    
    NormalModifier::NormalModifier():IMeshOperator()
    {

    }

    NormalModifier::~NormalModifier()
    {

    }
    void NormalModifier::update()
    {
        if(getInput(0)){
            std::cout << "I have an input : " << getInput(0)->getName()  << std::endl;
        }
    }
}