#pragma once

#include <assimp/Importer.hpp>
#include "Mesh.h"

namespace msh {
	class MeshImporter {
	public:
		Mesh Import(const char* path);

		static MeshImporter* s_Instance;
		static MeshImporter* GetInstance();
	private:

	protected:
		MeshImporter() = default;

	};
}