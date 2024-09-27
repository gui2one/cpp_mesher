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
            auto input0 = std::dynamic_pointer_cast<IMeshOperator>(getInput(0));
            mMeshCache = input0->mMeshCache;
            mMeshCache.ComputeNormals();
            
        }else{
            std::cout << "Error ! This node (" << getName() <<") NEEDS an input" << std::endl;
            
        }
    }
}