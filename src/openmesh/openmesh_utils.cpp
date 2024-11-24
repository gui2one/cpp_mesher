#include "openmesh_utils.h"
namespace NED::openmeshutils {


OpenMesh::PolyMesh_ArrayKernelT<> openmesh_cube() {
// typedef OpenMesh::PolyMesh_ArrayKernelT<> OMesh;    
  OMesh mesh;
  // generate vertices
  OMesh::VertexHandle vhandle[8];
  vhandle[0] = mesh.add_vertex(OMesh::Point(-1, -1, 1));
  vhandle[1] = mesh.add_vertex(OMesh::Point(1, -1, 1));
  vhandle[2] = mesh.add_vertex(OMesh::Point(1, 1, 1));
  vhandle[3] = mesh.add_vertex(OMesh::Point(-1, 1, 1));
  vhandle[4] = mesh.add_vertex(OMesh::Point(-1, -1, -1));
  vhandle[5] = mesh.add_vertex(OMesh::Point(1, -1, -1));
  vhandle[6] = mesh.add_vertex(OMesh::Point(1, 1, -1));
  vhandle[7] = mesh.add_vertex(OMesh::Point(-1, 1, -1));
  // generate (quadrilateral) faces
  std::vector<OMesh::VertexHandle> face_vhandles;
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
  // write mesh to output.obj

  return mesh;
}
};