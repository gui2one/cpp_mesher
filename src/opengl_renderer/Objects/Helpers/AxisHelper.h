#ifndef AXISHELPER_H
#define AXISHELPER_H
#pragma once

#include "Objects/Entity3d.h"
#include "Objects/IRenderable.h"
#include "Render/OpenGLMeshBuffer.h"
#include "Mesh/MeshUtils.h"
namespace GLR {
	class AxisHelper : public Entity3d, public IRenderable
	{
	public:
		AxisHelper();
		~AxisHelper();


	void InitRenderData() override;
	void BindRenderData() override;
	void Render() override;
	private:

	MeshBuffer m_MeshBuffer;
		

};
} /* end namespace GLR*/

#endif /* AXISHELPER_H */