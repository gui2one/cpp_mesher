#include "MeshExporter.h"
namespace msh {
	MeshExporter::MeshExporter()
	{
        m_AiExporter = std::make_shared<Assimp::Exporter>();

	}

    void MeshExporter::MakeScene(const Mesh& mesh) {

        m_Scene.mRootNode = new aiNode();

        m_Scene.mMaterials = new aiMaterial * [1];
        m_Scene.mMaterials[0] = nullptr;
        m_Scene.mNumMaterials = 1;

        m_Scene.mMaterials[0] = new aiMaterial();

        m_Scene.mMeshes = new aiMesh * [1];
        m_Scene.mMeshes[0] = nullptr;
        m_Scene.mNumMeshes = 1;

        m_Scene.mMeshes[0] = new aiMesh();
        m_Scene.mMeshes[0]->mMaterialIndex = 0;

        m_Scene.mRootNode->mMeshes = new unsigned int[1];
        m_Scene.mRootNode->mMeshes[0] = 0;
        m_Scene.mRootNode->mNumMeshes = 1;

        auto pMesh = m_Scene.mMeshes[0];

        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> uvs;

        auto& points = mesh.GetPoints();
        for (size_t i = 0; i < points.size(); i++)
        {
            //vertices.push_back(data[draw_order[i]]);
            vertices.push_back(points[i].position);
            normals.push_back(points[i].normal);
            uvs.push_back(points[i].t_coords);

            //LOG_WARN("uvs : {0} {1}", points[i].t_coords.x, points[i].t_coords.y);

        }



        const auto& vVertices = vertices;

        pMesh->mVertices = new aiVector3D[vVertices.size()];
        pMesh->mNormals = new aiVector3D[vVertices.size()];
        pMesh->mNumVertices = int(vVertices.size());

        pMesh->mTextureCoords[0] = new aiVector3D[vVertices.size()];
        pMesh->mNumUVComponents[0] = int(vVertices.size());

        int j = 0;
        for (auto itr = vVertices.begin(); itr != vVertices.end(); ++itr)
        {
            pMesh->mVertices[itr - vVertices.begin()] = aiVector3D(vVertices[j].x, vVertices[j].y, vVertices[j].z);
            pMesh->mNormals[itr - vVertices.begin()] = aiVector3D(normals[j].x, normals[j].y, normals[j].z);
            pMesh->mTextureCoords[0][itr - vVertices.begin()] = aiVector3D(uvs[j].x, uvs[j].y, 0);
            j++;
        }

        auto& faces = mesh.GetFaces();
        pMesh->mFaces = new aiFace[faces.size()];
        pMesh->mNumFaces = (unsigned int)(faces.size());


        for (size_t i = 0; i < faces.size(); i++)
        {
            auto& vFace = mesh.GetFaces()[i];
            const int numIndices = int(faces[i].m_Vertices.size());

            aiFace& face = pMesh->mFaces[i];
            face.mIndices = new unsigned int[numIndices];
            face.mNumIndices = numIndices;

            for (int j = 0; j < numIndices; j++) {

                face.mIndices[j] = faces[i].GetVertex(j).point_id;
                

            }



        }
    }
	bool MeshExporter::ExportPLY(const char* path)
	{

     

        //mExportFormatDesc->id is "collada"  and mFilePath is "C:/Users/kevin/Desktop/myColladaFile.dae"
        if (AI_SUCCESS == m_AiExporter->Export(&m_Scene, std::string("plyb"), std::string(path)))
        {
            LOG_INFO("Successfully exported {0}", path);
            return true;
        };


        LOG_ERROR("Error exporting {0}", path);
		return false;
	}
    bool MeshExporter::ExportGLTF(const char* path)
    {
        //mExportFormatDesc->id is "collada"  and mFilePath is "C:/Users/kevin/Desktop/myColladaFile.dae"
        
        
        if (AI_SUCCESS == m_AiExporter->Export(&m_Scene, std::string("gltf2"), std::string(path)))
        {
            LOG_INFO("Successfully exported {0}", path);
            return true;
        };


        LOG_ERROR("Error exporting {0}", path);
        return false;
    }


    bool MeshExporter::ExportFBX(const char* path)
    {
        //mExportFormatDesc->id is "collada"  and mFilePath is "C:/Users/kevin/Desktop/myColladaFile.dae"


        if (AI_SUCCESS == m_AiExporter->Export(&m_Scene, std::string("fbx"), std::string(path)))
        {
            LOG_INFO("Successfully exported {0}", path);
            return true;
        };


        LOG_ERROR("Error exporting {0}", path);
        return false;
    }
}