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
            auto input0 = getInput(0);
            std::cout << "----------------------" << std::endl;
            
            std::cout << "I have an input : " << getInput(0)->getName()  << std::endl;
            mMeshCache = input0->mMeshCache;
            mMeshCache.ComputeNormals();
            std::cout << "Result: "<< mMeshCache << std::endl;
            
        }else{
            std::cout << "Error ! This node (" << getName() <<") NEEDS an input" << std::endl;
            
        }
    }
}