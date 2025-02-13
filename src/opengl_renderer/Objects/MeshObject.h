#ifndef MESHOBJECT_H
#define MESHOBJECT_H
#pragma once

#include "Entity3d.h"
#include "Mesh/Mesh.h"
#include "Mesh/PointCloud.h"
#include "Mesh/MeshUtils.h"
#include "Objects/IRenderable.h"

#include "Render/Material.h"
#include "Render/OpenGLMeshBuffer.h"
#include "Render/OpenGLBuffer.h"
#include "Render/OpenGLVertexArray.h"


namespace GLR {

    class MeshObject : public Entity3d, public IRenderable
    {
    public:

        std::shared_ptr<Material> m_Material;
    public: 
        MeshObject();
        MeshObject(std::string name);
        MeshObject(const MeshObject& other);
        MeshObject& operator=(const MeshObject& other);
        virtual ~MeshObject() {};
        
        void InitRenderData() override;
        void BindRenderData() override;
        void Render() override;

        void SetMesh(std::shared_ptr<Mesh> mesh);
        inline  std::shared_ptr<Mesh> GetMesh() { return m_MeshBuffer.GetMesh(); }
        inline const  std::shared_ptr<Mesh> GetMesh() const { return m_MeshBuffer.GetMesh(); }

        MeshBuffer* GetMeshBuffer() { return &m_MeshBuffer; }

        inline void SetItemUUID(std::string uuid) { m_ItemUUID = uuid;  }
        inline std::string& GetItemUUID() { return m_ItemUUID; }
        inline const std::string& GetItemUUID() const { return m_ItemUUID; }

        inline BoundingBox& GetBoundingBox(){ return m_BoundingBox; }
        inline const BoundingBox& GetBoundingBox() const { return m_BoundingBox; }

        static std::shared_ptr<MeshObject> MakePlaneObject();
        static std::shared_ptr<MeshObject> MakeBoxObject();
        static std::shared_ptr<MeshObject> MakeQuadSphereObject();


    private:
        std::string m_ItemUUID;
        BoundingBox m_BoundingBox;
        MeshBuffer m_MeshBuffer;

        // BufferLayout m_BufferLayout = {
        //     {ShaderDataType::Float3, "position"},
        //     {ShaderDataType::Float3, "normal"},
        //     {ShaderDataType::Float2, "t_coords"}
        // };           


    };

}
#endif /* MESHOBJECT_H */