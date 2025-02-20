#include "openmesh_utils.h"
namespace NED::openmeshutils {

void compute_normals(GMesh &mesh, bool reverse) {
  auto normal_prop_handle = mesh.add_dynamic_property("normal", PropertyType::PROP_VEC3F);

  mesh.request_vertex_normals();
  mesh.request_face_normals();
  // let the mesh update the normals
  mesh.update_normals();
  // mesh.update_vertex_normals();

  // dispose the face normals, as we don't need them anymore
  mesh.release_face_normals();

  // std::cout << mngr.getName() << std::endl;
  //  find "normal" prop
  for (auto prop : mesh.vertex_props) {
    if (prop.name == "normal") {
      for (GMesh::VertexIter v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it) {
        auto vh = *v_it;
        mesh.property(std::get<OpenMesh::VPropHandleT<OpenMesh::Vec3f>>(normal_prop_handle), *v_it) = mesh.normal(vh);
        if (reverse) {
          mesh.SetVertexPropValue<OpenMesh::Vec3f>(prop, vh,
                                                   mesh.GetVertexPropValue<OpenMesh::Vec3f>(prop, vh) * -1.0f);
        }
      }
    }
  }
}

void set_normals(GMesh &mesh, glm::vec3 normal) {
  mesh.request_vertex_normals();

  for (auto vh : mesh.vertices()) {
    mesh.set_normal(vh, OpenMesh::Vec3f(normal.x, normal.y, normal.z));
  }
}

GMesh triangulate(GMesh &mesh) {
  // Iterate over all faces and triangulate

  GMesh result = GMesh(mesh);
  for (GMesh::FaceIter f_it = result.faces_begin(); f_it != result.faces_end(); ++f_it) {
    if (result.is_valid_handle(*f_it)) {
      result.triangulate(*f_it);
    }
  }

  // Perform garbage collection to clean up the mesh
  result.garbage_collection();

  return result;
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
      auto vh = result.vertex_handle(vhB.idx() + (int)meshA.n_vertices());
      vhs.push_back(vh);
    }

