#include "MeshUtils.h"


static float lerp(float a, float b, float t) {
    return a + t * (b - a);
}
namespace GLR 
{
		Mesh GLR::MeshUtils::MakeQuad()
		{
			Mesh mesh = MakePlane(1,1,1,1);

			return mesh;
		}

		Mesh MeshUtils::MakeSimpleBox(float w, float h, float l)
		{
			Mesh mesh;

			std::vector<Point> temp_pts;

			temp_pts.reserve(8);

			temp_pts.emplace_back(Point(0.0f, 0.0f, 0.0f));
			temp_pts.emplace_back(Point(0.0f, 1.0f, 0.0f));
			temp_pts.emplace_back(Point(1.0f, 1.0f, 0.0f));
			temp_pts.emplace_back(Point(1.0f, 0.0f, 0.0f));

			temp_pts.emplace_back(Point(0.0f, 0.0f, 1.0f));
			temp_pts.emplace_back(Point(0.0f, 1.0f, 1.0f));
			temp_pts.emplace_back(Point(1.0f, 1.0f, 1.0f));
			temp_pts.emplace_back(Point(1.0f, 0.0f, 1.0f));

	
			// point uvs
			temp_pts[0].t_coords = glm::vec2(0.0f, 0.0f);
			temp_pts[1].t_coords = glm::vec2(0.0f, 1.0f);
			temp_pts[2].t_coords = glm::vec2(1.0f, 1.0f);
			temp_pts[3].t_coords = glm::vec2(1.0f, 0.0f);
			
			
			temp_pts[4].t_coords = glm::vec2(1.0f, 0.0f);
			temp_pts[5].t_coords = glm::vec2(1.0f, 0.0f);
			temp_pts[6].t_coords = glm::vec2(1.0f, 0.0f);
			temp_pts[7].t_coords = glm::vec2(1.0f, 0.0f);


			std::vector<Face> faces;

			faces.insert(faces.end(), {
				Face({ Vertex(0), Vertex(1), Vertex(2), Vertex(3) }),
				Face({ Vertex(4), Vertex(5), Vertex(6), Vertex(7) }),

				Face({ Vertex(0), Vertex(1), Vertex(5), Vertex(4) }),
				Face({ Vertex(1), Vertex(2), Vertex(6), Vertex(5) }),
				Face({ Vertex(2), Vertex(3), Vertex(7), Vertex(6) }),
				Face({ Vertex(3), Vertex(0), Vertex(4), Vertex(7) })
				});

			// uvs 
			mesh.SetPoints(temp_pts);
			mesh.SetFaces(faces);

			Scale(mesh, glm::vec3(w, h, l));
			return mesh;
		}

        Mesh MeshUtils::MakeSimpleBox(float size)
        {
			Mesh mesh = MakeSimpleBox(size, size, size);
            return mesh;
        }

        Mesh MeshUtils::MakeCircle(float radius, uint32_t segments)
        {
            std::vector<Point> points;
            std::vector<Face> faces;

			/* add center point */
			Point center_pt(glm::vec3(0.f));
			center_pt.t_coords = glm::vec2(0.5f, 0.5f);

			points.push_back(center_pt);

			/* add circumference points */
			for(uint32_t i = 0; i < segments; i++)
			{
				float ratio = (float)i / ((float)(segments));
				float x = sinf(ratio * glm::pi<float>() * 2.0f) * radius;
				float z = cosf(ratio * glm::pi<float>() * 2.0f) * radius;
				Point pt(glm::vec3(x, 0.f, z));

				pt.t_coords = glm::vec2(pt.position / radius / 2.0f + 0.5f);
				points.push_back(pt);
			}

			/* Make circle Triangles */

			for(uint32_t i = 1; i < segments+1; i++)
			{
				uint32_t id_0 = 0;
				uint32_t id_1 = i;
				uint32_t id_2 = (i + 1);
				if(i >= segments)
				{
					id_2 = 1;
				}
				Face face({Vertex(id_0), Vertex(id_1), Vertex(id_2)});
				faces.push_back(face);
			}

			Mesh mesh;
			mesh.SetPoints(points);
			mesh.SetFaces(faces);
			return mesh;
        }

