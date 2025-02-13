#pragma once
#include "glm/glm.hpp"
#include <vector>
namespace GLR
{

	namespace PDS
	{
		std::vector<glm::vec2> GeneratePoints(glm::vec2 grid_size, float cell_size, uint32_t numSamples);
	}
}