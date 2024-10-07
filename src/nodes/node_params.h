#ifndef NODE_PARAMS_H
#define NODE_PARAMS_H
#pragma once

#include <NodeParam.h>
// #include <Event.h>
#include <EventDispatcher.h>
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
        
        if(ImGui::DragFloat3(name, glm::value_ptr(value), 0.05f)){
            ParamChangedEvent event;
            EventManager::GetInstance().Dispatch(event);
        }
    }
public:
    glm::vec3 value;
};
};
#endif