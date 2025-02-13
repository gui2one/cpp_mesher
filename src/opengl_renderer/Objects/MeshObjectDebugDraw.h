#ifndef MESHOBJECTDEBUGDRAW_H
#define MESHOBJECTDEBUGDRAW_H
#pragma once

#include "Objects/MeshObject.h"
#include "Render/OpenGLLineSegmentsBuffer.h"
#include "Objects/IRenderable.h"
#include "ShaderManager.h"

namespace GLR{

    class MeshObjectDebugDraw : public Entity3d, public IRenderable
    {

    public:

        MeshObjectDebugDraw();
        MeshObjectDebugDraw(std::string name);
        ~MeshObjectDebugDraw();

        void InitRenderData() override;
        void BindRenderData() override;
        void Render() override;        

        void SetTarget(std::shared_ptr<MeshObject> target);



    private:
        std::shared_ptr<MeshObject> m_Target;
        std::shared_ptr<LineSegmentsBuffer> m_LinesBuffer;

    };
}

#endif