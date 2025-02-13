#ifndef OPENGLLAYER_H
#define OPENGLLAYER_H
#pragma once

#include "Render/OpenGLMeshBuffer.h"
#include "Render/OpenGLFrameBuffer.h"
#include "ShaderManager.h"
#include "Mesh/MeshUtils.h"
namespace GLR
{


class Layer
{
public:
    Layer();
    ~Layer();

    void SetLayout(std::vector<std::shared_ptr<OpenGLFrameBufferLayoutElement>> layout_elements);
    void Display();
    void SetSize(uint32_t width, uint32_t height);
    void SetSupersampling(uint32_t value){ m_FrameBuffer.m_Supersampling = value; }


    FrameBuffer& GetFrameBuffer() { return m_FrameBuffer; };
    
    uint32_t GetColorAttachementID(){ return GetFrameBuffer().m_Layout.m_Elements[0]->textureID; }

    void Bind();
    void Unbind();

    static std::shared_ptr<GLR::Layer> CreateStandardLayout();
private:

    FrameBuffer m_FrameBuffer;
    MeshBuffer m_MeshBuffer;
};

}
#endif