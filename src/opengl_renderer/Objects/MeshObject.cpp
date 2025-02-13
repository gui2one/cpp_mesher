#include "MeshObject.h"

namespace GLR
{
	MeshObject::MeshObject():Entity3d()
	{
		
	}
	
	MeshObject::MeshObject(std::string name)
		:Entity3d(name)
	{
	}

	MeshObject::MeshObject(const MeshObject& other)
		:Entity3d(other)
	{
		m_BoundingBox = other.m_BoundingBox;
		m_Position    = other.m_Position;
		m_Rotation    = other.m_Rotation;
		m_Scale       = other.m_Scale;
		m_Transforms  = other.m_Transforms;
		m_ItemUUID  = other.m_ItemUUID;
		m_MeshBuffer = other.m_MeshBuffer;
	}
    
	MeshObject& MeshObject::operator=(const MeshObject &other)
    {
		m_BoundingBox = other.m_BoundingBox;
		m_Position    = other.m_Position;
		m_Rotation    = other.m_Rotation;
		m_Scale       = other.m_Scale;
		m_Transforms  = other.m_Transforms;
		m_ItemUUID  = other.m_ItemUUID;
		m_MeshBuffer = other.m_MeshBuffer;

		return *this;	
    }

    void MeshObject::InitRenderData()
	{
		m_MeshBuffer.InitRenderData();
	}

	void MeshObject::BindRenderData()
	{
		m_MeshBuffer.BindRenderData();
	}

    void MeshObject::Render()
    {
		m_MeshBuffer.Render();
    }

    void MeshObject::SetMesh(std::shared_ptr<Mesh> mesh)
 	{
	
		mesh->ComputeAABB();
		m_BoundingBox = mesh->m_BoundingBox;
		m_MeshBuffer.SetMesh(mesh);

		m_MeshBuffer.InitRenderData();
	}

    std::shared_ptr<MeshObject> MeshObject::MakePlaneObject()
    {
		std::shared_ptr<MeshObject> plane = std::make_shared<MeshObject>();
		auto mesh = MeshUtils::MakePlane(1.0f, 1.0f, 3, 3);
		MeshUtils::Rotate(mesh, glm::vec3( glm::pi<float>(), 0.0f, 0.0f));

		MeshUtils::ComputeNormals(mesh, false, 80.0f);
		
		plane->SetMesh(std::make_shared<Mesh>(mesh));
        return plane;
    }

    std::shared_ptr<MeshObject> MeshObject::MakeBoxObject()
    {
		std::shared_ptr<MeshObject> box = std::make_shared<MeshObject>();

		auto mesh = MeshUtils::MakeBox(1.0f, 1.0f, 1.0f, 1, 1);
		// MeshUtils::Reverse(mesh);
		MeshUtils::ComputeNormals(mesh, false, 80.0f);
		// mesh.Triangulate();
		box->SetMesh(std::make_shared<Mesh>(mesh));
        return box;
    }
	
    std::shared_ptr<MeshObject> MeshObject::MakeQuadSphereObject()
    {
		std::shared_ptr<MeshObject> quad_sphere = std::make_shared<MeshObject>();

		auto mesh = MeshUtils::MakeQuadSphere(1.0f);
		quad_sphere->SetMesh(std::make_shared<Mesh>(mesh));
        
		return quad_sphere;
    }
}