    fhA = result.add_face(vhs);
  }

  return result;
}
static OpenSubdiv::Far::TopologyRefiner *createTopologyRefiner(int maxlevel, osd_DATA &osd_data, SubdivSchema schema) {
  using namespace OpenSubdiv;

  typedef Far::TopologyDescriptor Descriptor;

  Sdc::SchemeType type;  // = OpenSubdiv::Sdc::SCHEME_LOOP;
  switch (schema) {
    case SubdivSchema::CatmullClark:
      type = OpenSubdiv::Sdc::SCHEME_CATMARK;
      break;
    case SubdivSchema::Loop:
      type = OpenSubdiv::Sdc::SCHEME_LOOP;
      break;
    case SubdivSchema::Bilinear:
      type = OpenSubdiv::Sdc::SCHEME_BILINEAR;
      break;
  }
  Sdc::Options options;
  options.SetVtxBoundaryInterpolation(Sdc::Options::VTX_BOUNDARY_EDGE_ONLY);

  Descriptor desc;
  desc.numVertices = osd_data.nverts;
  desc.numFaces = osd_data.nfaces;
  desc.numVertsPerFace = osd_data.vertsperface.data();
  desc.vertIndicesPerFace = osd_data.vertIndices.data();

  // Instantiate a FarTopologyRefiner from the descriptor
  Far::TopologyRefiner *refiner = Far::TopologyRefinerFactory<Descriptor>::Create(
      desc, Far::TopologyRefinerFactory<Descriptor>::Options(type, options));

  // // Uniformly refine the topology up to 'maxlevel'
  // refiner->RefineUniform(Far::TopologyRefiner::UniformOptions(maxlevel));

  return refiner;
}
static osd_DATA mesh_to_osd_data(GMesh &mesh, bool do_triangulate = false) {
  // auto tris = triangulate(mesh);
  GMesh _mesh;
  if (do_triangulate) {
    _mesh = triangulate(mesh);
  } else {
    _mesh = mesh;
  }
  std::vector<osd_Point3> positions;
  // std::vector<osd_Point3> uvs;
  std::vector<int> vertsperface;
  for (OpenMesh::VertexHandle vh : _mesh.vertices()) {
    osd_Point3 pos;
    osd_Point3 uv;
    auto pt = _mesh.point(vh);
    pos.SetPoint(pt[0], pt[1], pt[2]);
    // uv.SetPoint(pt.t_coords.x, pt.t_coords.y, 0.0f);
    positions.push_back(pos);
    // uvs.push_back(uv);
  }

  // std::cout << "n faces : " << _mesh.n_faces() << std::endl;
  for (OpenMesh::FaceHandle fh : _mesh.faces()) {
    if (_mesh.is_valid_handle(fh) == false) continue;
    int num_vertices = 0;
    for (GMesh::FaceVertexIter fv_it = _mesh.fv_iter(fh); fv_it.is_valid(); ++fv_it) {
      ++num_vertices;
    }
    vertsperface.push_back((int)num_vertices);
  }
  std::vector<int> vertIndices;
  for (auto fh : _mesh.faces()) {
    for (GMesh::FaceVertexIter fv_it = _mesh.fv_iter(fh); fv_it.is_valid(); ++fv_it) {
      auto vh = *fv_it;
      vertIndices.push_back(vh.idx());
    }
  }

  std::vector<LinearFloatAttribute> linear_float_attributes;
  std::vector<LinearVec2Attribute> linear_vec2_attributes;
  std::vector<LinearVec3Attribute> linear_vec3_attributes;
  for (auto vp : _mesh.vertex_props) {
    std::cout << "collecting " << vp.name << " for subdivision" << std::endl;
    if (vp.type == PropertyType::PROP_VEC3F) {
      // std::cout << "Vec3f" << std::endl;
      std::vector<osd_Point3> values;
      for (auto vh : _mesh.vertices()) {
        auto prop_value = _mesh.GetVertexPropValue<OpenMesh::Vec3f>(vp, vh);
        osd_Point3 pt;
        pt.SetPoint(prop_value[0], prop_value[1], prop_value[2]);
        values.push_back(pt);
      }
      linear_vec3_attributes.push_back({vp.name, values});
    } else if (vp.type == PropertyType::PROP_VEC2F) {
      // std::cout << "Vec2f" << std::endl;
      std::vector<osd_Point2> values;
      for (auto vh : _mesh.vertices()) {
        auto prop_value = _mesh.GetVertexPropValue<OpenMesh::Vec2f>(vp, vh);
        osd_Point2 pt;
        pt.SetPoint(prop_value[0], prop_value[1]);
        values.push_back(pt);
      }
      linear_vec2_attributes.push_back({vp.name, values});
    }
  }
  return osd_DATA{positions,   (int)_mesh.n_vertices(), (int)_mesh.n_faces(),   vertsperface,
                  vertIndices, linear_float_attributes, linear_vec2_attributes, linear_vec3_attributes};
}

