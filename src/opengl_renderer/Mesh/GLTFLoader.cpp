#include "GLTFLoader.h"
#include <memory>
#include <spdlog/fmt/ostr.h> 
namespace GLR 
{
	GLTFLoader* GLTFLoader::s_Instance = nullptr;

	GLTFLoader* GLTFLoader::GetInstance()
	{
		if( s_Instance == nullptr)
		{
			s_Instance = new GLTFLoader();
		}

		return s_Instance;
	}
	GLTFLoader::GLTFLoader()
	{

	}

	GLTFLoader::~GLTFLoader()
	{

	}
	
	std::vector<Mesh> GLTFLoader::LoadBinary(fs::path path)
	{   
		std::vector<Mesh> meshes;
		std::string err, warn;
		tinygltf::Model model;
		bool ret = loader.LoadBinaryFromFile(&model, &err, &warn, path.string());

		if(!warn.empty())
		{
			LOG_WARN(warn);
		}
		if(!err.empty())
		{
			LOG_ERROR(err);
		}

		// std::cout << path.string() <<" has:\n"
		//             << model.accessors.size() << " accessors\n"
		//             << model.animations.size() << " animations\n"
		//             << model.buffers.size() << " buffers\n"
		//             << model.bufferViews.size() << " bufferViews\n"
		//             << model.materials.size() << " materials\n"
		//             << model.meshes.size() << " meshes\n"
		//             << model.nodes.size() << " nodes\n"
		//             << model.textures.size() << " textures\n"
		//             << model.images.size() << " images\n"
		//             << model.skins.size() << " skins\n"
		//             << model.samplers.size() << " samplers\n"
		//             << model.cameras.size() << " cameras\n"
		//             << model.scenes.size() << " scenes\n"
		//             << model.lights.size() << " lights\n";  



		for(const auto& scene : model.scenes)
		{
			
			for(const auto& node_id : scene.nodes)
			{
				auto& cur_node = model.nodes[node_id];

				while( cur_node.children.size() > 0)
				{
					cur_node = model.nodes[cur_node.children[0]];

				}

			}
		}
		/* check for images*/
		for(const auto& img : model.images)
		{
			// LOG_INFO("file path :{} \nDimensions : {} {}", path.string(), img.width, img.height);
			if( img.uri.empty())
			{
				// LOG_INFO("buffer view : {}", img.bufferView);
			}
		}


		for(const auto& mesh : model.meshes)
		{
			Mesh orb_mesh = ConvertToOrbitalsMesh(model, mesh);
			meshes.push_back(orb_mesh);
		}    

		return meshes;
	}

	struct GLTF_NODE{
		std::string name;
		int id;
		int parent_id;
	};
	std::ostream& operator<<(std::ostream& os,const std::vector<GLTF_NODE>& node_list)
	{
		for(const auto& node : node_list)
		{
			os << "Node : " << node.name <<std::endl;
			os << "  ID         : "<< node.id << std::endl;
			os << "  Parent ID  : "<< node.parent_id << std::endl;
			os << "-- "<< std::endl;
		}

		return os;
	}	

	void traverse_node(int node_id, int parent_id, std::vector<GLTF_NODE>& gltf_nodes, tinygltf::Scene& scene, tinygltf::Model& model)
	{

		auto& node = model.nodes[node_id];
		auto& cur_node = model.nodes[node_id];
		gltf_nodes.push_back({cur_node.name, node_id,  parent_id});
		for(auto& child_id : cur_node.children)
		{

			traverse_node(child_id, node_id, gltf_nodes, scene, model);

		}
	}

