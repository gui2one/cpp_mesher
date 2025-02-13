#ifndef MESHUTILS_H
#define MESHUTILS_H
#pragma once

#include "Mesh.h"
#include <cmath>
namespace GLR
{

struct MeshUtils
{
	static Mesh MakeQuad();
	static Mesh MakeSimpleBox(float w, float h, float l);
	static Mesh MakeSimpleBox(float size);
	static Mesh MakePlane(float width, float length, uint32_t cols = 1, uint32_t rows = 1, bool triangulate = true);
	static Mesh MakeCircle(float radius = 1.0f, uint32_t segments = 10);
	static Mesh MakeSimpleTube(float radius1 = 1.0f, float radius2 = 1.0f, float length = 1.0f, uint32_t segments = 10, bool triangulate = true);

	static Mesh MakeCylinder(float radius1 = 1.0f, float radius2 = 1.0f, float length = 1.0f, uint32_t segments = 10, bool triangulate = true);
	static Mesh MakeCone(float radius1 = 1.0f, float radius2 = 0.2f, float length = 1.0f, uint32_t segments = 10, bool triangulate = true);
	
	
	static Mesh MakeSimpleSphere(float radius, uint32_t cols = 10, uint32_t rows = 10);
	
	static Mesh MakeBox(float w, float h, float l, uint32_t cols = 10, uint32_t rows = 10, bool triangulate = true);
	static Mesh MakeQuadSphere(float radius, uint32_t cols = 10, uint32_t rows = 10, bool triangulate = true);

	static Mesh UniquePoints(Mesh& input_mesh);
	static Mesh Spherify(Mesh& input_mesh);

	static Mesh Clone(Mesh mesh);

	static Mesh Merge(Mesh& meshA, Mesh& meshB);
	static Mesh PlanarUVS(Mesh& input_mesh, glm::vec3 dir = glm::vec3(0.0f, 1.0f, 0.0f));
	
	static void Reverse(Mesh& mesh);
	static void ComputeNormals(Mesh& mesh, bool fuse_normals = false, float smooth_angle_limit = 60.f);
	static void FuseNormals(Mesh& mesh, float smooth_angle_limit = 60.f);

	static void Scale(Mesh& _mesh, glm::vec3 _scale);
	static void Translate(Mesh& _mesh, glm::vec3 _vec);
	static void Rotate(Mesh& _mesh, glm::vec3 _vec);

	static void UVScale(Mesh& _mesh, glm::vec2 _scale);
	static void UVScale(std::shared_ptr<Mesh> _mesh, glm::vec2 _scale);

	static void UVTranslate(Mesh& _mesh, glm::vec2 _vec);
};

}

#endif /*MESHUTILS_H*/
