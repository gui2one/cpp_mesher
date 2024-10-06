#ifndef NODE_PARAMS_H
#define NODE_PARAMS_H
#pragma once

#include <NodeParam.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace NodeEditor {

template<>
class Param<glm::vec3> : public NodeParam
{
public:
    Param(const char * _name, glm::vec3 _value) : NodeParam(_name), value(_value) {}

    ~Param() {}

    glm::vec3 Eval(){
        return value;
    }

    void Display(){
        
        ImGui::InputFloat3(name, glm::value_ptr(value));
    }
public:
    glm::vec3 value;
};
};
#endif