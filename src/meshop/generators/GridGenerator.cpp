#include "GridGenerator.h"
namespace msh{

GridGenerator::GridGenerator() : IMeshOperator() , mWidth(1.0f), mLength(1.0f)
{

}

GridGenerator::~GridGenerator()
{

}
void GridGenerator::update()
{
    std::cout << "TODO : Generate a grid" << std::endl;
    Mesh result;
    uint32_t cols = 10;
    uint32_t rows = 10; 
    std::vector<Point> points;
    for(uint32_t i = 0; i < rows; i++){
        for(uint32_t j = 0; j < cols; j++){
            Point p;
            float u = j / (float)(cols - 1);
            float v = i / (float)(rows - 1);
            p.position.x = u * mWidth;
            p.position.y = v * mLength;
            p.position.z = 0.0f;

            p.t_coords.x = u;
            p.t_coords.y = v;
            points.push_back(p);
        }
    }
    result.SetPoints(points);

    std::vector<Face> faces;
    for(uint32_t i = 0; i < rows-1; i++){
        for(uint32_t j = 0; j < cols-1; j++){
            faces.push_back(Face({Vertex(i*cols+j),Vertex(i*cols+j+1),Vertex((i+1)*cols+j+1),Vertex((i+1)*cols+j)}));
        }
    }
    result.SetFaces(faces);
    result.ComputeNormals();

    mMeshCache = result;
}
}