#pragma once
#include "assimp/cexport.h"
#include "assimp/scene.h"
#include <assimp/Exporter.hpp>
#include "Mesh.h"

#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>

namespace msh {
	class MeshExporter {
	public:
		MeshExporter();
		void MakeScene(const Mesh& mesh);
		void MakeScene(const OMesh& mesh);
		void ClearScene();

		bool ExportPLY(const char* path);
		bool ExportGLTF(const char* path);
		bool ExportFBX(const char* path);
	private:
		 std::shared_ptr<Assimp::Exporter> m_AiExporter;
		 aiScene m_Scene;
	};
}