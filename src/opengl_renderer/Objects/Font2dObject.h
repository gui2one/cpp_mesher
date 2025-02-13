#ifndef FONT_2D_OBJECT_H
#define FONT_2D_OBJECT_H
#pragma once

#include "Entity3d.h"
#include "Mesh/Mesh.h"
#include "Mesh/PointCloud.h"
#include "Mesh/MeshUtils.h"
#include "Objects/IRenderable.h"

#include "Render/OpenGLMeshBuffer.h"
#include "Render/OpenGLBuffer.h"
#include "Render/OpenGLVertexArray.h"
#include "MSDFAtlas.h"


namespace GLR {

    class Font2dObject : public Entity3d, public IRenderable
    {
    public:

    public: 
        Font2dObject();
        Font2dObject(std::string name);
        Font2dObject(const Font2dObject& other);
        Font2dObject& operator=(const Font2dObject& other);
        virtual ~Font2dObject() {};
        
        void InitRenderData() override;
        void BindRenderData() override;
        void Render() override;

        void SetAtlas(MSDFAtlas* atlas){ m_Atlas = atlas;}
        MSDFAtlas* GetAtlas() const { return m_Atlas;}

        void SetText(const char* str) { 
            m_Text = str;
            GenerateQuads();    
        }
        const char* GetText() { return m_Text;}
        void GenerateQuads();
        inline  std::shared_ptr<Mesh> GetMesh() { return m_MeshBuffer.GetMesh(); }
        inline const  std::shared_ptr<Mesh> GetMesh() const { return m_MeshBuffer.GetMesh(); }

        inline void SetItemUUID(std::string uuid) { m_ItemUUID = uuid;  }
        inline std::string& GetItemUUID() { return m_ItemUUID; }
        inline const std::string& GetItemUUID() const { return m_ItemUUID; }

        inline BoundingBox& GetBoundingBox(){ return m_BoundingBox; }
        inline const BoundingBox& GetBoundingBox() const { return m_BoundingBox; }


    private:
        std::string m_ItemUUID;
        BoundingBox m_BoundingBox;
        MeshBuffer m_MeshBuffer;

        MSDFAtlas* m_Atlas;

        const char* m_Text;
        // BufferLayout m_BufferLayout = {
        //     {ShaderDataType::Float3, "position"},
        //     {ShaderDataType::Float3, "normal"},
        //     {ShaderDataType::Float2, "t_coords"}
        // };           


    };

}
#endif /* Text2dObject_H */