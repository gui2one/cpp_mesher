#ifndef PCH_H
#define PCH_H
#pragma once

#include <assert.h>

#include <algorithm>
#include <deque>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <numeric>
#include <queue>
#include <string>
#include <thread>
#include <vector>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/vector_angle.hpp>

// #undef min
// #undef max

#define NOMINMAX
#include <glad/glad.h>
#include <windows.h>

#include "Log.h"

namespace fs = std::filesystem;
#endif