        Mesh MeshUtils::MakeSimpleTube(float radius1, float radius2, float length, uint32_t segments, bool triangulate)
        {
			Mesh mesh = MakePlane(length, 1.0f, 1, segments, triangulate);
			Translate(mesh, glm::vec3(length / 2.0f, 0.5f, 0.0f));
			for(auto& pt : mesh.GetPoints())
			{
				auto ratio_x = pt.position.x / length;
				auto ratio_y = pt.position.y;
				auto x = sinf( glm::pi<float>() * 2.0f * (1.0f - ratio_y)) * lerp(radius1, radius2, ratio_x);
				auto z = cosf( glm::pi<float>() * 2.0f * ratio_y) * lerp(radius1, radius2, ratio_x);
				
				pt.position.x = x;
				pt.position.y = ratio_x * length;
				pt.position.z = z;
			}

            return mesh;
        }

        Mesh MeshUtils::MakeCylinder(float radius1, float radius2, float length, uint32_t segments, bool triangulate)
        {
			Mesh mesh = MakeSimpleTube(radius1, radius2, length, segments, false);

			Mesh cap = MakeCircle(radius1, segments);
			Reverse(cap);
			ComputeNormals(cap);
			Merge(mesh, cap);

			Mesh cap2 = Clone(cap);

			if( abs(radius2) > 0.0f && abs(radius1) > 0.0f)
			{
				Scale(cap2, glm::vec3(radius2 / radius1));
			
			}else{
			
				Scale(cap2, glm::vec3(0.0f));
			}
			
			Translate(cap2, glm::vec3( 0.f, length, 0.0f ));
			Reverse(cap2);
			ComputeNormals(cap2);
			Merge(mesh, cap2);

			if(triangulate)
			{
				mesh.Triangulate();
			}

            return mesh;
        }

        Mesh MeshUtils::MakeCone(float radius1, float radius2, float length, uint32_t segments, bool triangulate)
        {
			Mesh mesh = MakeSimpleTube(radius1, radius2, length, segments, triangulate);
            return Mesh();
        }

        Mesh MeshUtils::MakePlane(float width, float length, uint32_t cols, uint32_t rows, bool triangulate)
		{

			Mesh mesh;
			std::vector<Point> points;
			float col_offset = width / (cols);
			float row_offset = length / (rows);
			for (uint32_t row = 0; row < rows+1; row++)
			{
				for (uint32_t col = 0; col < cols+1; col++)
				{	
					Point pt;

					pt.position.x = (col_offset * col) - (width  / 2.0f);
					pt.position.y = (row_offset * row) - (length / 2.0f);
					pt.position.z = 0.f;

					pt.normal = glm::vec3(0.f, 0.f, 1.f);

					float uv_x =        (float)col/((float)cols);
					float uv_y = (float)row/((float)rows);
					
					pt.t_coords = glm::vec2(uv_x, uv_y);

					points.push_back(pt);
				}
			}

			mesh.SetPoints(points);

			/* Faces */
			std::vector<Face> faces;
			for (uint32_t row = 0; row < rows; row++)
			{
				for (uint32_t col = 0; col < cols; col++)
				{	
					uint32_t t1_0 = col + (row * (cols+1)); 
					uint32_t t1_1 = col + (row * (cols+1)) + 1; 
					uint32_t t1_2 = col + (row * (cols+1)) + cols + 2; 
					uint32_t t1_3 = col + (row * (cols+1)) + cols + 1; 

					Face face1({Vertex(t1_0), Vertex(t1_1), Vertex(t1_2), Vertex(t1_3)});
					faces.push_back(face1);					
				}
			}
			
			mesh.SetFaces(faces);

			if(triangulate)
			{
				mesh.Triangulate();
			}

			return mesh;
		}

