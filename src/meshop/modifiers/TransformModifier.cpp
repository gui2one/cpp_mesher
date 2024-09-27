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
        switch (mTransformOrder)
        {
        case meshutils::TRANSFORM_ORDER::TRS:
            meshutils::translate(mMeshCache, mTranslate);
            meshutils::rotate(mMeshCache, mRot, mAxisOrder);
            meshutils::scale(mMeshCache, mScale);
            break;
        case meshutils::TRANSFORM_ORDER::TSR:
            meshutils::translate(mMeshCache, mTranslate);
            meshutils::scale(mMeshCache, mScale);
            meshutils::rotate(mMeshCache, mRot, mAxisOrder);
            break;
        case meshutils::TRANSFORM_ORDER::RST:
            meshutils::rotate(mMeshCache, mRot, mAxisOrder);
            meshutils::scale(mMeshCache, mScale);
            meshutils::translate(mMeshCache, mTranslate);
            break;
        case meshutils::TRANSFORM_ORDER::RTS:
            meshutils::rotate(mMeshCache, mRot, mAxisOrder);
            meshutils::translate(mMeshCache, mTranslate);
            meshutils::scale(mMeshCache, mScale);
            break;
        case meshutils::TRANSFORM_ORDER::SRT:
            meshutils::scale(mMeshCache, mScale);
            meshutils::rotate(mMeshCache, mRot, mAxisOrder);
            meshutils::translate(mMeshCache, mTranslate);
            break;
        case meshutils::TRANSFORM_ORDER::STR:
            meshutils::scale(mMeshCache, mScale);
            meshutils::translate(mMeshCache, mTranslate);
            meshutils::rotate(mMeshCache, mRot, mAxisOrder);
            break;
        default:
            break;
        }
        
        mMeshCache.ComputeNormals();
        std::cout << "Result: "<< mMeshCache << std::endl;
        
    }else{
        std::cout << "Error ! This node (" << getName() <<") NEEDS an input" << std::endl;
        
    }
}
}