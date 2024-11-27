#pragma once

#include <cstdint>
#include <string>
#include <iostream>
#include <vector>
#include <filesystem>
#include <memory>
#include <algorithm>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>

#include "cgal/boolean_utils.h"

#include "spdlog/fmt/ostr.h"  // support for user defined types


// openmesh stuff
#define _USE_MATH_DEFINES
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
typedef OpenMesh::PolyMesh_ArrayKernelT<> OMesh;