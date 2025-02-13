#include <iostream>
#include <fstream>
#include <random>

#include "RNDGenerator.h"
#include "PoissonDiscSampling.h"
namespace GLR::PDS
{

    std::vector<glm::vec2> GeneratePoints(glm::vec2 grid_size, float radius, uint32_t numSamples)
    {

        auto generator = RNDGenerator::GetInstance();

        int k = numSamples;
        float r = radius;
        float w = r / sqrt(2.0f);
        // step 0
        int cols = (int)floor(grid_size.x / w);
        int rows = (int)floor(grid_size.y / w);

        std::vector<glm::vec2> grid;
        std::vector<bool> bool_grid;
        std::vector<glm::vec2> active;
        std::vector<glm::vec2> ordered;

        grid.reserve((size_t)cols * (size_t)rows);
        bool_grid.reserve((size_t)cols * (size_t)rows);

        for (size_t i = 0; i < (size_t)cols * (size_t)rows; ++i)
        {
            grid.emplace_back(glm::vec2(0.f, 0.f));
            bool_grid.emplace_back(false);
        }

        // bail early : there are issues when grid size is 1/1
        if (cols < 2 || rows < 2) {
            LOG_INFO("Grid Size : {} {}", cols, rows);
            return std::vector<glm::vec2>();
        }

        // step 1
        float x = generator->Float(grid_size.x);
        float y = generator->Float(grid_size.y);
        int _i = (int)floor(x / w);
        int j = (int)floor(y / w);
        glm::vec2 pos = glm::vec2(x, y);
        int idx = _i + j * cols;
        grid[idx] = pos;
        bool_grid[idx] = true;
        active.push_back(pos);

        while (active.size() > 0)
        {
            int randIndex = (int)floor(generator->Float((float)active.size()));
            auto pos = active[randIndex];
            bool found = false;
            for (size_t n = 0; n < k; n++)
            {
                auto sample = glm::normalize(glm::vec2(generator->Float(-1.f, 1.f), generator->Float(-1.f, 1.f)));
                float m = generator->Float(r, 2 * r);
                sample *= m;
                sample += pos;

                int col = (int)floor(sample.x / w);
                int row = (int)floor(sample.y / w);

                if (col > -1 && row > -1 && col < cols && row < rows && !bool_grid[col + row * cols])
                {
                    bool ok = true;
                    for (int i = -1; i <= 1; i++)
                    {
                        for (int j = -1; j <= 1; j++)
                        {
                            int index = (col + i) + (row + j) * cols;
                            if (index >= 0 && index < grid.size() - 1)
                            {

                                auto neighbor = grid[index];
                                bool test = bool_grid[index];
                                if (test)
                                {
                                    float d = glm::distance(sample, neighbor);

                                    if (d < r)
                                    {
                                        ok = false;
                                    }
                                }
                            }
                        }
                    }
                    if (ok)
                    {
                        found = true;
                        grid[col + row * cols] = sample;
                        bool_grid[col + row * cols] = true;
                        active.push_back(sample);
                        ordered.push_back(sample);
                        break;
                    }
                }
            }
            if (!found)
            {
                active.erase(active.begin() + randIndex);
            }
        }

        if (ordered.size() == 0) {
            ordered.push_back(glm::vec2(0.5f, 0.5f));
        }

        return ordered;

        // return std::vector<glm::vec2>();
    }
}
