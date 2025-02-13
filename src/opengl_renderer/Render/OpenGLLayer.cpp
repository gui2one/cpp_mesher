#include "OpenGLLayer.h"

namespace GLR{

    Layer::Layer()
    {
        Mesh quad = MeshUtils::MakeQuad();
        MeshUtils::Translate(quad, glm::vec3(0.5f, 0.5f, 0.0f));
        auto mesh = std::make_shared<Mesh>(quad);
        m_MeshBuffer.SetMesh(mesh);
        m_MeshBuffer.InitRenderData();
    }

    Layer::~Layer()
    {
        LOG_INFO("Layer Destructor");
    }

    void Layer::SetLayout(std::vector<std::shared_ptr<OpenGLFrameBufferLayoutElement>> layout_elements)
    {
        for(auto& element : layout_elements)
        {
            m_FrameBuffer.AddLayoutElement(element);
        }
    }
    void Layer::Display()
    {
        glDisable(GL_CULL_FACE);

        auto& shader = ShaderManager::GetInstance()->screen_shader;
        shader->UseProgram();

        glBindTexture(GL_TEXTURE_2D, m_FrameBuffer.m_Layout.m_Elements[0]->textureID);

        auto projection = glm::ortho(0.0f, 1.0f * m_FrameBuffer.m_Supersampling , 0.0f, 1.0f * m_FrameBuffer.m_Supersampling, -1.0f, 1.0f);
        auto transform = glm::mat4(1.0);
        shader->SetMat4("u_projection", projection);

        m_MeshBuffer.Render();
    }
    void Layer::SetSize(uint32_t width, uint32_t height)
    {
        uint32_t cur_width = m_FrameBuffer.GetWidth() / m_FrameBuffer.m_Supersampling; 
        uint32_t cur_height = m_FrameBuffer.GetHeight() / m_FrameBuffer.m_Supersampling; 
        if(width != cur_width || height != cur_height){

            m_FrameBuffer.Invalidate(width, height);
            // LOG_INFO("FB was invalidated. New Size : {} {}", width, height);
        }
    }
    void Layer::Bind()
    {
        m_FrameBuffer.Bind();
    }
    void Layer::Unbind()
    {
        m_FrameBuffer.Unbind();
    }
    
    std::shared_ptr<GLR::Layer> Layer::CreateStandardLayout()
    {

        std::vector<std::shared_ptr<OpenGLFrameBufferLayoutElement>> elements;

        auto color_element = std::make_shared<OpenGLFrameBufferLayoutElement>();
        color_element->textureID = 0;
        color_element->attachment = GL_COLOR_ATTACHMENT0;
        color_element->internalFormat = GL_SRGB_ALPHA;
        color_element->format = GL_RGBA;
        color_element->type = GL_FLOAT;
        elements.push_back(color_element);

        auto color_id_element = std::make_shared<OpenGLFrameBufferLayoutElement>();
        color_id_element->textureID = 0;
        color_id_element->attachment = GL_COLOR_ATTACHMENT1;
        color_id_element->internalFormat = GL_RGBA8;
        color_id_element->format = GL_RGBA;
        color_id_element->type = GL_UNSIGNED_BYTE;
        elements.push_back(color_id_element);

        auto depth_element = std::make_shared<OpenGLFrameBufferLayoutElement>();
        depth_element->textureID = 0;
        depth_element->attachment = GL_DEPTH_STENCIL_ATTACHMENT;
        depth_element->internalFormat = GL_DEPTH24_STENCIL8;
        depth_element->format = GL_DEPTH_STENCIL;
        depth_element->type = GL_UNSIGNED_INT_24_8;
        elements.push_back(depth_element);  

        auto layer = std::make_shared<Layer>(); 
        layer->SetLayout(elements);

        return layer;

    }
}