	static std::shared_ptr<MeshObject> node_to_mesh_object(tinygltf::Model& model, int node_id)
	{
			auto mesh_object = std::make_shared<MeshObject>();
			auto& cur_node = model.nodes[node_id];

			if( cur_node.translation.size() == 3)
			{
				auto x = (float)cur_node.translation[0];
				auto y = (float)cur_node.translation[1];
				auto z = (float)cur_node.translation[2];
				// LOG_INFO(" Translation : {} {} {}", x, y, z);
				mesh_object->SetPosition(mesh_object->GetPosition() + glm::vec3(x, y, z));
			}

			if( cur_node.rotation.size() == 4)
			{
				/* rotation is a quaternion */

				auto x = (float)cur_node.rotation[0];
				auto y = (float)cur_node.rotation[1];
				auto z = (float)cur_node.rotation[2];
				auto w = (float)cur_node.rotation[3];
				glm::quat quat = glm::quat(w, x, y, z);
				auto euler = glm::eulerAngles(quat);
				// LOG_INFO(" rotation : {} {} {}", euler.x, euler.y, euler.z);
				mesh_object->SetRotation(glm::degrees(glm::vec3(euler.x, euler.y, euler.z)));
			}
			if( cur_node.scale.size() == 3)
			{
				auto x = (float)cur_node.scale[0];
				auto y = (float)cur_node.scale[1];
				auto z = (float)cur_node.scale[2];
				// LOG_INFO(" Scale : {} {} {}", x, y, z);
				mesh_object->SetScale(x, y, z);
			}
		
			auto mesh = std::make_shared<Mesh>(GLTFLoader::ConvertToOrbitalsMesh(model, model.meshes[cur_node.mesh]));

			mesh_object->SetMesh(mesh);
			mesh_object->SetName(cur_node.name);

			return mesh_object;
	}

    std::vector<std::shared_ptr<MeshObject>> GLTFLoader::LoadBinaryMeshObjects(fs::path path)
    {
		std::vector<std::shared_ptr<MeshObject>> mesh_objects;
		std::string err, warn;
		tinygltf::Model model;
		bool ret = loader.LoadBinaryFromFile(&model, &err, &warn, path.string());

		if(!warn.empty())
		{
			LOG_WARN(warn);
		}
		if(!err.empty())
		{
			LOG_ERROR(err);
		}

		// std::cout << path.string() <<" has:\n"
		//             << model.accessors.size() << " accessors\n"
		//             << model.animations.size() << " animations\n"
		//             << model.buffers.size() << " buffers\n"
		//             << model.bufferViews.size() << " bufferViews\n"
		//             << model.materials.size() << " materials\n"
		//             << model.meshes.size() << " meshes\n"
		//             << model.nodes.size() << " nodes\n"
		//             << model.textures.size() << " textures\n"
		//             << model.images.size() << " images\n"
		//             << model.skins.size() << " skins\n"
		//             << model.samplers.size() << " samplers\n"
		//             << model.cameras.size() << " cameras\n"
		//             << model.scenes.size() << " scenes\n"
		//             << model.lights.size() << " lights\n";  

		// LOG_INFO("---- Textures ---------------------------");		
		// for(const auto& texture : model.textures)
		// {
		// 	LOG_INFO("Texture name                   : {}",texture.name);
		// 	LOG_INFO("Texture source                 : {}",texture.source);
		// 	LOG_INFO("Texture extensions json string : {}",texture.extensions_json_string);
		// 	LOG_INFO("Texture extras json string     : {}",texture.extras_json_string);
		// }
		
		// LOG_INFO("---- Images ---------------------------");		
		// for(const auto& image : model.images)
		// {
		// 	LOG_INFO("Image name       :  {}",image.name);
		// 	LOG_INFO("Image extensions :  {}",image.extensions_json_string);
		// }
		// LOG_INFO("---- Materials ---------------------------");		
		// for(const auto& material : model.materials)
		// {
		// 	LOG_INFO("Material name                               :  {}",material.name);
		// 	LOG_INFO("Material Extras                             :  {}",material.extras_json_string);
		// 	LOG_INFO("Material Extensions                         :  {}",material.extensions_json_string);
		// 	LOG_INFO("Material baseColorTexture Index             :  {}", material.pbrMetallicRoughness.baseColorTexture.index);
		// 	LOG_INFO("Material metallicRoughnessTexture Index     :  {}", material.pbrMetallicRoughness.metallicRoughnessTexture.index);
		// }
		for(auto& scene : model.scenes)
		{

			std::vector<GLTF_NODE> gltf_nodes;

			for(const auto& node_id : scene.nodes)
			{
				auto& cur_node = model.nodes[node_id];
				traverse_node(node_id, -1, gltf_nodes, scene, model);			
			}

			std::map<int, std::pair<std::shared_ptr<MeshObject>, int>> objects_map;
			// LOG_INFO("\n{}", gltf_nodes);


			/* First create all 'empty' mesh_ objects */
			for(auto& gltf_node : gltf_nodes)
			{

				auto mesh_object = node_to_mesh_object(model, gltf_node.id);
				objects_map[gltf_node.id] = {mesh_object, gltf_node.parent_id};
			}

			for(const auto& obj : objects_map)
			{
				auto parent_id = obj.second.second;
				auto mesh_obj = obj.second.first;
				if(parent_id != -1)
				{
					mesh_obj->SetParent(objects_map[parent_id].first.get());
				}
				mesh_objects.push_back(mesh_obj);
			}


		}

        return mesh_objects;
    }

