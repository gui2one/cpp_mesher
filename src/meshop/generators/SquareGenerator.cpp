#include "SquareGenerator.h"
namespace msh {
SquareGenerator::SquareGenerator()
{

}

SquareGenerator::~SquareGenerator()
{

}

void SquareGenerator::update()
{
    std::cout << "Generating a Square ..." << std::endl;
    Mesh result;
    std::vector<Point> points;
    Point pt1 = Point();
    pt1.position = glm::vec3(0.0f,0.0f,0.0f);
    pt1.t_coords = glm::vec2(0.0f,0.0f);
    Point pt2 = Point();
    pt2.position = glm::vec3(1.0f,0.0f,0.0f);
    pt2.t_coords = glm::vec2(1.0f,0.0f);
    Point pt3 = Point();
    pt3.position = glm::vec3(1.0f,1.0f,0.0f);
    pt3.t_coords = glm::vec2(1.0f,1.0f);
    Point pt4 = Point();
    pt4.position = glm::vec3(0.0f,1.0f,0.0f);
    pt4.t_coords = glm::vec2(0.0f,1.0f);
    points.push_back(pt1);
    points.push_back(pt2);
    points.push_back(pt3);
    points.push_back(pt4);
    
    result.SetPoints(points);

    std::vector<Face> faces;
    faces.push_back(Face({Vertex(0),Vertex(1),Vertex(2),Vertex(3)}));
    result.SetFaces(faces);

    mMeshCache = result;
}
}
