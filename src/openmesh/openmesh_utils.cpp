#include "openmesh_utils.h"
namespace NED::openmeshutils {

void compute_normals(GMesh &mesh){
  mesh.request_vertex_normals();
  mesh.request_face_normals();
  // let the mesh update the normals
  mesh.update_normals();
  // dispose the face normals, as we don't need them anymore
  mesh.release_face_normals();
}

void set_normals(GMesh &mesh, glm::vec3 normal){
  mesh.request_vertex_normals();

  for(auto vh : mesh.vertices()){
    mesh.set_normal(vh, OpenMesh::Vec3f(normal.x, normal.y, normal.z));
  }
}

GMesh openmesh_cube() {
  GMesh mesh;

  auto pt0 = mesh.add_point(GMesh::Point(-1, -1, 1));
  auto pt1 = mesh.add_point(GMesh::Point(1, -1, 1));
  auto pt2 = mesh.add_point(GMesh::Point(1, 1, 1));
  auto pt3 = mesh.add_point(GMesh::Point(-1, 1, 1));
  auto pt4 = mesh.add_point(GMesh::Point(-1, -1, -1));
  auto pt5 = mesh.add_point(GMesh::Point(1, -1, -1));
  auto pt6 = mesh.add_point(GMesh::Point(1, 1, -1));
  auto pt7 = mesh.add_point(GMesh::Point(-1, 1, -1));


  auto v0 = mesh.add_vertex_from_point(pt0);
  auto v1 = mesh.add_vertex_from_point(pt1);
  auto v2 = mesh.add_vertex_from_point(pt2);
  auto v3 = mesh.add_vertex_from_point(pt3);
  auto v4 = mesh.add_vertex_from_point(pt4);
  auto v5 = mesh.add_vertex_from_point(pt5);
  auto v6 = mesh.add_vertex_from_point(pt6);
  auto v7 = mesh.add_vertex_from_point(pt7);

  mesh.add_face(v0, v1, v2, v3);
  mesh.add_face(v7, v6, v5, v4);
  mesh.add_face(v1, v0, v4, v5);
  mesh.add_face(v2, v1, v5, v6);
  mesh.add_face(v3, v2, v6, v7);
  mesh.add_face(v0, v3, v7, v4);

  return mesh;
}

GMesh openmesh_square() 
{
  GMesh mesh;
  mesh.request_vertex_texcoords2D();

  auto pt0 = mesh.add_point(GMesh::Point(0, 0, 0));
  auto pt1 = mesh.add_point(GMesh::Point(1, 0, 0));
  auto pt2 = mesh.add_point(GMesh::Point(1, 1, 0));
  auto pt3 = mesh.add_point(GMesh::Point(0, 1, 0));

  auto v0 = mesh.add_vertex_from_point(pt0);
  auto v1 = mesh.add_vertex_from_point(pt1);
  auto v2 = mesh.add_vertex_from_point(pt2);
  auto v3 = mesh.add_vertex_from_point(pt3);

  mesh.set_texcoord2D_glm(v0, glm::vec2(0.0f, 0.0f));
  mesh.set_texcoord2D_glm(v1, glm::vec2(1.0f, 0.0f));
  mesh.set_texcoord2D_glm(v2, glm::vec2(1.0f, 1.0f));
  mesh.set_texcoord2D_glm(v3, glm::vec2(0.0f, 1.0f));

  mesh.add_face(v0, v1, v2, v3);
  
  return mesh;
}
};  // namespace NED::openmeshutils