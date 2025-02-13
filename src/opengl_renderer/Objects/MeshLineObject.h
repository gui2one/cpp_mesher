#ifndef MESHLINEOBJECT_H
#define MESHLINEOBJECT_H
#pragma once

#include "Entity3d.h"
#include "IRenderable.h"
#include "Mesh/Mesh.h"
#include "Render/OpenGLBuffer.h"
#include "Render/OpenGLVertexArray.h"

namespace GLR {

struct MeshLinePoint{

    glm::vec3 position;
    glm::vec3 previous_position;
    glm::vec3 next_position;
    glm::vec3 normal;
    glm::vec2 t_coord;
    float direction;

};


class MeshLineObject : public Entity3d, public IRenderable
{
public:
    MeshLineObject();
    MeshLineObject(const MeshLineObject& other);
    MeshLineObject(std::string name);
    ~MeshLineObject();

    void InitRenderData() override;
    void BindRenderData() override;
    void Render() override;

    const std::vector<MeshLinePoint>& GetPoints() const { return m_Points; }
          std::vector<MeshLinePoint>& GetPoints()       { return m_Points; }

    const std::vector<uint32_t>& GetIndices() const { return m_Indices; }
          std::vector<uint32_t>& GetIndices()       { return m_Indices; }


    inline void AddPoint(MeshLinePoint point) { m_Points.push_back(point); }
    inline void SetPoints(std::vector<MeshLinePoint> points) { m_Points = points; }

private:

    std::vector<MeshLinePoint> m_Points;
    std::vector<uint32_t> m_Indices;

    //rendering
    std::shared_ptr<VertexBuffer> m_VertexBuffer;
    std::shared_ptr<IndexBuffer> m_IndexBuffer;
    std::shared_ptr<VertexArray> m_VertexArray;

    BufferLayout m_BufferLayout = {
        {ShaderDataType::Float3, "position"},
        {ShaderDataType::Float3, "previous_position"},
        {ShaderDataType::Float3, "next_position"},
        {ShaderDataType::Float3, "normal"},
        {ShaderDataType::Float2, "t_coord"},
        {ShaderDataType::Float, "direction"},
    };    
};
} /* end namespace GLR*/

#endif