#include "mesh_utils.h"


namespace msh::meshutils {

Mesh generate_grid(float width, float length, uint32_t _cols, uint32_t _rows) {

  Mesh result;
  uint32_t cols = _cols + 1;
  uint32_t rows = _rows + 1;
  std::vector<Point> points;
  std::vector<Vertex> vertices;
  for (uint32_t i = 0; i < rows; i++) {
    for (uint32_t j = 0; j < cols; j++) {
      Point p;
      Vertex vtx;
      vtx.point_id = i * cols + j;
      float u = j / (float)(cols - 1);
      float v = i / (float)(rows - 1);
      p.position.x = u * width;
      p.position.y = v * length;
      p.position.z = 0.0f;

      p.t_coords.x = u;
      p.t_coords.y = v;

      points.push_back(p);
      vertices.push_back(vtx);
    }
  }
  result.SetPoints(points);
  result.SetVertices(vertices);

  std::vector<Face> faces;
  for (uint32_t i = 0; i < rows - 1; i++) {
    for (uint32_t j = 0; j < cols - 1; j++) {
      uint32_t id0 = i * cols + j;
      uint32_t id1 = i * cols + j + 1;
      uint32_t id2 = (i + 1) * cols + j + 1;
      uint32_t id3 = (i + 1) * cols + j;
      faces.push_back(Face({id0, id1, id2, id3}));
    }
  }
  result.SetFaces(faces);

  return result;
}

Mesh generate_tube(float radius1, float radius2, float height, uint32_t cols,
                  uint32_t rows) {
  Mesh result;
  result = generate_grid(1.0f, 1.0f, cols, rows);

  for (auto &pt : result.GetPoints()) {
    glm::vec3 new_pos;
    float radius = std::lerp(radius1, radius2, pt.position.y);
    // float radius = radius1;
    new_pos.x = cosf(pt.position.x * glm::pi<float>() * 2.0f) * radius;
    new_pos.z = pt.position.y * height;
    new_pos.y = sinf(pt.position.x * glm::pi<float>() * 2.0f) * radius;

    pt.position = new_pos;
  }
  return result;
}

Mesh generate_tube2(float radius, float height, uint32_t _cols, uint32_t _rows) {
  Mesh result;
  uint32_t cols = _cols;
  uint32_t rows = _rows + 1;
  std::vector<Point> points;
  std::vector<Vertex> vertices;
  for (uint32_t i = 0; i < rows; i++) {
    for (uint32_t j = 0; j < cols; j++) {
      Point p;
      Vertex vtx;
      vtx.point_id = i * cols + j;
      float u = j / (float)(cols);
      float v = i / (float)(rows - 1);
      p.position.x = cosf(u * glm::pi<float>() * 2.0f) * radius;
      p.position.y = sinf(u * glm::pi<float>() * 2.0f) * radius;
      p.position.z = v * height;

      p.t_coords.x = u;
      p.t_coords.y = v;

      points.push_back(p);
      vertices.push_back(vtx);
    }
  }
  result.SetPoints(points);
  result.SetVertices(vertices);

  std::vector<Face> faces;
  for (uint32_t i = 0; i < rows - 1; i++) {
    for (uint32_t j = 0; j < cols; j++) {
      if (j < cols - 1) {
        uint32_t id0 = i * cols + j;
        uint32_t id1 = i * cols + j + 1;
        uint32_t id2 = (i + 1) * cols + j + 1;
        uint32_t id3 = (i + 1) * cols + j;
        // LOG_INFO("OK indices {} -- {} -- {} -- {}",id0,id1,id2,id3);
        faces.push_back(Face({id0, id1, id2, id3}));
      } else {
        uint32_t id0 = i * cols + j;
        uint32_t id1 = i * cols;
        uint32_t id2 = (i + 1) * cols;
        uint32_t id3 = (i + 1) * cols + j;
        // LOG_ERROR("wtf indices {} -- {} -- {} -- {}",id0,id1,id2,id3);

        faces.push_back(Face({id0, id1, id2, id3}));
      }
    }
  }
  result.SetFaces(faces);
  return result;
}

Mesh generate_torus(float radius1, float radius2, uint32_t _cols, uint32_t _rows) {
  Mesh result;
  uint32_t cols = _cols;
  uint32_t rows = _rows;
  result = msh::meshutils::generate_grid(1.0f, 1.0f, cols, rows);

  for (auto &pt : result.GetPoints()) {
    float v = pt.position.x * glm::pi<float>() * 2.0f;
    float u = pt.position.y * glm::pi<float>() * 2.0f;
    pt.position.x = (radius1 + radius2 * cosf(v)) * cosf(u);
    pt.position.y = (radius1 + radius2 * cosf(v)) * sinf(u);
    pt.position.z = radius2 * sin(v);
  }
  return result;
}

Mesh generate_disc(float radius, uint32_t segs) {
  Mesh result;
  std::vector<Point> pts;
  std::vector<Vertex> vertices;
  for (uint32_t i = 0; i < segs; i++) {
    Point p;
    Vertex vtx;
    vtx.point_id = i;
    float pos_1d = i / (float)(segs);
    float u = cosf(pos_1d * glm::pi<float>() * 2.0f);
    float v = sinf(pos_1d * glm::pi<float>() * 2.0f);
    p.position.x = u * radius;
    p.position.y = v * radius;
    p.position.z = 0.0f;
    p.t_coords.x = u / 2.0f + 0.5f;
    p.t_coords.y = v / 2.0f + 0.5f;

    pts.push_back(p);
    vertices.push_back(vtx);
  }
  Face face;
  for (uint32_t i = 0; i < segs; i++) {
    face.GetVerticesIndex().push_back(i);
  }

  result.SetPoints(pts);
  result.SetVertices(vertices);
  result.SetFaces({face});
  return result;
}

/**
 * Merges two meshes into a single mesh.
 *
 * @param mesh1 The first mesh to merge.
 * @param mesh2 The second mesh to merge.
 *
 * @return A new mesh that contains all points and faces from both input meshes.
 */
Mesh merge(Mesh &mesh1, Mesh &mesh2) {
  Mesh merged;
  std::vector<Point> pts(mesh1.GetPoints().size());
  std::copy(mesh1.GetPoints().begin(), mesh1.GetPoints().end(), pts.begin());

  std::vector<Vertex> vertices(mesh1.GetVertices().size());
  std::copy(mesh1.GetVertices().begin(), mesh1.GetVertices().end(),
            vertices.begin());

  pts.insert(pts.end(), mesh2.GetPoints().begin(), mesh2.GetPoints().end());

  std::vector<Face> faces(mesh1.GetFaces().size());
  std::vector<Face> mesh2_faces(mesh2.GetFaces().size());
  std::copy(mesh1.GetFaces().begin(), mesh1.GetFaces().end(), faces.begin());
  std::copy(mesh2.GetFaces().begin(), mesh2.GetFaces().end(),
            mesh2_faces.begin());

  for (auto &face : mesh2_faces) {
    for (auto &idx : face.GetVerticesIndex()) {
      idx += (uint32_t)mesh1.GetPoints().size();
    }
  }
  faces.insert(faces.end(), mesh2_faces.begin(), mesh2_faces.end());

  std::vector<Vertex> mesh2_vertices2(mesh2.GetVertices().size());
  std::copy(mesh2.GetVertices().begin(), mesh2.GetVertices().end(),
            mesh2_vertices2.begin());
  for (auto &vert : mesh2_vertices2) {
    vert.point_id += (uint32_t)mesh1.GetPoints().size();
  }
  vertices.insert(vertices.end(), mesh2_vertices2.begin(),
                  mesh2_vertices2.end());

  merged.SetPoints(pts);
  merged.SetVertices(vertices);
  merged.SetFaces(faces);
  return merged;
}

Mesh triangulate(Mesh &mesh)
{
  Mesh tri_mesh = mesh;
  std::vector<Face> new_faces;

  for (size_t i = 0; i < tri_mesh.GetFaces().size(); i++)
  {
  	uint32_t num_verts = uint32_t(tri_mesh.GetFaces()[i].GetNumVerticesIndex());

  	uint32_t num_tris = num_verts - 3 + 1;

  	for (size_t tri = 0; tri < num_tris; tri++)
  	{

  		Face face;
  		face.SetVerticesIndex(
  			{
  				tri_mesh.GetFaces()[i].GetVertexIndex(0),
  				tri_mesh.GetFaces()[i].GetVertexIndex(uint32_t(tri) + 1),
          tri_mesh.GetFaces()[i].GetVertexIndex(uint32_t(tri) + 2)
  			}
  		);

  		new_faces.push_back(face);

  	}

  }

  tri_mesh.SetFaces(new_faces);  
  return tri_mesh;
}

static OpenSubdiv::Far::TopologyRefiner *
createTopologyRefiner(int maxlevel, osd_DATA& osd_data, SubdivSchema schema) {
    using namespace OpenSubdiv;
    
    typedef Far::TopologyDescriptor Descriptor;

    Sdc::SchemeType type; // = OpenSubdiv::Sdc::SCHEME_LOOP;
    switch(schema) {
      case SubdivSchema::CatmullClark:
        std::cout << "Schema to Catmull-Clark" << std::endl;
        
        type = OpenSubdiv::Sdc::SCHEME_CATMARK;
        break;
      case SubdivSchema::Loop:
        type = OpenSubdiv::Sdc::SCHEME_LOOP;
        std::cout << "Schema to Loop" << std::endl;
        break;
      case SubdivSchema::Bilinear:
        type = OpenSubdiv::Sdc::SCHEME_BILINEAR;
        std::cout << "Schema to Bilinear" << std::endl;
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
    Far::TopologyRefiner * refiner =
        Far::TopologyRefinerFactory<Descriptor>::Create(desc,
            Far::TopologyRefinerFactory<Descriptor>::Options(type, options));

    // // Uniformly refine the topology up to 'maxlevel'
    // refiner->RefineUniform(Far::TopologyRefiner::UniformOptions(maxlevel));

    return refiner;
}

osd_DATA mesh_to_osd_data(Mesh& mesh, bool do_triangulate = false ) {
  // auto tris = triangulate(mesh);
  Mesh _mesh;
  if(do_triangulate) {
    _mesh = triangulate(mesh);
  }else{
    _mesh = mesh;
  }
  std::vector<osd_Point3> positions;
  std::vector<osd_Point3> uvs;
  std::vector<int> vertsperface;
  for(auto& pt : _mesh.GetPoints()) {
    osd_Point3 pos;
    osd_Point3 uv;
    pos.SetPoint(pt.position.x, pt.position.y, pt.position.z);
    uv.SetPoint(pt.t_coords.x, pt.t_coords.y, 0.0f);
    positions.push_back(pos);
    uvs.push_back(uv);
  }
  for(auto& face : _mesh.GetFaces()) {
    vertsperface.push_back((int)face.GetVerticesIndex().size());
  }
  std::vector<int> vertIndices;
  for(auto& face : _mesh.GetFaces()) {
    for(auto idx : face.GetVerticesIndex()) {
      vertIndices.push_back(idx);
    }
  }
  return osd_DATA{positions, uvs, (int)_mesh.GetPoints().size(), (int)_mesh.GetFaces().size(), vertsperface, vertIndices};
}



Mesh subdivide(Mesh &mesh, int maxlevel, SubdivSchema schema) { 
  if( maxlevel <= 0) {
    return mesh;
  }
  using namespace OpenSubdiv;
  bool do_triangulate = false;
  if(schema == SubdivSchema::Loop) {
    do_triangulate = true;
  }
  auto osd_data = mesh_to_osd_data(mesh, do_triangulate);

  Far::TopologyRefiner * refiner = createTopologyRefiner(maxlevel, osd_data, schema);
  refiner->RefineUniform(Far::TopologyRefiner::UniformOptions(maxlevel));

  int nCoarseVerts = (int)osd_data.positions.size();
  int nFineVerts   = refiner->GetLevel(maxlevel).GetNumVertices();
  int nTotalVerts  = refiner->GetNumFaceVerticesTotal();
  int nTempVerts   = nTotalVerts - nCoarseVerts - nFineVerts;
    // Allocate and initialize the primvar data for the original coarse vertices:
    std::vector<osd_Point3> coarsePosBuffer(nCoarseVerts);
    std::vector<osd_Point3> coarseClrBuffer(nCoarseVerts);

 for (int i = 0; i < nCoarseVerts; ++i) {
        coarsePosBuffer[i] = osd_data.positions[i];
        coarseClrBuffer[i] = osd_data.uvs[i];
    }

    // Allocate intermediate and final storage to be populated:
    std::vector<osd_Point3> tempPosBuffer(nTempVerts);
    std::vector<osd_Point3> finePosBuffer(nFineVerts);

    std::vector<osd_Point3> tempClrBuffer(nTempVerts);
    std::vector<osd_Point3> fineClrBuffer(nFineVerts);

    // Interpolate all primvar data -- separate buffers can be populated on
    // separate threads if desired:
    osd_Point3 * srcPos = &coarsePosBuffer[0];
    osd_Point3 * dstPos = &tempPosBuffer[0];

    osd_Point3 * srcClr = &coarseClrBuffer[0];
    osd_Point3 * dstClr = &tempClrBuffer[0];

    Far::PrimvarRefiner primvarRefiner(*refiner);

    for (int level = 1; level < maxlevel; ++level) {
        primvarRefiner.Interpolate(       level, srcPos, dstPos);
        primvarRefiner.InterpolateVarying(level, srcClr, dstClr);

        srcPos = dstPos, dstPos += refiner->GetLevel(level).GetNumVertices();
        srcClr = dstClr, dstClr += refiner->GetLevel(level).GetNumVertices();
    }

    // Interpolate the last level into the separate buffers for our final data:
    primvarRefiner.Interpolate(       maxlevel, srcPos, finePosBuffer);
    primvarRefiner.InterpolateVarying(maxlevel, srcClr, fineClrBuffer);


  //  // Assuming you have a pointer or reference to the refiner:
  Far::TopologyLevel const &refinedLevel = refiner->GetLevel(maxlevel);

  int numFaces = refinedLevel.GetNumFaces();

  Mesh newMesh;
  newMesh.GetPoints().resize(nFineVerts);
  newMesh.GetVertices().resize(nFineVerts);
  newMesh.GetFaces().resize(numFaces);

  for (int i=0; i<nFineVerts; ++i) {
    newMesh.GetPoints()[i].position = glm::vec3(finePosBuffer[i].GetPoint()[0], finePosBuffer[i].GetPoint()[1], finePosBuffer[i].GetPoint()[2]);
    newMesh.GetPoints()[i].t_coords = glm::vec3(fineClrBuffer[i].GetPoint()[0], fineClrBuffer[i].GetPoint()[1], fineClrBuffer[i].GetPoint()[2]);
    newMesh.GetVertices()[i].point_id = i;
  }

  for(int i = 0; i < numFaces; ++i) {
    auto indices = refinedLevel.GetFaceVertices(i);
    std::vector<uint32_t> faceIndices;
    for(int j = 0; j < indices.size(); ++j) {
      faceIndices.push_back((uint32_t)indices[j]);
    }
    newMesh.GetFaces()[i].SetVerticesIndex(faceIndices);
  }

  delete refiner;

  return newMesh; 
}

void translate(Mesh &mesh, glm::vec3 translation) {
  for (auto &pt : mesh.GetPoints()) {
    pt.position += translation;
  }
}

void rotate(Mesh &mesh, glm::vec3 radians, AXIS_ORDER order) {

  for (auto &pt : mesh.GetPoints()) {
    switch (order) {
    case AXIS_ORDER::XYZ:
      pt.position = glm::rotateX(pt.position, radians.x);
      pt.position = glm::rotateY(pt.position, radians.y);
      pt.position = glm::rotateZ(pt.position, radians.z);
      break;
    case AXIS_ORDER::XZY:
      pt.position = glm::rotateX(pt.position, radians.x);
      pt.position = glm::rotateZ(pt.position, radians.z);
      pt.position = glm::rotateY(pt.position, radians.y);
      break;
    case AXIS_ORDER::YXZ:
      pt.position = glm::rotateY(pt.position, radians.y);
      pt.position = glm::rotateX(pt.position, radians.x);
      pt.position = glm::rotateZ(pt.position, radians.z);
      break;
    case AXIS_ORDER::YZX:
      pt.position = glm::rotateY(pt.position, radians.y);
      pt.position = glm::rotateZ(pt.position, radians.z);
      pt.position = glm::rotateX(pt.position, radians.x);
      break;
    case AXIS_ORDER::ZXY:
      pt.position = glm::rotateZ(pt.position, radians.z);
      pt.position = glm::rotateX(pt.position, radians.x);
      pt.position = glm::rotateY(pt.position, radians.y);
      break;
    case AXIS_ORDER::ZYX:
      pt.position = glm::rotateZ(pt.position, radians.z);
      pt.position = glm::rotateY(pt.position, radians.y);
      pt.position = glm::rotateX(pt.position, radians.x);
      break;
    }
  }
}

void scale(Mesh &mesh, glm::vec3 scale) {
  for (auto &pt : mesh.GetPoints()) {
    pt.position.x *= scale.x;
    pt.position.y *= scale.y;
    pt.position.z *= scale.z;
  }
}

void transform(Mesh &mesh, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale_, TRANSFORM_ORDER tr_order, AXIS_ORDER axis_order)
{
      if (tr_order == TRANSFORM_ORDER::TRS) {

        translate(mesh, pos);
        rotate(mesh, rot, axis_order);
        scale(mesh, scale_);
      } else if (tr_order == TSR) {

        translate(mesh, pos);
        scale(mesh, scale_);
        rotate(mesh, rot, axis_order);
      } else if (tr_order == RTS) {

        rotate(mesh, rot, axis_order);
        translate(mesh, pos);
        scale(mesh, scale_);
      } else if (tr_order == RST) {

        rotate(mesh, rot, axis_order);
        scale(mesh, scale_);
        translate(mesh, pos);
      } else if (tr_order == STR) {

        scale(mesh, scale_);
        translate(mesh, pos);
        rotate(mesh, rot, axis_order);
      } else if (tr_order == SRT) {

        scale(mesh, scale_);
        rotate(mesh, rot, axis_order);
        translate(mesh, pos);
      }  
}

void noise_displace(Mesh &mesh, NoiseParams params) {

  FastNoise::SmartNode<FastNoise::Simplex> fnSimplex = FastNoise::New<FastNoise::Simplex>();
  FastNoise::SmartNode<FastNoise::Perlin> fnPerlin = FastNoise::New<FastNoise::Perlin>();
  FastNoise::SmartNode<FastNoise::CellularDistance> fnCellular = FastNoise::New<FastNoise::CellularDistance>();
  auto fnFractal = FastNoise::New<FastNoise::FractalFBm>();

  switch(params.noise_type) {
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

  for (auto &pt : mesh.GetPoints()) {

    float noise_val = fnFractal->GenSingle3D(
        (pt.position.x + params.offset.x) * params.frequency,
        (pt.position.y + params.offset.y) * params.frequency,
        (pt.position.z + params.offset.z) * params.frequency, params.seed);
    pt.position += pt.normal * (noise_val * params.amplitude);
  }
}
void twist(Mesh &mesh, float turns) {
  msh::BoundingBox bb = mesh.ComputeAABB();

  float height = bb.size.z;
  for (auto &pt : mesh.GetPoints()) {
    float pt_height = (pt.position.z - bb.position.z) / height;
    pt.position =
        glm::rotateZ(pt.position, glm::two_pi<float>() * turns * pt_height);
  }
}
void fuse_points(Mesh &mesh, float tolerance) {
  std::cout << "fusePoints" << std::endl;
  std::cout << "num points before : " << mesh.GetPoints().size() << ""
            << std::endl;

  std::unordered_map<glm::vec3, std::vector<uint32_t>, Vec3Hash, Vec3Equal>
      point_map(mesh.GetPoints().size(), Vec3Hash{tolerance},
                Vec3Equal{tolerance});

  // point_map[glm::vec3(0.0f, 0.0f, 0.0f)] = 0;
  // point_map[glm::vec3(0.00000001f, 0.0f, 0.0f)] = 1;
  for (size_t i = 0; i < mesh.GetPoints().size(); i++) {
    glm::vec3 &pos = mesh.GetPoints()[i].position;

    // std::cout << "point to fuse : " << i << std::endl;

    point_map[pos].push_back((uint32_t)i);
  }

  std::cout << "num points after : " << point_map.size() << std::endl;

  std::unordered_map<uint32_t, uint32_t> pt_fuse_target;
  for (auto &entry : point_map) {
    if (entry.second.size() > 1) {

      std::cout << "points to fuse to " << entry.second[0] << " : ";

      for (size_t i = 1; i < entry.second.size(); i++) {
        std::cout << entry.second[i] << " ";
        pt_fuse_target.insert({entry.second[i], entry.second[0]});
      }

      std::cout << std::endl;
    }
  }

  // update faces vertex indices
  for(auto& face : mesh.GetFaces()) {
    for(auto& idx : face.GetVerticesIndex()) {
      if(pt_fuse_target.find(idx) != pt_fuse_target.end()) {
        idx = pt_fuse_target[idx];
      }
    }
  }

  //delete fused points
  // collect indices to delete
  std::vector<uint32_t> indices_to_delete;
  for (auto &entry : pt_fuse_target) {
    indices_to_delete.push_back(entry.first);
    // check faces vertex indices and offset them by -1 if necessary
    for(auto& face : mesh.GetFaces()) {
      for(auto& idx : face.GetVerticesIndex()) {
        if(idx >= entry.first) {
          idx--;
        }
      }

    }
  }

  // print faces data 
  for(auto& face : mesh.GetFaces()){
    LOG_INFO("{}", face);
  }

  std::vector<Point> temp_points = mesh.GetPoints();
  std::unordered_set<uint32_t> indices_to_remove(indices_to_delete.begin(), indices_to_delete.end());

  std::vector<Point> new_points;
  new_points.reserve(temp_points.size());

  for (size_t i = 0; i < temp_points.size(); ++i) {
      if (indices_to_remove.find(i) == indices_to_remove.end()) {
          new_points.push_back(temp_points[i]);
      }
  }

  std::cout << "Num points after: " << new_points.size() << std::endl;

  mesh.SetPoints(new_points);
  // mesh.SetFaces({});


}
} /* end namespace msh::meshutils*/