        Mesh MeshUtils::MakeSimpleSphere(float radius, uint32_t cols, uint32_t rows)
        {
			Mesh mesh;
			std::vector<Point> points;
			float col_offset = 1.0f / (cols);
			float row_offset = 1.0f / (rows);
			for (uint32_t row = 0; row < rows+1; row++)
			{
				for (uint32_t col = 0; col < cols+1; col++)
				{	
					Point pt;
					
					/* position between 0.0 and 1.0 in x and y */
					float ratio_x = (col_offset * col);
					float ratio_y = (row_offset * row);

					pt.position.x = ratio_x + 0.5f;
					pt.position.y = ratio_y + 0.5f;
					pt.position.z = 0.f;

					pt.normal = glm::vec3(0.f, 0.f, 1.f);
					

					pt.t_coords.x = ratio_x;
					pt.t_coords.y = ratio_y;

					points.push_back(pt);
				}
			}

			mesh.SetPoints(points);

			/* Faces */
			/* Because this is for a sphere, the polygon rows at the poles, ie first row and last row need to be triangles*/
			std::vector<Face> faces;
			for (uint32_t row = 0; row < rows; row++)
			{
				for (uint32_t col = 0; col < cols; col++)
				{	
					if( row != 0 && row != rows-1 )
					{

						uint32_t t1_0 = col + (row * (cols+1)); 
						uint32_t t1_1 = col + (row * (cols+1)) + 1; 
						uint32_t t1_2 = col + (row * (cols+1)) + cols + 2; 
						uint32_t t1_3 = col + (row * (cols+1)) + cols + 1; 

						Face face({Vertex(t1_0), Vertex(t1_1), Vertex(t1_2), Vertex(t1_3)});
						faces.push_back(face);

					}else if(row == 0){

						uint32_t t1_0 = col + (row * (cols+1)); 
						uint32_t t1_1 = col + (row * (cols+1)) + cols + 2; 
						uint32_t t1_2 = col + (row * (cols+1)) + cols + 1; 
						

						Face face({Vertex(t1_0), Vertex(t1_1), Vertex(t1_2)});
						faces.push_back(face);

					} else if(row == rows-1){

						uint32_t t1_0 = col + (row * (cols+1)); 
						uint32_t t1_1 = col + (row * (cols+1)) + 1; 
						uint32_t t1_2 = col + (row * (cols+1)) + cols + 1; 
						

						Face face({Vertex(t1_0), Vertex(t1_1), Vertex(t1_2)});
						faces.push_back(face);
					}
				}
			}
			
			mesh.SetFaces(faces);

			MeshUtils::Translate(mesh, glm::vec3(0.5f, 0.5f, 0.0f));
			for(auto& pt : mesh.GetPoints())
			{	
				float u = pt.position.x * glm::pi<float>() * 2.0f;
				float v = pt.position.y * glm::pi<float>() * 1.0f;

				pt.position.x = radius * sinf(v)*cosf(u);
				pt.position.y = radius * sinf(v)*sinf(u);
				pt.position.z = radius * cosf(v);				
				
			}

			Rotate(mesh, glm::vec3( glm::pi<float>()/2.0f, 0.0f, 0.0f));
			/* 
				basic ComputeNormals fails here
				set the normals from position as a fix
			 */
			for(auto& pt : mesh.GetPoints())
			{
				pt.normal = glm::normalize(pt.position);
			}

			mesh.Triangulate();
            return mesh;
        }

        Mesh MeshUtils::MakeBox(float w, float h, float l, uint32_t cols, uint32_t rows, bool triangulate)
        {
			Mesh plane = MakePlane(1.0f, 1.0f, cols, rows, false);
			Rotate(plane, glm::vec3(glm::pi<float>(), 0.f, 0.f));
			Translate(plane, glm::vec3(0.f, 0.f, -0.5f));

			auto _clone = Clone(plane);
			Rotate(_clone, glm::vec3(glm::pi<float>() / 2.0f, 0.f, 0.f));
			 Merge(plane, _clone);

			_clone = Clone(_clone);
			Rotate(_clone, glm::vec3(glm::pi<float>()/ 2.0f, 0.f, 0.f));
			Merge(plane, _clone);			
			
			_clone = Clone(_clone);
			Rotate(_clone, glm::vec3(glm::pi<float>()/ 2.0f, 0.f, 0.f));
			Merge(plane, _clone);			

			_clone = Clone(_clone);
			Rotate(_clone, glm::vec3(0.f, 0.f, glm::pi<float>()/ 2.0f));
			Merge(plane, _clone);			

			_clone = Clone(_clone);
			Rotate(_clone, glm::vec3(0.f, glm::pi<float>(), 0.f ));
			Merge(plane, _clone);			

			Scale(plane, glm::vec3(w, h, l));

			if(triangulate)
			{
				plane.Triangulate();
			} 
			
			return plane;
        }

        Mesh MeshUtils::MakeQuadSphere(float radius, uint32_t cols, uint32_t rows, bool triangulate)
        {
			Mesh box = MakeBox(1.0f, 1.0f, 1.0f, cols, rows, false);            
			Spherify(box);
			Scale(box, glm::vec3(radius));

			
			/* normals are just the positions ... */
			for(auto& pt : box.GetPoints())
			{
				pt.normal = glm::normalize(pt.position);
			}

			if(triangulate)
			{
				box.Triangulate();
			} 
			
			return box;
        }

