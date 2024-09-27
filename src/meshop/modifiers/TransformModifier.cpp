#include "TransformModifier.h"

namespace msh{
    
TransformModifier::TransformModifier()
{

}

TransformModifier::~TransformModifier()
{

}

void TransformModifier::update(){
    if(getInput(0)){
        auto input0 = std::dynamic_pointer_cast<IMeshOperator>(getInput(0));
        mMeshCache = input0->mMeshCache;
        meshutils::rotate(mMeshCache, mRot, mOrder);
        mMeshCache.ComputeNormals();
        std::cout << "Result: "<< mMeshCache << std::endl;
        
    }else{
        std::cout << "Error ! This node (" << getName() <<") NEEDS an input" << std::endl;
        
    }
}
}