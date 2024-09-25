#ifndef IMESHOPERATOR_H
#define IMESHOPERATOR_H

#pragma once

#include "utils.h"
#include "Mesh.h"
namespace msh {

class INode{
public : 
    INode();
    virtual ~INode() = default;
    virtual void update() = 0;

    inline const char * getName() const{
        return mName;
    }

    inline void setName(const char * name){
        mName = name;
    }

    inline void setInput(int index, std::shared_ptr<INode> op){
        inputs[index] = op;
    }

    inline std::shared_ptr<INode> getInput(int index){
        return inputs[index];
    }

public :
    std::string mUUID;
    const char * mName = "default";


    std::shared_ptr<INode> inputs[4] = {nullptr,nullptr,nullptr,nullptr};
};
class IMeshOperator : public INode
{
public:
    
    IMeshOperator();
    virtual ~IMeshOperator(){};

    virtual void update() = 0;

private:


public :
    Mesh mMeshCache;
};
}
#endif