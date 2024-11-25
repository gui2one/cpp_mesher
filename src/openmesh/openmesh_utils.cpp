#include "openmesh_utils.h"
namespace NED::openmeshutils {

GMesh openmesh_cube() {
  GMesh mesh;
  mesh.request_vertex_normals();
  // generate vertices
  GMesh::VertexHandle vhandle[8];
  vhandle[0] = mesh.add_vertex(GMesh::Point(-1, -1, 1));
  vhandle[1] = mesh.add_vertex(GMesh::Point(1, -1, 1));
  vhandle[2] = mesh.add_vertex(GMesh::Point(1, 1, 1));
  vhandle[3] = mesh.add_vertex(GMesh::Point(-1, 1, 1));
  vhandle[4] = mesh.add_vertex(GMesh::Point(-1, -1, -1));
  vhandle[5] = mesh.add_vertex(GMesh::Point(1, -1, -1));
  vhandle[6] = mesh.add_vertex(GMesh::Point(1, 1, -1));
  vhandle[7] = mesh.add_vertex(GMesh::Point(-1, 1, -1));
  // generate (quadrilateral) faces
  std::vector<GMesh::VertexHandle> face_vhandles;
  face_vhandles.clear();
  face_vhandles.push_back(vhandle[0]);
  face_vhandles.push_back(vhandle[1]);
  face_vhandles.push_back(vhandle[2]);
  face_vhandles.push_back(vhandle[3]);
  mesh.add_face(face_vhandles);

  face_vhandles.clear();
  face_vhandles.push_back(vhandle[7]);
  face_vhandles.push_back(vhandle[6]);
  face_vhandles.push_back(vhandle[5]);
  face_vhandles.push_back(vhandle[4]);
  mesh.add_face(face_vhandles);
  face_vhandles.clear();
  face_vhandles.push_back(vhandle[1]);
  face_vhandles.push_back(vhandle[0]);
  face_vhandles.push_back(vhandle[4]);
  face_vhandles.push_back(vhandle[5]);
  mesh.add_face(face_vhandles);
  face_vhandles.clear();
  face_vhandles.push_back(vhandle[2]);
  face_vhandles.push_back(vhandle[1]);
  face_vhandles.push_back(vhandle[5]);
  face_vhandles.push_back(vhandle[6]);
  mesh.add_face(face_vhandles);
  face_vhandles.clear();
  face_vhandles.push_back(vhandle[3]);
  face_vhandles.push_back(vhandle[2]);
  face_vhandles.push_back(vhandle[6]);
  face_vhandles.push_back(vhandle[7]);
  mesh.add_face(face_vhandles);
  face_vhandles.clear();
  face_vhandles.push_back(vhandle[0]);
  face_vhandles.push_back(vhandle[3]);
  face_vhandles.push_back(vhandle[7]);
  face_vhandles.push_back(vhandle[4]);
  mesh.add_face(face_vhandles);


    mesh.request_face_normals();
    // let the mesh update the normals
    mesh.update_normals();
    // dispose the face normals, as we don't need them anymore
    mesh.release_face_normals();

  return mesh;
}

GMesh openmesh_square() 
{
  GMesh mesh;
  mesh.request_vertex_texcoords2D();
  mesh.request_vertex_normals();
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

  // OpenMesh::VPropHandleT<OpenMesh::Vec2f> uv_property;
  // mesh.add_property(uv_property, "TexCoord");

  // mesh.property(uv_property, v0) = OpenMesh::Vec2f(0.0f, 0.0f);
  // mesh.property(uv_property, v1) = OpenMesh::Vec2f(1.0f, 0.0f);
  // mesh.property(uv_property, v2) = OpenMesh::Vec2f(1.0f, 1.0f);
  // mesh.property(uv_property, v3) = OpenMesh::Vec2f(0.0f, 1.0f);

  mesh.add_face(v0, v1, v2, v3);

  // mesh.request_face_normals();
  // // let the mesh update the normals
  // mesh.update_normals();
  // // dispose the face normals, as we don't need them anymore
  // mesh.release_face_normals();

  // std::cout << "Mesh has " << mesh.n_vertices() << " vertices and " << mesh.n_faces() << " faces." << std::endl;
  
  return mesh;
}
};  // namespace NED::openmeshutils