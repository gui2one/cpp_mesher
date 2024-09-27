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
        meshutils::rotate(mMeshCache, glm::vec3(glm::radians(70.0f), glm::radians(45.0f), 0.0f), mOrder);
        mMeshCache.ComputeNormals();
        std::cout << "Result: "<< mMeshCache << std::endl;
        
    }else{
        std::cout << "Error ! This node (" << getName() <<") NEEDS an input" << std::endl;
        
    }
}
}