GMesh subdivide(GMesh &mesh, int maxlevel, SubdivSchema schema) {
  if (maxlevel <= 0) {
    return mesh;
  }
  using namespace OpenSubdiv;
  bool do_triangulate = false;
  if (schema == SubdivSchema::Loop) {
    do_triangulate = true;
  }
  auto osd_data = mesh_to_osd_data(mesh, do_triangulate);

  Far::TopologyRefiner *refiner = createTopologyRefiner(maxlevel, osd_data, schema);
  refiner->RefineUniform(Far::TopologyRefiner::UniformOptions(maxlevel));

  int nCoarseVerts = (int)osd_data.positions.size();
  int nFineVerts = refiner->GetLevel(maxlevel).GetNumVertices();
  int nTotalVerts = refiner->GetNumFaceVerticesTotal();
  int nTempVerts = nTotalVerts - nCoarseVerts - nFineVerts;
  // Allocate and initialize the primvar data for the original coarse vertices:
  std::vector<osd_Point3> coarsePosBuffer(nCoarseVerts);

  // Allocate intermediate and final storage to be populated:
  std::vector<osd_Point3> tempPosBuffer(nTempVerts);
  std::vector<osd_Point3> finePosBuffer(nFineVerts);

  // Interpolate all primvar data -- separate buffers can be populated on
  // separate threads if desired:
  osd_Point3 *srcPos = &coarsePosBuffer[0];
  osd_Point3 *dstPos = &tempPosBuffer[0];

  for (int i = 0; i < nCoarseVerts; ++i) {
    coarsePosBuffer[i] = osd_data.positions[i];
    // coarseClrBuffer[i] = osd_data.uvs[i];
  }

  // vec3 linear attributes
  std::vector<std::vector<osd_Point3>> coarse_linear_vec3_attributes(osd_data.linear_vec3_attributes.size());

  std::vector<std::vector<osd_Point3>> tempVec3AttrBuffers(osd_data.linear_vec3_attributes.size());
  std::vector<std::vector<osd_Point3>> fineVec3AttrBuffers(osd_data.linear_vec3_attributes.size());

  std::vector<osd_Point3 *> srcVec3AttrArrays(osd_data.linear_vec3_attributes.size());
  std::vector<osd_Point3 *> dstVec3AttrArrays(osd_data.linear_vec3_attributes.size());
  for (int i = 0; i < osd_data.linear_vec3_attributes.size(); i++) {
    std::vector<osd_Point3> values(nCoarseVerts);
    for (int j = 0; j < nCoarseVerts; j++) {
      values[j] = osd_data.linear_vec3_attributes[i].values[j];
    }
    coarse_linear_vec3_attributes[i] = values;
  }
  // init arrays
  for (int i = 0; i < osd_data.linear_vec3_attributes.size(); i++) {
    tempVec3AttrBuffers[i] = std::vector<osd_Point3>(nTempVerts);
    fineVec3AttrBuffers[i] = std::vector<osd_Point3>(nFineVerts);
    osd_Point3 *src = &coarse_linear_vec3_attributes[i][0];
    osd_Point3 *dst = &tempVec3AttrBuffers[i][0];

    srcVec3AttrArrays[i] = src;
    dstVec3AttrArrays[i] = dst;
    LOG_INFO("init arrays for {} property", osd_data.linear_vec3_attributes[i].name);
  }

  // vec2 linear attributes
  std::vector<std::vector<osd_Point2>> coarse_linear_vec2_attributes(osd_data.linear_vec2_attributes.size());

  std::vector<std::vector<osd_Point2>> tempVec2AttrBuffers(osd_data.linear_vec2_attributes.size());
  std::vector<std::vector<osd_Point2>> fineVec2AttrBuffers(osd_data.linear_vec2_attributes.size());

  std::vector<osd_Point2 *> srcVec2AttrArrays(osd_data.linear_vec2_attributes.size());
  std::vector<osd_Point2 *> dstVec2AttrArrays(osd_data.linear_vec2_attributes.size());
  for (int i = 0; i < osd_data.linear_vec2_attributes.size(); i++) {
    std::vector<osd_Point2> values(nCoarseVerts);
    for (int j = 0; j < nCoarseVerts; j++) {
      values[j] = osd_data.linear_vec2_attributes[i].values[j];
    }
    coarse_linear_vec2_attributes[i] = values;
  }
  // init arrays
  for (int i = 0; i < osd_data.linear_vec2_attributes.size(); i++) {
    tempVec2AttrBuffers[i] = std::vector<osd_Point2>(nTempVerts);
    fineVec2AttrBuffers[i] = std::vector<osd_Point2>(nFineVerts);
    osd_Point2 *src = &coarse_linear_vec2_attributes[i][0];
    osd_Point2 *dst = &tempVec2AttrBuffers[i][0];

    srcVec2AttrArrays[i] = src;
    dstVec2AttrArrays[i] = dst;
    LOG_INFO("init arrays for {} property", osd_data.linear_vec2_attributes[i].name);
  }

  Far::PrimvarRefiner primvarRefiner(*refiner);

  for (int level = 1; level < maxlevel; ++level) {
    primvarRefiner.Interpolate(level, srcPos, dstPos);
    srcPos = dstPos, dstPos += refiner->GetLevel(level).GetNumVertices();

    for (int i = 0; i < osd_data.linear_vec3_attributes.size(); i++) {
      primvarRefiner.InterpolateVarying(level, srcVec3AttrArrays[i], dstVec3AttrArrays[i]);
      srcVec3AttrArrays[i] = dstVec3AttrArrays[i], dstVec3AttrArrays[i] += refiner->GetLevel(level).GetNumVertices();
      LOG_INFO("refine level: {}, property: {}", level, osd_data.linear_vec3_attributes[i].name);
    }

    for (int i = 0; i < osd_data.linear_vec2_attributes.size(); i++) {
      primvarRefiner.InterpolateVarying(level, srcVec2AttrArrays[i], dstVec2AttrArrays[i]);
      srcVec2AttrArrays[i] = dstVec2AttrArrays[i], dstVec2AttrArrays[i] += refiner->GetLevel(level).GetNumVertices();
      LOG_INFO("refine level: {}, property: {}", level, osd_data.linear_vec2_attributes[i].name);
    }
  }

  // Interpolate the last level into the separate buffers for our final data:
  primvarRefiner.Interpolate(maxlevel, srcPos, finePosBuffer);

  for (int i = 0; i < osd_data.linear_vec3_attributes.size(); i++) {
    LOG_INFO("refine level: {}, property: {}", maxlevel, osd_data.linear_vec3_attributes[i].name);
    primvarRefiner.InterpolateVarying(maxlevel, srcVec3AttrArrays[i], fineVec3AttrBuffers[i]);
  }
  for (int i = 0; i < osd_data.linear_vec2_attributes.size(); i++) {
    LOG_INFO("refine level: {}, property: {}", maxlevel, osd_data.linear_vec2_attributes[i].name);
    primvarRefiner.InterpolateVarying(maxlevel, srcVec2AttrArrays[i], fineVec2AttrBuffers[i]);
  }

  //  // Assuming you have a pointer or reference to the refiner:
  Far::TopologyLevel const &refinedLevel = refiner->GetLevel(maxlevel);

  int numFaces = refinedLevel.GetNumFaces();

  // Apply to a GMesh
  GMesh newMesh;
  newMesh.reserve(nFineVerts, 0, numFaces);

  for (int i = 0; i < nFineVerts; ++i) {
    newMesh.add_vertex(
        GMesh::Point(finePosBuffer[i].GetPoint()[0], finePosBuffer[i].GetPoint()[1], finePosBuffer[i].GetPoint()[2]));
  }

  // TODO : finally add refined vertex properties
  for (int i = 0; i < osd_data.linear_vec3_attributes.size(); i++) {
    auto prop_name = osd_data.linear_vec3_attributes[i].name;
    newMesh.add_dynamic_property(prop_name, PropertyType::PROP_VEC3F);

    auto prop_result = newMesh.GetVertexProp(prop_name);
    if (prop_result.success) {
      auto prop_handle = std::get<OpenMesh::VPropHandleT<OpenMesh::Vec3f>>(prop_result.prop.handle);
      for (auto v_it = newMesh.vertices_begin(); v_it != newMesh.vertices_end(); ++v_it) {
        auto idx = v_it->idx();
        newMesh.property(prop_handle, *v_it) = {fineVec3AttrBuffers[i][idx].GetPoint()[0],
                                                fineVec3AttrBuffers[i][idx].GetPoint()[1],
                                                fineVec3AttrBuffers[i][idx].GetPoint()[2]};
      }
    }
  }
  for (int i = 0; i < osd_data.linear_vec2_attributes.size(); i++) {
    auto prop_name = osd_data.linear_vec2_attributes[i].name;
    newMesh.add_dynamic_property(prop_name, PropertyType::PROP_VEC2F);

    auto prop_result = newMesh.GetVertexProp(prop_name);
    if (prop_result.success) {
      auto prop_handle = std::get<OpenMesh::VPropHandleT<OpenMesh::Vec2f>>(prop_result.prop.handle);
      for (auto v_it = newMesh.vertices_begin(); v_it != newMesh.vertices_end(); ++v_it) {
        auto idx = v_it->idx();
        newMesh.property(prop_handle, *v_it) = {fineVec2AttrBuffers[i][idx].GetPoint()[0],
                                                fineVec2AttrBuffers[i][idx].GetPoint()[1]};
      }
    }
  }
  for (int i = 0; i < numFaces; ++i) {
    auto indices = refinedLevel.GetFaceVertices(i);
    std::vector<OpenMesh::VertexHandle> vhs;
    for (int j = 0; j < indices.size(); ++j) {
      auto vh = newMesh.vertex_handle(indices[j]);
      vhs.push_back(vh);
    }

    newMesh.add_face(vhs.data(), vhs.size());
  }

  delete refiner;

  return newMesh;
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

GMesh transform(GMesh &mesh, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale_, TRANSFORM_ORDER tr_order,
                AXIS_ORDER axis_order) {
  auto result = GMesh(mesh);
  switch (tr_order) {
    case TRANSFORM_ORDER::TRS:
      result = translate(result, pos);
      result = rotate(result, rot, axis_order);
      result = scale(result, scale_);
      break;
    case TRANSFORM_ORDER::TSR:
      result = translate(result, pos);
      result = scale(result, scale_);
      result = rotate(result, rot, axis_order);
      break;
    case TRANSFORM_ORDER::RTS:
      result = rotate(result, rot, axis_order);
      result = translate(result, pos);
      result = scale(result, scale_);
      break;
    case TRANSFORM_ORDER::RST:
      result = rotate(result, rot, axis_order);
      result = scale(result, scale_);
      result = translate(result, pos);
      break;
    case TRANSFORM_ORDER::STR:
      result = scale(result, scale_);
      result = translate(result, pos);
      result = rotate(result, rot, axis_order);
      break;
    case TRANSFORM_ORDER::SRT:
      result = scale(result, scale_);
      result = rotate(result, rot, axis_order);
      result = translate(result, pos);
      break;
    default:
      break;
  }

  return result;
}

GMesh noise_displace(GMesh &mesh, NoiseParamsStruct params) {
  GMesh result = GMesh(mesh);
  FastNoise::SmartNode<FastNoise::Simplex> fnSimplex = FastNoise::New<FastNoise::Simplex>();
  FastNoise::SmartNode<FastNoise::Perlin> fnPerlin = FastNoise::New<FastNoise::Perlin>();
  FastNoise::SmartNode<FastNoise::CellularDistance> fnCellular = FastNoise::New<FastNoise::CellularDistance>();
  auto fnFractal = FastNoise::New<FastNoise::FractalFBm>();

  switch (params.noise_type) {
    case NoiseType::Simplex:
      fnFractal->SetSource(fnSimplex);
      break;
    case NoiseType::Perlin:
      fnFractal->SetSource(fnPerlin);
      break;
    case NoiseType::Cellular:
      fnFractal->SetSource(fnCellular);
      break;
  }
  // fnFractal->SetSource(fnSimplex);
  fnFractal->SetOctaveCount((int)params.octaves);
  fnFractal->SetGain(params.gain);
  fnFractal->SetLacunarity(params.lacunarity);
  fnFractal->SetWeightedStrength(params.weightedStrength);

  for (auto vh : result.vertices()) {
    auto pt = result.point(vh);
    auto pos = glm::vec3(pt[0], pt[1], pt[2]);
    float noise_val = fnFractal->GenSingle3D((pos.x + params.offset.x) * params.frequency,
                                             (pos.y + params.offset.y) * params.frequency,
                                             (pos.z + params.offset.z) * params.frequency, params.seed);

    auto normal_result = mesh.GetVertexProp("normal");
    if (normal_result.success == true) {
      auto normal_value =
          mesh.property(std::get<OpenMesh::VPropHandleT<OpenMesh::Vec3f>>(normal_result.prop.handle), vh);
      result.set_point(vh, GMesh::Point(glm::value_ptr(pos)) + normal_value * params.amplitude * noise_val);
    }
  }

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

GMesh openmesh_square(bool compute_uvs) {
  GMesh mesh;

  auto pt0 = mesh.add_vertex(GMesh::Point(0, 0, 0));
  auto pt1 = mesh.add_vertex(GMesh::Point(1, 0, 0));
  auto pt2 = mesh.add_vertex(GMesh::Point(1, 1, 0));
  auto pt3 = mesh.add_vertex(GMesh::Point(0, 1, 0));

  if (compute_uvs) {
    auto uv_prop = mesh.add_dynamic_property("uv", PropertyType::PROP_VEC2F);

    mesh.SetVertexPropValue<OpenMesh::Vec2f>(uv_prop, pt0, OpenMesh::Vec2f(0.0f, 0.0f));
    mesh.SetVertexPropValue<OpenMesh::Vec2f>(uv_prop, pt1, OpenMesh::Vec2f(1.0f, 0.0f));
    mesh.SetVertexPropValue<OpenMesh::Vec2f>(uv_prop, pt2, OpenMesh::Vec2f(1.0f, 1.0f));
    mesh.SetVertexPropValue<OpenMesh::Vec2f>(uv_prop, pt3, OpenMesh::Vec2f(0.0f, 1.0f));
  }

  auto f_handle = mesh.add_face(pt0, pt1, pt2, pt3);

  return mesh;
}
GMesh openmesh_grid(float width, float length, uint32_t _cols, uint32_t _rows, bool compute_uvs) {
  GMesh result;
  uint32_t cols = _cols + 1;
  uint32_t rows = _rows + 1;
  std::vector<GMesh::Point> points;

  if (compute_uvs) {
    result.add_dynamic_property("uv", PropertyType::PROP_VEC2F);
  }

  for (uint32_t i = 0; i < rows; i++) {
    for (uint32_t j = 0; j < cols; j++) {
      GMesh::Point p;

      float u = j / (float)(cols - 1);
      float v = i / (float)(rows - 1);

      p[0] = u * width - (width / 2.0f);
      p[1] = v * length - (length / 2.0f);
      p[2] = 0.0f;

      auto vh = result.add_vertex(p);
      if (compute_uvs) {
        result.SetVertexPropValue(result.GetVertexProp("uv").prop, vh, OpenMesh::Vec2f(u, v));
      }
      // points.push_back(p);
    }
  }

  /*for (const auto &pt : points) {
    result.add_vertex(pt);
  }*/

  for (uint32_t i = 0; i < rows - 1; i++) {
    for (uint32_t j = 0; j < cols - 1; j++) {
      uint32_t id0 = i * cols + j;
      uint32_t id1 = i * cols + j + 1;
      uint32_t id2 = (i + 1) * cols + j + 1;
      uint32_t id3 = (i + 1) * cols + j;
      auto vh0 = result.vertex_handle(id0);
      auto vh1 = result.vertex_handle(id1);
      auto vh2 = result.vertex_handle(id2);
      auto vh3 = result.vertex_handle(id3);
      result.add_face({vh0, vh1, vh2, vh3});
    }
  }
  // result.SetFaces(faces);

  return result;
}
GMesh openmesh_torus(float radius1, float radius2, uint32_t _cols, uint32_t _rows) {
  GMesh result;
  uint32_t cols = _cols;
  uint32_t rows = _rows;
  result = openmesh_grid(1.0f, 1.0f, cols, rows);

  for (auto vh : result.vertices()) {
    auto pt = result.point(vh);
    float v = pt[0] * glm::pi<float>() * 2.0f;
    float u = (1.0f - pt[1]) * glm::pi<float>() * 2.0f;
    pt[0] = (radius1 + radius2 * cosf(v)) * cosf(u);
    pt[1] = (radius1 + radius2 * cosf(v)) * sinf(u);
    pt[2] = radius2 * sin(v);

    result.set_point(vh, pt);
  }
  return result;
}

void list_vertex_properties(GMesh &mesh) {
  std::cout << "Listing vertex properties:" << std::endl;
  for (auto &prop : mesh.vertex_props) {
    std::cout << std::format("\t{}({})", prop.name, prop.type_name) << std::endl;
  }

  std::cout << "----------------------------" << std::endl;
}
};  // namespace NED::openmeshutils