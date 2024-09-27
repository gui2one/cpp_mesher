#ifndef TRANSFORMMODIFIER_H
#define TRANSFORMMODIFIER_H

#pragma once
#include "../IMeshOperator.h"
#include "mesh_utils.h"
namespace msh{
    
class TransformModifier : public IMeshOperator
{
public:
    TransformModifier();
    ~TransformModifier();

    virtual void update() override;
public:
    glm::vec3 mTranslate = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 mRot = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 mScale = glm::vec3(1.0f, 1.0f, 1.0f);

    meshutils::AXYS_ORDER mAxisOrder = meshutils::AXYS_ORDER::XYZ; 
    meshutils::TRANSFORM_ORDER mTransformOrder = meshutils::TRANSFORM_ORDER::TRS; 

};

}
#endif