        Mesh MeshUtils::UniquePoints(Mesh& input_mesh)
		{
			Mesh mesh;
			auto& faces = input_mesh.GetFaces();
			std::vector<Point> temp_pts;
			for (uint32_t i = 0; i < faces.size(); i++)
			{
				std::vector<Vertex> new_vertices;

				for (uint32_t vert_id = 0; vert_id < faces[i].GetNumVertices(); vert_id++)
				{

					temp_pts.push_back(Point(input_mesh.GetPoints()[faces[i].GetVertex(vert_id).point_id].position));
					temp_pts[vert_id].t_coords = input_mesh.GetPoints()[faces[i].GetVertex(vert_id).point_id].t_coords;
					Vertex vert;
					vert.point_id = (uint32_t)temp_pts.size() - 1;
					vert.t_coords = input_mesh.GetPoints()[faces[i].GetVertex(vert_id).point_id].t_coords;
					new_vertices.push_back(vert);
				}


				faces[i].SetVertices(new_vertices);
			}

			mesh.SetPoints(temp_pts);
			mesh.SetFaces(faces);

			return mesh;
		}

        Mesh MeshUtils::Spherify(Mesh &input_mesh)
        {
			for(auto& pt : input_mesh.GetPoints()){
				pt.position = glm::normalize(pt.position);
			}
            return Mesh();
        }

        Mesh MeshUtils::Clone(Mesh mesh)
        {
			Mesh result;
			result = mesh;
            return result;
        }

        Mesh MeshUtils::Merge(Mesh &meshA, Mesh &meshB)
        {
			// merge points
			
			std::vector<Point> merged_pts;
			auto pointsA = meshA.GetPoints(); 
			uint32_t id_offset = (uint32_t)pointsA.size();
			auto pointsB = meshB.GetPoints(); 

			pointsA.insert(pointsA.end(), pointsB.begin(), pointsB.end());

			meshA.SetPoints(pointsA);


			// merge faces
			// first offset vertex point id by the number of points already in meshA. see id_offset variable
			auto facesA = meshA.GetFaces();
			auto facesB = meshB.GetFaces();
			for(auto& face : facesB){
				auto& verts = face.GetVertices();
				for(auto& vert : verts){

					vert.SetPointID(vert.point_id + id_offset);
				}
			}

			// actual merging of faces
			facesA.insert(facesA.end(), facesB.begin(), facesB.end());

			meshA.SetFaces(facesA);

            return meshA;
        }


		/* NOT IMPLEMENTED YET !!! */
        Mesh MeshUtils::PlanarUVS(Mesh &input_mesh, glm::vec3 dir)
        {
			/* not implemented yet ! */
			return input_mesh;
		}


		static bool face_has_pt_id(Face& face, uint32_t id)
		{
			for(size_t vertex_id=0; vertex_id < face.GetNumVertices(); vertex_id++)
			{
				auto& vertex = face.GetVertices()[vertex_id];
				if(vertex.point_id == id)
				{
					return true;
				} 
			}
			return false;
		}

        void MeshUtils::Reverse(Mesh &mesh)
        {
			for(auto& face : mesh.GetFaces())
			{
				std::reverse(face.GetVertices().begin(), face.GetVertices().end());
			}
        }

        void MeshUtils::ComputeNormals(Mesh &mesh, bool fuse_normals, float smooth_angle_limit)
        {
			auto& faces = mesh.GetFaces();
			auto& points = mesh.GetPoints();
			/* 
				first get "geometric" normals on vertices 
			*/
			for (uint32_t face_id = 0; face_id < faces.size(); face_id++)
			{
				std::vector<Vertex> vertices;
				for(uint32_t i = 0; i < faces[face_id].GetNumVertices(); i++)
				{
					uint32_t num_verts = faces[face_id].GetNumVertices();
					auto id_A = (i + 0) % num_verts;
					auto id_B = (i + 1) % num_verts;
					auto id_C = (i + 2) % num_verts;
					Point pA = points[faces[face_id].GetVertex(id_A).point_id];
					Point pB = points[faces[face_id].GetVertex(id_B).point_id];
					Point pC = points[faces[face_id].GetVertex(id_C).point_id];

					glm::vec3 AB = pB.position - pA.position;
					glm::vec3 AC = pC.position - pA.position;

					glm::vec3 cross = glm::normalize(glm::cross(glm::normalize(AB), glm::normalize(AC)));
					
					Vertex vert(faces[face_id].GetVertex(uint32_t(i)).point_id);
					vert.normal = cross;
					vertices.push_back(vert);
				
				}
				
				faces[face_id].SetVertices(vertices);
			}


			/* create an array of vec3 for normals */ 
			std::vector<glm::vec3> point_normals(points.size());
			std::vector<uint32_t> num_normals(points.size());
			
			/* init values to zeros */
			for (size_t i = 0; i < point_normals.size(); i++)
			{
				point_normals[i] = glm::vec3(0.0f, 0.0f, 0.0f);
				num_normals[i] = 0;
			}

			for (size_t face_id = 0; face_id < faces.size(); face_id++)
			{
				for (size_t vert_id = 0; vert_id < faces[face_id].GetNumVertices(); vert_id++)
				{
					uint32_t point_id = faces[face_id].GetVertex(uint32_t(vert_id)).point_id;

					num_normals[point_id]++;
					point_normals[point_id] += faces[face_id].GetVertex(uint32_t(vert_id)).normal;
				}
			}

			for (size_t i = 0; i < point_normals.size(); i++)
			{
				/* Finally take the average */
				points[i].normal = glm::normalize(point_normals[i] / (float)num_normals[i]);
			}

			if( fuse_normals)
			{
				FuseNormals(mesh, smooth_angle_limit);
			}


        }

