#include "AxisHelper.h"

namespace GLR {

AxisHelper::AxisHelper() 
	: Entity3d()
{

}

AxisHelper::~AxisHelper()
{

}

void AxisHelper::BindRenderData()
{
	if( m_MeshBuffer.GetMesh() != nullptr)
	{
		m_MeshBuffer.BindRenderData();
	}
}

void AxisHelper::InitRenderData()
{
	float radius = 0.025f;
	Mesh mesh;
	Mesh axis_x = MeshUtils::MakeCylinder(radius, radius, 1.0f);
	axis_x.SetColor(glm::vec3(0.0f, 0.6f, 0.0f));
	MeshUtils::Merge(mesh, axis_x);

	auto axis_y = axis_x;
	MeshUtils::Rotate(axis_y, glm::vec3(0.0f, 0.0f, glm::pi<float>() / 2.0f));
	axis_y.SetColor(glm::vec3(1.0f, 0.0f, 0.0f));
	MeshUtils::Merge(mesh, axis_y);

	auto axis_z = axis_x;
	MeshUtils::Rotate(axis_z, glm::vec3(glm::pi<float>() / 2.0f, 0.0f, 0.0f));
	axis_z.SetColor(glm::vec3(0.0f, 0.0f, 1.0f));
	MeshUtils::Merge(mesh, axis_z);

	Mesh sphere = MeshUtils::MakeSimpleSphere(radius * 2.0f, 20, 10);
	sphere.SetColor(glm::vec3(1.f));
	MeshUtils::Merge(mesh, sphere);	
	
	mesh.Triangulate();

	m_MeshBuffer.SetMesh(std::make_shared<Mesh>(mesh));

	m_MeshBuffer.InitRenderData();

}

void AxisHelper::Render()
{
	m_MeshBuffer.BindRenderData();
	m_MeshBuffer.Render();
}

} /* end namespace GLR*/