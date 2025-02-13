#include "MeshLineObject.h"

namespace GLR {
MeshLineObject::MeshLineObject()
    :Entity3d()
{

}

MeshLineObject::MeshLineObject(const MeshLineObject &other)
    :Entity3d(other)
{
    m_Points = other.m_Points;
    m_Indices = other.m_Indices;
    m_VertexBuffer = other.m_VertexBuffer;
    m_IndexBuffer = other.m_IndexBuffer;
    m_VertexArray = other.m_VertexArray;
}

MeshLineObject::MeshLineObject(std::string name)
    :Entity3d(name)
{

}

MeshLineObject::~MeshLineObject()
{

}

void MeshLineObject::InitRenderData()
{
    std::vector<MeshLinePoint> double_points;
    double_points.reserve(m_Points.size() * 2);
    for(size_t i = 0; i< m_Points.size(); i++)
    {
        auto pt = m_Points[i];
        pt.direction = 1.0f;
        if(i > 0){
            pt.previous_position = m_Points[i-1].position;
        }else{
            auto next_pos = m_Points[i+1].position;
            auto dir = m_Points[i+1].position - pt.position;
            pt.previous_position = pt.position - dir;
        }

        if( i != m_Points.size()-1){
            pt.next_position = m_Points[i+1].position;
        }else{
            auto prev_pos = m_Points[i-1].position;
            auto dir = m_Points[i-1].position - pt.position;
            pt.next_position = pt.position - dir;
        }

        auto other = pt;
        other.direction = -1.0f;
        double_points.emplace_back(pt);
        double_points.emplace_back(other);
    }

    m_Indices.clear();

    for(uint32_t i=0; i < double_points.size(); i++){
        m_Indices.push_back(i);
    }

    /* init render data */
    m_VertexBuffer = std::make_shared<VertexBuffer>((float*)double_points.data(), (uint32_t)double_points.size() * (uint32_t)sizeof(GLR::MeshLinePoint));
    m_VertexBuffer->setLayout(m_BufferLayout);

    m_IndexBuffer = std::make_shared<IndexBuffer>((uint32_t*)m_Indices.data(), (uint32_t)m_Indices.size() * (uint32_t)sizeof(uint32_t));
    
    m_VertexArray = std::make_shared<VertexArray>();
    m_VertexArray->SetIndexBuffer(m_IndexBuffer);
    m_VertexArray->addVertexBuffer(m_VertexBuffer);
}

void MeshLineObject::BindRenderData()
{
    m_VertexArray->Bind();
    m_IndexBuffer->Bind();
}

void MeshLineObject::Render()
{
    BindRenderData();
    glDrawElements(GL_TRIANGLE_STRIP, (GLsizei)GetIndices().size(), GL_UNSIGNED_INT, nullptr);
}

} /* end namespace GLR*/