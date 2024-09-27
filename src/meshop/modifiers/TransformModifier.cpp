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
        meshutils::rotate(mMeshCache, glm::vec3(0.0f, PI / 4.0f, 0.0f));
        mMeshCache.ComputeNormals();
        std::cout << "Result: "<< mMeshCache << std::endl;
        
    }else{
        std::cout << "Error ! This node (" << getName() <<") NEEDS an input" << std::endl;
        
    }
}
}