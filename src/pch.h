#pragma once
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