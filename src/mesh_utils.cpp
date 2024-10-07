#include "mesh_utils.h"

namespace msh::meshutils
{   

    Mesh generateGrid(float width, float length, uint32_t _cols, uint32_t _rows){

        Mesh result;
        uint32_t cols = _cols+1;
        uint32_t rows = _rows+1; 
        std::vector<Point> points;
        std::vector<Vertex> vertices;
        for(uint32_t i = 0; i < rows; i++){
            for(uint32_t j = 0; j < cols; j++){
                Point p;
                Vertex vtx;
                vtx.point_id = i*cols+j;
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
        for(uint32_t i = 0; i < rows-1; i++){
            for(uint32_t j = 0; j < cols-1; j++){
                uint32_t id0 = i*cols+j;
                uint32_t id1 = i*cols+j+1;
                uint32_t id2 = (i+1)*cols+j+1;
                uint32_t id3 = (i+1)*cols+j;
                faces.push_back(Face({id0, id1, id2, id3}));
            }
        }
        result.SetFaces(faces);

        return result;
    }

    Mesh generateTube(float radius, float height, uint32_t cols, uint32_t rows)
    {
        Mesh result;
        result = generateGrid(1.0f, 1.0f, cols, rows);

        for(auto& pt : result.GetPoints()){
            glm::vec3 new_pos;

            new_pos.x = cosf(pt.position.x * glm::pi<float>() * 2.0f) * radius;
            new_pos.z = pt.position.y * height;
            new_pos.y = sinf(pt.position.x * glm::pi<float>() * 2.0f) * radius;
            
            pt.position = new_pos;
        }
        return result;
    }

    Mesh generateTube2(float radius, float height, uint32_t _cols, uint32_t _rows)
    {
      Mesh result;
        uint32_t cols = _cols;
        uint32_t rows = _rows+1; 
        std::vector<Point> points;
        std::vector<Vertex> vertices;
        for(uint32_t i = 0; i < rows; i++){
            for(uint32_t j = 0; j < cols; j++){
                Point p;
                Vertex vtx;
                vtx.point_id = i*cols+j;
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
        for(uint32_t i = 0; i < rows-1; i++){
            for(uint32_t j = 0; j < cols; j++){
                if( j < cols-1){
                    uint32_t id0 = i*cols+j;
                    uint32_t id1 = i*cols+j+1;
                    uint32_t id2 = (i+1)*cols+j+1;
                    uint32_t id3 = (i+1)*cols+j;
                    // LOG_INFO("OK indices {} -- {} -- {} -- {}",id0,id1,id2,id3);
                    faces.push_back(Face({id0, id1, id2, id3}));
                }else{
                    uint32_t id0 = i*cols+j;
                    uint32_t id1 = i * cols;
                    uint32_t id2 = (i+1)*cols;
                    uint32_t id3 = (i+1)*cols+j;
                    // LOG_ERROR("wtf indices {} -- {} -- {} -- {}",id0,id1,id2,id3);

                    faces.push_back(Face({id0,id1,id2,id3}));
                }
            }
        }
        result.SetFaces(faces);        
        return result;
    }

    Mesh generateDisc(float radius, uint32_t segs)
    {
        Mesh result;
        std::vector<Point> pts; 
        std::vector<Vertex> vertices; 
        for(uint32_t i = 0; i < segs; i++){
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
        for(uint32_t i = 0; i < segs    ; i++){
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
    Mesh merge(Mesh& mesh1, Mesh& mesh2)
    {
        Mesh merged;
        std::vector<Point> pts(mesh1.GetPoints().size());
        std::copy(mesh1.GetPoints().begin(),mesh1.GetPoints().end(), pts.begin());
        
        std::vector<Vertex> vertices(mesh1.GetVertices().size());
        std::copy(mesh1.GetVertices().begin(),mesh1.GetVertices().end(), vertices.begin());

        pts.insert(pts.end(), mesh2.GetPoints().begin(), mesh2.GetPoints().end());

        std::vector<Face> faces(mesh1.GetFaces().size());
        std::vector<Face> mesh2_faces(mesh2.GetFaces().size());
        std::copy(mesh1.GetFaces().begin(),mesh1.GetFaces().end(), faces.begin());
        std::copy(mesh2.GetFaces().begin(),mesh2.GetFaces().end(), mesh2_faces.begin());
        
        for(auto& face: mesh2_faces){
            for(auto& idx : face.GetVerticesIndex()){
                idx += (uint32_t)mesh1.GetPoints().size();
            }
        }
        faces.insert(faces.end(), mesh2_faces.begin(), mesh2_faces.end());


        std::vector<Vertex> mesh2_vertices2(mesh2.GetVertices().size());
        std::copy(mesh2.GetVertices().begin(),mesh2.GetVertices().end(), mesh2_vertices2.begin());
        for(auto& vert : mesh2_vertices2){
            vert.point_id += (uint32_t)mesh1.GetPoints().size();
        }
        vertices.insert(vertices.end(), mesh2_vertices2.begin(), mesh2_vertices2.end());

        merged.SetPoints(pts);
        merged.SetVertices(vertices);
        merged.SetFaces(faces);
        return merged;
    }

    void translate(Mesh &mesh, glm::vec3 translation)
    {
        for(auto& pt : mesh.GetPoints()){
            pt.position += translation;
        }
    }

    void rotate(Mesh &mesh, glm::vec3 radians, AXYS_ORDER order)
    {

        for(auto& pt : mesh.GetPoints()){
            switch(order){
                case AXYS_ORDER::XYZ:
                    pt.position = glm::rotateX(pt.position, radians.x);
                    pt.position = glm::rotateY(pt.position, radians.y);
                    pt.position = glm::rotateZ(pt.position, radians.z);
                    break;
                case AXYS_ORDER::XZY:
                    pt.position = glm::rotateX(pt.position, radians.x);
                    pt.position = glm::rotateZ(pt.position, radians.z);
                    pt.position = glm::rotateY(pt.position, radians.y);
                    break;
                case AXYS_ORDER::YXZ:
                    pt.position = glm::rotateY(pt.position, radians.y);
                    pt.position = glm::rotateX(pt.position, radians.x);
                    pt.position = glm::rotateZ(pt.position, radians.z);
                    break;
                case AXYS_ORDER::YZX:
                    pt.position = glm::rotateY(pt.position, radians.y);
                    pt.position = glm::rotateZ(pt.position, radians.z);
                    pt.position = glm::rotateX(pt.position, radians.x);
                    break;
                case AXYS_ORDER::ZXY:
                    pt.position = glm::rotateZ(pt.position, radians.z);
                    pt.position = glm::rotateX(pt.position, radians.x);
                    pt.position = glm::rotateY(pt.position, radians.y);
                    break;
                case AXYS_ORDER::ZYX:
                    pt.position = glm::rotateZ(pt.position, radians.z);
                    pt.position = glm::rotateY(pt.position, radians.y);
                    pt.position = glm::rotateX(pt.position, radians.x);
                    break;
            }
        }
    }

    void scale(Mesh &mesh, glm::vec3 scale)
    {
        for(auto& pt : mesh.GetPoints()){
            pt.position.x *= scale.x;
            pt.position.y *= scale.y;
            pt.position.z *= scale.z;
        }
    }

    void NoiseDisplace(Mesh& mesh, NoiseParams params){

        auto fnSimplex = FastNoise::New<FastNoise::Simplex>();
        auto fnFractal = FastNoise::New<FastNoise::FractalFBm>();

        fnFractal->SetSource( fnSimplex );
        fnFractal->SetOctaveCount( (int)params.octaves );
        fnFractal->SetGain( params.gain );
        fnFractal->SetLacunarity( params.lacunarity );
        fnFractal->SetWeightedStrength(params.weightedStrength);


        //auto noise_val = fnFractal->GenSingle2D( 0.1234f, 0.2546f, 1234 );
        for(auto& pt : mesh.GetPoints()){

            float noise_val = fnFractal->GenSingle2D((pt.position.x + params.offset.x) * params.frequency, (pt.position.y + params.offset.y) * params.frequency, params.seed);
            pt.position += glm::vec3(0.0f, 0.0f, noise_val * params.amplitude);
        }
    }
} /* end namespace msh::meshutils*/