        void MeshUtils::FuseNormals(Mesh &mesh, float smooth_angle_limit)
        {
			/* fuse normals */
			for(size_t i = 0; i < mesh.GetPoints().size(); i++)
			{
				auto& pt = mesh.GetPoints()[i];
				glm::vec3 temp_normal = pt.normal;
				int inc = 1; // first entry is this point itself
				std::vector<uint32_t> common_pts;
				common_pts.push_back((uint32_t)i);
				for(size_t j =0; j < mesh.GetPoints().size(); j++)
				{
					/* check for every other point position */
					if( i != j)
					{
						auto& other_pt = mesh.GetPoints()[j];
						/* using distance squared to avoid sqrt*/
						auto dist_squared = glm::distance2(pt.position, other_pt.position);
			
						if( dist_squared < 0.0001f)
						{
							/* check for angle before adding to list */
							float angle = glm::degrees(glm::angle(pt.normal, other_pt.normal));
							
							if( angle < smooth_angle_limit)
							{
								common_pts.push_back((uint32_t)j);
								temp_normal += other_pt.normal;
								inc++;
							}
						}
					}
				}

				temp_normal /= inc;

				for(auto id : common_pts)
				{
					mesh.GetPoints()[id].normal = glm::normalize(temp_normal);
				}
				// pt.normal = glm::normalize(temp_normal);
			}			
        }

        void MeshUtils::Translate(Mesh& _mesh, glm::vec3 _vec)
		{
			auto& pts = _mesh.GetPoints();
			for (size_t i = 0; i < pts.size(); i++)
			{
				pts[i].position = pts[i].position + _vec;
			}
		}

		void MeshUtils::Scale(Mesh& _mesh, glm::vec3 _scale)
		{
			auto& pts = _mesh.GetPoints();
			for (size_t i = 0; i < pts.size(); i++)
			{
				pts[i].position = pts[i].position * _scale;
			}
		}

		void MeshUtils::Rotate(Mesh& _mesh, glm::vec3 _rot)
		{
			auto& pts = _mesh.GetPoints();
			for (size_t i = 0; i < pts.size(); i++)
			{
				pts[i].position = glm::rotateX(pts[i].position, _rot.x);
				pts[i].position = glm::rotateY(pts[i].position, _rot.y);
				pts[i].position = glm::rotateZ(pts[i].position, _rot.z);
			}
		}

        void MeshUtils::UVTranslate(Mesh &_mesh, glm::vec2 _vec)
        {
			auto& pts = _mesh.GetPoints();
			for (size_t i = 0; i < pts.size(); i++)
			{
				pts[i].t_coords = pts[i].t_coords + _vec;
			}			
        }
        void MeshUtils::UVScale(Mesh &_mesh, glm::vec2 _scale)
        {
			auto& pts = _mesh.GetPoints();
			for (size_t i = 0; i < pts.size(); i++)
			{
				pts[i].t_coords = pts[i].t_coords * _scale;
			}			
        }
        void MeshUtils::UVScale(std::shared_ptr<Mesh> _mesh, glm::vec2 _scale)
        {
			auto& pts = _mesh->GetPoints();
			for (size_t i = 0; i < pts.size(); i++)
			{
				pts[i].t_coords = pts[i].t_coords * _scale;
			}				
        }
}