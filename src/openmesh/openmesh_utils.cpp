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

void triangulate(GMesh &mesh) {

    // Iterate over all faces and triangulate
  for (GMesh::FaceIter f_it = mesh.faces_begin(); f_it != mesh.faces_end(); ++f_it) {
    if (mesh.is_valid_handle(*f_it)) {
      mesh.triangulate(*f_it);
    }
  }

  // Perform garbage collection to clean up the mesh
  mesh.garbage_collection();
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


  auto v00 = mesh.add_vertex_from_point(pt0);
  auto v01 = mesh.add_vertex_from_point(pt1);
  auto v02 = mesh.add_vertex_from_point(pt2);
  auto v03 = mesh.add_vertex_from_point(pt3);
  mesh.add_face(v00, v01, v02, v03);

  auto v10 = mesh.add_vertex_from_point(pt7);
  auto v11 = mesh.add_vertex_from_point(pt6);
  auto v12 = mesh.add_vertex_from_point(pt5);
  auto v13 = mesh.add_vertex_from_point(pt4);
  mesh.add_face(v10, v11, v12, v13);
  
  auto v20 = mesh.add_vertex_from_point(pt1);
  auto v21 = mesh.add_vertex_from_point(pt0);
  auto v22 = mesh.add_vertex_from_point(pt4);
  auto v23 = mesh.add_vertex_from_point(pt5);
  mesh.add_face(v20, v21, v22, v23);

  auto v30 = mesh.add_vertex_from_point(pt2);
  auto v31 = mesh.add_vertex_from_point(pt1);
  auto v32 = mesh.add_vertex_from_point(pt5);
  auto v33 = mesh.add_vertex_from_point(pt6);
  mesh.add_face(v30, v31, v32, v33);

  auto v40 = mesh.add_vertex_from_point(pt3);
  auto v41 = mesh.add_vertex_from_point(pt2);
  auto v42 = mesh.add_vertex_from_point(pt6);
  auto v43 = mesh.add_vertex_from_point(pt7);
  mesh.add_face(v40, v41, v42, v43);

  auto v50 = mesh.add_vertex_from_point(pt0);
  auto v51 = mesh.add_vertex_from_point(pt3);
  auto v52 = mesh.add_vertex_from_point(pt7);
  auto v53 = mesh.add_vertex_from_point(pt4);
  mesh.add_face(v50, v51, v52, v53);
  


  return mesh;
}

GMesh openmesh_square() 
{
  GMesh mesh;
  

  auto pt0 = mesh.add_point(GMesh::Point(0, 0, 0));
  auto pt1 = mesh.add_point(GMesh::Point(1, 0, 0));
  auto pt2 = mesh.add_point(GMesh::Point(1, 1, 0));
  auto pt3 = mesh.add_point(GMesh::Point(0, 1, 0));

  auto v0 = mesh.add_vertex_from_point(pt0);
  auto v1 = mesh.add_vertex_from_point(pt1);
  auto v2 = mesh.add_vertex_from_point(pt2);
  auto v3 = mesh.add_vertex_from_point(pt3);

  mesh.request_vertex_texcoords2D();
  mesh.set_texcoord2D(v0, OpenMesh::Vec2f(0.0f, 0.0f));
  mesh.set_texcoord2D(v1, OpenMesh::Vec2f(1.0f, 0.0f));
  mesh.set_texcoord2D(v2, OpenMesh::Vec2f(1.0f, 1.0f));
  mesh.set_texcoord2D(v3, OpenMesh::Vec2f(0.0f, 1.0f));

  auto f_handle = mesh.add_face(v0, v1, v2, v3);

  return mesh;
}
};  // namespace NED::openmeshutils