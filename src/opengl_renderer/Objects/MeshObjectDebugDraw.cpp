#include "MeshObjectDebugDraw.h"

namespace GLR{

    MeshObjectDebugDraw::MeshObjectDebugDraw() : Entity3d()
    {
        
    }

    MeshObjectDebugDraw::MeshObjectDebugDraw(std::string name) 
        : Entity3d(name)
    {
    }

    MeshObjectDebugDraw::~MeshObjectDebugDraw()
    {

    }

    void MeshObjectDebugDraw::InitRenderData()
    {
        m_LinesBuffer = std::make_shared<LineSegmentsBuffer>();
        
        std::vector<glm::vec3> points;
        for(auto& point : m_Target->GetMesh()->GetPoints())
        {
            points.push_back(point.position);
            points.push_back(point.position + (point.normal* 0.2f));    
            // points.push_back(point.position + glm::vec3(0.0f, 10.0f, 0.0f));
        }
        std::shared_ptr<LineSegments> segments = std::make_shared<LineSegments>();
        segments->SetPoints(points);
        m_LinesBuffer->SetMesh(segments);

        m_LinesBuffer->InitRenderData();
    }

    void MeshObjectDebugDraw::BindRenderData()
    {
        m_LinesBuffer->BindRenderData();
    }

    void MeshObjectDebugDraw::Render()
    {
    
        m_LinesBuffer->BindRenderData();
        m_LinesBuffer->Render();

    }
    void MeshObjectDebugDraw::SetTarget(std::shared_ptr<MeshObject> target)
    {
        m_Target = target;
        SetParent(m_Target.get());

        InitRenderData();
    }
}