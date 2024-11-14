#include "mesh_utils.h"

namespace msh::meshutils {

Mesh generateGrid(float width, float length, uint32_t _cols, uint32_t _rows) {

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

Mesh generateTube(float radius1, float radius2, float height, uint32_t cols,
                  uint32_t rows) {
  Mesh result;
  result = generateGrid(1.0f, 1.0f, cols, rows);

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

Mesh generateTube2(float radius, float height, uint32_t _cols, uint32_t _rows) {
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

Mesh generateDisc(float radius, uint32_t segs) {
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

static OpenSubdiv::Far::TopologyRefiner const *
createTopologyRefiner(int maxlevel, osd_DATA& osd_data, SubdivSchema schema) {
    using namespace OpenSubdiv;
    // Populate a topology descriptor with our raw data

    typedef Far::TopologyDescriptor Descriptor;

    // Sdc::SchemeType type = OpenSubdiv::Sdc::SCHEME_CATMARK;
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

    // Uniformly refine the topology up to 'maxlevel'
    refiner->RefineUniform(Far::TopologyRefiner::UniformOptions(maxlevel));

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
  std::vector<float> verts;
  std::vector<int> vertsperface;
  for(auto& pt : _mesh.GetPoints()) {
    verts.push_back(pt.position.x);
    verts.push_back(pt.position.y);
    verts.push_back(pt.position.z);
  }
  for(auto& face : _mesh.GetFaces()) {
    vertsperface.push_back((int)face.GetVerticesIndex().size());
  }
  std::vector<int> vertIndices;
  for(auto& face : _mesh.GetFaces()) {
    for(auto& idx : face.GetVerticesIndex()) {
      vertIndices.push_back(idx);
    }
  }
  return osd_DATA{verts, (int)_mesh.GetPoints().size(), (int)_mesh.GetFaces().size(), vertsperface, vertIndices};
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

  int nCoarseVerts=0,
      nRefinedVerts=0;
  int numFaces = 0;
  int numVertices = 0;

  //
  // Setup phase
  //
  Far::StencilTable const * stencilTable = NULL;
  // Setup Far::StencilTable
    Far::TopologyRefiner const * refiner = createTopologyRefiner(maxlevel, osd_data, schema);

    // Setup a factory to create FarStencilTable (for more details see
    // Far tutorials)
    Far::StencilTableFactory::Options options;
    options.generateOffsets=true;
    options.generateIntermediateLevels=false;

    stencilTable = Far::StencilTableFactory::Create(*refiner, options);

    nCoarseVerts = refiner->GetLevel(0).GetNumVertices();
    nRefinedVerts = stencilTable->GetNumStencils();
    
    // Assuming you have a pointer or reference to the refiner:
    Far::TopologyLevel const &refinedLevel = refiner->GetLevel(maxlevel);

    numFaces = refinedLevel.GetNumFaces();
    numVertices = refinedLevel.GetNumFaceVertices();

  // Setup a buffer for vertex primvar data:
  Osd::CpuVertexBuffer * vbuffer =
      Osd::CpuVertexBuffer::Create(3, nCoarseVerts + nRefinedVerts);

  //
  // Execution phase (every frame)
  //
  {
    // Pack the control vertex data at the start of the vertex buffer
    // and update every time control data changes
    vbuffer->UpdateData(osd_data.vertices.data(), 0, nCoarseVerts);


    Osd::BufferDescriptor srcDesc(0, 3, 3);
    Osd::BufferDescriptor dstDesc(nCoarseVerts*3, 3, 3);

    // Launch the computation
    Osd::CpuEvaluator::EvalStencils(vbuffer, srcDesc,
                                    vbuffer, dstDesc,
                                    stencilTable);
  }

  float const * refinedVerts = vbuffer->BindCpuBuffer() + 3*nCoarseVerts;
  Mesh newMesh;
  newMesh.GetPoints().resize(nRefinedVerts);
  newMesh.GetVertices().resize(nRefinedVerts);
  newMesh.GetFaces().resize(numFaces);

  for (int i=0; i<nRefinedVerts; ++i) {
    newMesh.GetPoints()[i].position = glm::vec3(refinedVerts[3*i], refinedVerts[3*i+1], refinedVerts[3*i+2]);
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
  mesh = newMesh;

  delete stencilTable;
  delete vbuffer;
  delete refiner;
  return mesh; 
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

  auto fnSimplex = FastNoise::New<FastNoise::Simplex>();
  auto fnFractal = FastNoise::New<FastNoise::FractalFBm>();

  fnFractal->SetSource(fnSimplex);
  fnFractal->SetOctaveCount((int)params.octaves);
  fnFractal->SetGain(params.gain);
  fnFractal->SetLacunarity(params.lacunarity);
  fnFractal->SetWeightedStrength(params.weightedStrength);

  // auto noise_val = fnFractal->GenSingle2D( 0.1234f, 0.2546f, 1234 );
  for (auto &pt : mesh.GetPoints()) {

    // float noise_val = fnFractal->GenSingle2D((pt.t_coords.x +
    // params.offset.x) * params.frequency, (pt.t_coords.y + params.offset.y) *
    // params.frequency, params.seed);
    float noise_val = fnFractal->GenSingle3D(
        (pt.position.x + params.offset.x) * params.frequency,
        (pt.position.y + params.offset.y) * params.frequency,
        (pt.position.z + params.offset.z) * params.frequency, params.seed);
    // pt.position += glm::vec3(0.0f, 0.0f, noise_val * params.amplitude);
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

  for (auto &entry : point_map) {
    if (entry.second.size() > 1) {

      std::cout << "points to fuse : ";

      for (size_t i = 0; i < entry.second.size(); i++) {
        std::cout << entry.second[i] << " ";
      }

      std::cout << std::endl;
    }
  }
  // for(const auto& entry : point_map){
  //     auto& pt = mesh.GetPoints()[entry.second];
  //     std::cout << pt.position.x << std::endl;

  // }
}
} /* end namespace msh::meshutils*/
