#include "mesh_utils.h"

namespace msh::meshutils
{   

    Mesh generateGrid(float width, float length, uint32_t _cols, uint32_t _rows){

        Mesh result;
        uint32_t cols = _cols+1;
        uint32_t rows = _rows+1; 
        std::vector<Point> points;
        for(uint32_t i = 0; i < rows; i++){
            for(uint32_t j = 0; j < cols; j++){
                Point p;
                float u = j / (float)(cols - 1);
                float v = i / (float)(rows - 1);
                p.position.x = u * width;
                p.position.y = v * length;
                p.position.z = 0.0f;

                p.t_coords.x = u;
                p.t_coords.y = v;
                
                points.push_back(p);
            }
        }
        result.SetPoints(points);

        std::vector<Face> faces;
        for(uint32_t i = 0; i < rows-1; i++){
            for(uint32_t j = 0; j < cols-1; j++){
                faces.push_back(Face({Vertex(i*cols+j),Vertex(i*cols+j+1),Vertex((i+1)*cols+j+1),Vertex((i+1)*cols+j)}));
            }
        }
        result.SetFaces(faces);

        return result;
    }
    Mesh generateTube(float radius, float height, uint32_t cols, uint32_t rows)
    {
        LOG_WARN("TODO : Cylinder has no caps yet");
        Mesh result;
        result = generateGrid(1.0f, 1.0f, cols, rows);

        for(auto& pt : result.GetPoints()){
            glm::vec3 new_pos;

            new_pos.x = cosf(pt.position.x * PI * 2.0f) * radius;
            new_pos.z = pt.position.y * height;
            new_pos.y = sinf(pt.position.x * PI * 2.0f) * radius;
            
            pt.position = new_pos;
        }
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
    Mesh mergeMeshes(Mesh &mesh1, Mesh &mesh2)
    {
        Mesh merged;
        std::vector<Point> pts(mesh1.GetPoints().size());
        std::copy(mesh1.GetPoints().begin(),mesh1.GetPoints().end(), pts.begin());

        pts.insert(pts.end(), mesh2.GetPoints().begin(), mesh2.GetPoints().end());

        std::vector<Face> faces(mesh1.GetFaces().size());
        std::vector<Face> mesh2_faces(mesh2.GetFaces().size());
        std::copy(mesh1.GetFaces().begin(),mesh1.GetFaces().end(), faces.begin());
        std::copy(mesh2.GetFaces().begin(),mesh2.GetFaces().end(), mesh2_faces.begin());
        
        for(auto& face: mesh2_faces){
            for(auto& vert : face.GetVertices()){
                vert.point_id += mesh1.GetPoints().size();
            }
        }
        faces.insert(faces.end(), mesh2_faces.begin(), mesh2_faces.end());

        merged.SetPoints(pts);
        merged.SetFaces(faces);
        return merged;
    }
} /* end namespace msh::meshutils*/
