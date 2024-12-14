#include "openmesh_utils.h"
namespace NED::openmeshutils {

void compute_normals(GMesh &mesh) {
  mesh.request_vertex_normals();
  mesh.request_face_normals();
  // let the mesh update the normals
  mesh.update_normals();
  // dispose the face normals, as we don't need them anymore
  mesh.release_face_normals();
}

void set_normals(GMesh &mesh, glm::vec3 normal) {
  mesh.request_vertex_normals();

  for (auto vh : mesh.vertices()) {
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

GMesh combine(GMesh &meshA, GMesh &meshB) {
  // Keep track of the vertex mapping from meshB to meshA
  std::vector<GMesh::VertexHandle> vertexMap;

  GMesh result;
  result = GMesh(meshA);
  // Step 1: Add all vertices from meshB to meshA
  for (const auto &vhA : meshB.vertices()) {
    GMesh::Point point = meshB.point(vhA);
    GMesh::VertexHandle vhA = result.add_vertex(point);
    vertexMap.push_back(vhA);
  }

  // Step 2: Add all faces from meshB to meshA
  for (const auto &fhB : meshB.faces()) {
    GMesh::FaceHandle fhA = GMesh::FaceHandle(fhB);
    // std::cout << "Face index : " << fhA.idx() << std::endl;

    // Get the vertices of the face in meshB
    GMesh::FaceVertexIter fv_it = meshB.fv_iter(fhB);
    std::vector<GMesh::VertexHandle> vhs;
    for (GMesh::FaceVertexIter fv_it = meshB.fv_iter(fhB); fv_it != meshB.fv_end(fhB); ++fv_it) {
      GMesh::VertexHandle vhB = *fv_it;
      // std::cout << "\tVertex index : " << vhB.idx() << std::endl;
      auto vh = result.vertex_handle(vhB.idx() + meshA.n_vertices());
      vhs.push_back(vh);
    }

    // if (vhs.size() > 2) {
    fhA = result.add_face(vhs);
    //}
    // result.add_face();
  }

  return result;
}

GMesh translate(GMesh &mesh, glm::vec3 offset) {
  GMesh result = GMesh(mesh);

  for (auto vh : result.vertices()) {
    result.set_point(vh, result.point(vh) + GMesh::Point(glm::value_ptr(offset)));
  }
  return result;
}

GMesh scale(GMesh &mesh, glm::vec3 scale) {
  GMesh result = GMesh(mesh);

  for (auto vh : result.vertices()) {
    result.set_point(vh, result.point(vh) * GMesh::Point(glm::value_ptr(scale)));
  }
  return result;
}

GMesh rotate(GMesh &mesh, glm::vec3 radians, AXIS_ORDER axis_order) {
  GMesh result = GMesh(mesh);

  switch (axis_order) {
    case AXIS_ORDER::XYZ:
      for (auto vh : result.vertices()) {
        auto pt = result.point(vh);
        auto pos = glm::vec3(pt[0], pt[1], pt[2]);

        pos = glm::rotateX(pos, radians.x);
        pos = glm::rotateY(pos, radians.y);
        pos = glm::rotateZ(pos, radians.z);
        result.set_point(vh, GMesh::Point(glm::value_ptr(pos)));
      }
      break;
    case AXIS_ORDER::XZY:
      for (auto vh : result.vertices()) {
        auto pt = result.point(vh);
        auto pos = glm::vec3(pt[0], pt[1], pt[2]);

        pos = glm::rotateX(pos, radians.x);
        pos = glm::rotateZ(pos, radians.z);
        pos = glm::rotateY(pos, radians.y);
        result.set_point(vh, GMesh::Point(glm::value_ptr(pos)));
      }
      break;
    case AXIS_ORDER::YXZ:
      for (auto vh : result.vertices()) {
        auto pt = result.point(vh);
        auto pos = glm::vec3(pt[0], pt[1], pt[2]);

        pos = glm::rotateY(pos, radians.y);
        pos = glm::rotateX(pos, radians.x);
        pos = glm::rotateZ(pos, radians.z);
        result.set_point(vh, GMesh::Point(glm::value_ptr(pos)));
      }
      break;
    case AXIS_ORDER::YZX:
      for (auto vh : result.vertices()) {
        auto pt = result.point(vh);
        auto pos = glm::vec3(pt[0], pt[1], pt[2]);

        pos = glm::rotateY(pos, radians.y);
        pos = glm::rotateZ(pos, radians.z);
        pos = glm::rotateX(pos, radians.x);
        result.set_point(vh, GMesh::Point(glm::value_ptr(pos)));
      }
      break;
    case AXIS_ORDER::ZXY:
      for (auto vh : result.vertices()) {
        auto pt = result.point(vh);
        auto pos = glm::vec3(pt[0], pt[1], pt[2]);

        pos = glm::rotateZ(pos, radians.z);
        pos = glm::rotateX(pos, radians.x);
        pos = glm::rotateY(pos, radians.y);
        result.set_point(vh, GMesh::Point(glm::value_ptr(pos)));
      }
      break;
    case AXIS_ORDER::ZYX:
      for (auto vh : result.vertices()) {
        auto pt = result.point(vh);
        auto pos = glm::vec3(pt[0], pt[1], pt[2]);

        pos = glm::rotateZ(pos, radians.z);
        pos = glm::rotateY(pos, radians.y);
        pos = glm::rotateX(pos, radians.x);
        result.set_point(vh, GMesh::Point(glm::value_ptr(pos)));
      }
      break;
    default:
      break;
  }
  // result.set_point(vh, result.point(vh) * GMesh::Point(glm::value_ptr(radians)));

  return result;
}

GMesh openmesh_cube() {
  GMesh mesh;

  auto pt0 = mesh.add_vertex(GMesh::Point(-1, -1, 1));
  auto pt1 = mesh.add_vertex(GMesh::Point(1, -1, 1));
  auto pt2 = mesh.add_vertex(GMesh::Point(1, 1, 1));
  auto pt3 = mesh.add_vertex(GMesh::Point(-1, 1, 1));
  auto pt4 = mesh.add_vertex(GMesh::Point(-1, -1, -1));
  auto pt5 = mesh.add_vertex(GMesh::Point(1, -1, -1));
  auto pt6 = mesh.add_vertex(GMesh::Point(1, 1, -1));
  auto pt7 = mesh.add_vertex(GMesh::Point(-1, 1, -1));

  mesh.add_face(pt0, pt1, pt2, pt3);  // Front face
  mesh.add_face(pt7, pt6, pt5, pt4);  // Back face
  mesh.add_face(pt0, pt4, pt5, pt1);  // Bottom face
  mesh.add_face(pt2, pt6, pt7, pt3);  // Top face
  mesh.add_face(pt0, pt3, pt7, pt4);  // Left face
  mesh.add_face(pt1, pt5, pt6, pt2);  // Right face

  return mesh;
}

GMesh openmesh_square() {
  GMesh mesh;

  auto pt0 = mesh.add_vertex(GMesh::Point(0, 0, 0));
  auto pt1 = mesh.add_vertex(GMesh::Point(1, 0, 0));
  auto pt2 = mesh.add_vertex(GMesh::Point(1, 1, 0));
  auto pt3 = mesh.add_vertex(GMesh::Point(0, 1, 0));

  mesh.request_vertex_texcoords2D();
  mesh.set_texcoord2D(pt0, OpenMesh::Vec2f(0.0f, 0.0f));
  mesh.set_texcoord2D(pt1, OpenMesh::Vec2f(1.0f, 0.0f));
  mesh.set_texcoord2D(pt2, OpenMesh::Vec2f(1.0f, 1.0f));
  mesh.set_texcoord2D(pt3, OpenMesh::Vec2f(0.0f, 1.0f));

  auto f_handle = mesh.add_face(pt0, pt1, pt2, pt3);

  return mesh;
}
};  // namespace NED::openmeshutils