#ifndef IMESHOPERATOR_H
#define IMESHOPERATOR_H

#pragma once

#include "utils.h"
#include "Mesh.h"
namespace msh {

class INode{
public : 
    virtual ~INode() = default;
    virtual void update() = 0;
};
class IMeshOperator
{
public:
    
    IMeshOperator();
    virtual ~IMeshOperator(){};

    inline const char * getName() const{
        return mName;
    }

    inline void setName(const char * name){
        mName = name;
    }

    inline void setInput(int index, std::shared_ptr<IMeshOperator> op){
        inputs[index] = op;
    }

    inline std::shared_ptr<IMeshOperator> getInput(int index){
        return inputs[index];
    }


private:


public :
    std::string mUUID;
    const char * mName = "default";

    Mesh mMeshCache;

    std::shared_ptr<IMeshOperator> inputs[4] = {nullptr,nullptr,nullptr,nullptr};
};
}
#endif