    Mesh GLTFLoader::ConvertToOrbitalsMesh(tinygltf::Model& model, tinygltf::Mesh gltf_mesh)
	{

		Mesh result_mesh;
		// LOG_INFO("Converting mesh to engine format");
		for(const auto& mesh_primitive : gltf_mesh.primitives)
		{   
			bool convertedToTriangleList = true;

			const auto & indices_accessor = model.accessors[mesh_primitive.indices]; 
			const auto &buffer_view = model.bufferViews[indices_accessor.bufferView];
			const auto &buffer = model.buffers[buffer_view.buffer];      

			const auto dataAddress = buffer.data.data() + buffer_view.byteOffset +
										indices_accessor.byteOffset;   
			const auto byteStride = indices_accessor.ByteStride(buffer_view);
			const auto indices_count = indices_accessor.count;                                  
			// LOG_TRACE("\nIndices count : {}", indices_count);
			std::unique_ptr<intArrayBase> indicesArrayPtr = nullptr;
			switch (indices_accessor.componentType) 
			{
				// LOG_INFO("primitive");
				case TINYGLTF_COMPONENT_TYPE_BYTE:
					indicesArrayPtr =
						std::unique_ptr<intArray<char> >(new intArray<char>(
							arrayAdapter<char>(dataAddress, indices_count, byteStride)));
					break;

				case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
					indicesArrayPtr = std::unique_ptr<intArray<unsigned char> >(
						new intArray<unsigned char>(arrayAdapter<unsigned char>(
							dataAddress, indices_count, byteStride)));
					break;

				case TINYGLTF_COMPONENT_TYPE_SHORT:
					indicesArrayPtr =
						std::unique_ptr<intArray<short> >(new intArray<short>(
							arrayAdapter<short>(dataAddress, indices_count, byteStride)));
					break;

				case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
					indicesArrayPtr = std::unique_ptr<intArray<unsigned short> >(
						new intArray<unsigned short>(arrayAdapter<unsigned short>(
							dataAddress, indices_count, byteStride)));
					break;

				case TINYGLTF_COMPONENT_TYPE_INT:
					indicesArrayPtr = std::unique_ptr<intArray<int> >(new intArray<int>(
						arrayAdapter<int>(dataAddress, indices_count, byteStride)));
					break;

				case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
					indicesArrayPtr = std::unique_ptr<intArray<unsigned int> >(
						new intArray<unsigned int>(arrayAdapter<unsigned int>(
							dataAddress, indices_count, byteStride)));
					break;
				default:
					LOG_INFO("GLTF primitive : unknown component type");
					break;
			}
		
			const auto &indices = *indicesArrayPtr;
			std::vector<uint32_t> clean_indices;
			if (indicesArrayPtr) 
			{
				for (size_t i(0); i < indicesArrayPtr->size(); ++i) 
				{

					clean_indices.push_back((uint32_t)indices[i]);
				}

				for(const auto& attr : mesh_primitive.attributes)
				{
					const auto attribAccessor = model.accessors[attr.second];
					const auto &bufferView = model.bufferViews[attribAccessor.bufferView];
					const auto &buffer = model.buffers[bufferView.buffer];
					const auto dataPtr = buffer.data.data() + bufferView.byteOffset +
										attribAccessor.byteOffset;
					const auto byte_stride = attribAccessor.ByteStride(bufferView);
					const auto points_count = attribAccessor.count;
					// LOG_TRACE("\npts count : {}", points_count);
				}    
			}

			std::vector<Face> clean_faces;
			std::vector<Point> clean_points;

			std::vector<glm::vec3> clean_positions;
			std::vector<glm::vec3> clean_normals;
			std::vector<glm::vec2> clean_uvs;
			glm::vec3 pMin(0.0f);
			glm::vec3 pMax(0.0f);
			float scale = 1.0f;
			switch (mesh_primitive.mode) {

					case TINYGLTF_MODE_TRIANGLES:  // this is the simpliest case to handle

					{
					// std::cout << "TRIANGLES\n";

					for (const auto &attribute : mesh_primitive.attributes) 
					{
						const auto attribAccessor = model.accessors[attribute.second];
						const auto &bufferView =
							model.bufferViews[attribAccessor.bufferView];
						const auto &buffer = model.buffers[bufferView.buffer];
						const auto dataPtr = buffer.data.data() + bufferView.byteOffset +
											attribAccessor.byteOffset;
						const auto byte_stride = attribAccessor.ByteStride(bufferView);
						const auto count = attribAccessor.count;

						// std::cout << "current attribute has count " << count
						//         << " and stride " << byte_stride << " bytes\n";

						// std::cout << "attribute string is : " << attribute.first << '\n';
						if (attribute.first == "POSITION") {
						// std::cout << "found position attribute\n";

						// get the position min/max for computing the boundingbox
						pMin.x = (float)attribAccessor.minValues[0];
						pMin.y = (float)attribAccessor.minValues[1];
						pMin.z = (float)attribAccessor.minValues[2];
						pMax.x = (float)attribAccessor.maxValues[0];
						pMax.y = (float)attribAccessor.maxValues[1];
						pMax.z = (float)attribAccessor.maxValues[2];

						switch (attribAccessor.type) {
							case TINYGLTF_TYPE_VEC3: {
							switch (attribAccessor.componentType) 
							{
								case TINYGLTF_COMPONENT_TYPE_FLOAT:
								// std::cout << "Type is FLOAT\n";
								// // 3D vector of float
								v3fArray positions(
									arrayAdapter<v3f>(dataPtr, count, byte_stride));

								// std::cout << "positions's size : " << positions.size()
								//             << '\n';

								for (size_t i{0}; i < positions.size(); ++i) 
								{
									const auto v = positions[i];
									
									glm::vec3 pos;
									pos.x = v.x * scale;
									pos.y = v.y * scale;
									pos.z = v.z * scale;
									clean_positions.push_back(pos);
								}
							}
							break;
							case TINYGLTF_COMPONENT_TYPE_DOUBLE: 
							{
								switch (attribAccessor.type) 
								{
									case TINYGLTF_TYPE_VEC3: 
									{
										v3dArray positions(
											arrayAdapter<v3d>(dataPtr, count, byte_stride));
										for (size_t i{0}; i < positions.size(); ++i) 
										{
											const auto v = positions[i];
											std::cout << "positions[" << i << "]: (" << v.x
														<< ", " << v.y << ", " << v.z << ")\n";

											glm::vec3 pos;
											pos.x = (float)v.x * scale;
											pos.y = (float)v.y * scale;
											pos.z = (float)v.z * scale;
											clean_positions.push_back(pos);

										}
									} break;
									default:
										// TODO Handle error
										break;
								}
								break;
								default:
								break;
							}
							} break;
							}
						}

						if (attribute.first == "NORMAL") 
						{

							switch (attribAccessor.type) 
							{
								case TINYGLTF_TYPE_VEC3: 
								{
									switch (attribAccessor.componentType) 
									{
										case TINYGLTF_COMPONENT_TYPE_FLOAT: 
										{
											v3fArray normals(
												arrayAdapter<v3f>(dataPtr, count, byte_stride));

											for(size_t i(0); i < normals.size(); i++)
											{
												clean_normals.push_back(glm::vec3(normals[i].x, normals[i].y, normals[i].z));
											}
										} break;
										case TINYGLTF_COMPONENT_TYPE_DOUBLE: 
										{
											v3dArray normals(
												arrayAdapter<v3d>(dataPtr, count, byte_stride));

											for(size_t i(0); i < normals.size(); i++){
												clean_normals.push_back(glm::vec3((float)normals[i].x, (float)normals[i].y, (float)normals[i].z));
											}
										} break;
										default:
										{
											std::cerr << "Unhandeled componant type for normal\n";
										}
									}
								} break;
								default:
									std::cerr << "Unhandeled vector type for normal\n";
							}
						}
						// Face varying comment on the normals is also true for the UVs
						if (attribute.first == "TEXCOORD_0") 
						{
							switch (attribAccessor.type) 
							{
								case TINYGLTF_TYPE_VEC2: 
								{
									switch (attribAccessor.componentType) 
									{
										case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE :
										{

										}break;
										case TINYGLTF_COMPONENT_TYPE_FLOAT: {
											v2fArray uvs(
												arrayAdapter<v2f>(dataPtr, count, byte_stride));

											for(size_t i(0); i < uvs.size(); i++){
												clean_uvs.push_back(glm::vec2(uvs[i].x, 1.0f - (float)uvs[i].y));
											}


										} break;
										case TINYGLTF_COMPONENT_TYPE_DOUBLE: 
										{
											v2dArray uvs(
												arrayAdapter<v2d>(dataPtr, count, byte_stride));

											for(size_t i(0); i < uvs.size(); i++){
												clean_uvs.push_back(glm::vec2(uvs[i].x, uvs[i].y));
											}
										} break;
										default:
											std::cerr << "unrecognized vector type for UV";
										}
								} break;
								default:
									std::cerr << "unreconized componant type for UV";
							}
						}

						
					}
					break;

					default:
						break;

					// These aren't triangles:
					case TINYGLTF_MODE_POINTS:
					case TINYGLTF_MODE_LINE:
					case TINYGLTF_MODE_LINE_LOOP:
						std::cerr << "primitive is not triangle based, ignoring";
					}
				}

			// LOG_INFO("num positions = {}", clean_positions.size());    
			// LOG_INFO("num normals = {}", clean_normals.size());    
			// LOG_INFO("num uvs = {}", clean_uvs.size());    

			Mesh mesh;
			for(size_t i=0; i < clean_positions.size(); i++)
			{
				Point pt;
				pt.position = clean_positions[i];
				pt.normal = clean_normals[i];
				pt.t_coords = clean_uvs[i];

				clean_points.push_back(pt);
			}

			mesh.SetPoints(clean_points);

			for(size_t i=0; i<clean_indices.size()/3; i++){
				Face face;
				auto idx1 = clean_indices[(i*3)+0];
				auto idx2 = clean_indices[(i*3)+1];
				auto idx3 = clean_indices[(i*3)+2];
				face.SetVertices( {Vertex(idx1), Vertex(idx2), Vertex(idx3)});
				clean_faces.push_back(face);
			}

			mesh.SetFaces(clean_faces);

			result_mesh = MeshUtils::Merge(result_mesh, mesh);
		}	

		result_mesh.ComputeAABB();
		
		return result_mesh;
	}

} /* end namespace GLR*/