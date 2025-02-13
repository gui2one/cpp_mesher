#ifndef IRENDERABLE_H
#define IRENDERABLE_H
#pragma once

namespace GLR {
class IRenderable
{
public:
    
public:

    virtual ~IRenderable();

    virtual void InitRenderData() = 0;
    virtual void BindRenderData() = 0;
    virtual void Render() = 0;
    
private:
};
} /* end namespace GLR*/

#endif /* IRENDERABLE_H */