#include "gltf_utils.h"

namespace msh{

    void WriteMeshToGLTF(const Mesh &mesh, const std::string &outputFile) {
        tinygltf::Model model;
        tinygltf::TinyGLTF gltf;

        // Create a buffer to store binary data (vertices, indices, etc.)
        tinygltf::Buffer buffer;

        // Convert vertices to binary
        tinygltf::BufferView bufferView;
        tinygltf::Accessor accessor;

        // Vertex positions
        bufferView.buffer = 0;
        bufferView.byteOffset = 0;
        bufferView.byteLength = mesh.GetPoints().size() * sizeof(Vertex);
        bufferView.target = TINYGLTF_TARGET_ARRAY_BUFFER;
        model.bufferViews.push_back(bufferView);

        accessor.bufferView = 0;
        accessor.byteOffset = 0;
        accessor.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT;
        accessor.count = mesh.GetPoints().size();
        accessor.type = TINYGLTF_TYPE_VEC3;
        accessor.minValues = {0, 0, 0}; // Optionally, set min values for bounding box
        accessor.maxValues = {1, 1, 1}; // Optionally, set max values for bounding box
        model.accessors.push_back(accessor);

        // Convert indices to binary
        bufferView.buffer = 0;
        bufferView.byteOffset = bufferView.byteLength;
        bufferView.byteLength = mesh.GetIndices().size() * sizeof(unsigned int);
        bufferView.target = TINYGLTF_TARGET_ELEMENT_ARRAY_BUFFER;
        model.bufferViews.push_back(bufferView);

        accessor.bufferView = 1;
        accessor.byteOffset = 0;
        accessor.componentType = TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT;
        accessor.count = mesh.GetIndices().size();
        accessor.type = TINYGLTF_TYPE_SCALAR;
        model.accessors.push_back(accessor);

        // Fill buffer data
        buffer.data.insert(buffer.data.end(),
                        reinterpret_cast<const unsigned char*>(mesh.GetPoints().data()),
                        reinterpret_cast<const unsigned char*>(mesh.GetPoints().data()) + mesh.GetPoints().size() * sizeof(Vertex));
        buffer.data.insert(buffer.data.end(),
                        reinterpret_cast<const unsigned char*>(mesh.GetIndices().data()),
                        reinterpret_cast<const unsigned char*>(mesh.GetIndices().data()) + mesh.GetIndices().size() * sizeof(unsigned int));

        model.buffers.push_back(buffer);

        // Define a mesh
        tinygltf::Mesh gltfMesh;
        tinygltf::Primitive primitive;
        primitive.attributes["POSITION"] = 0; // Corresponds to the first accessor
        primitive.indices = 1; // Corresponds to the second accessor
        primitive.mode = TINYGLTF_MODE_TRIANGLES; // Assuming your mesh is made of triangles
        gltfMesh.primitives.push_back(primitive);
        model.meshes.push_back(gltfMesh);

        // Define a node
        tinygltf::Node node;
        node.mesh = 0; // The first (and only) mesh
        model.nodes.push_back(node);

        // Define a scene
        tinygltf::Scene scene;
        scene.nodes.push_back(0); // The first (and only) node
        model.scenes.push_back(scene);
        model.defaultScene = 0;

        // Write to file
        gltf.WriteGltfSceneToFile(&model, outputFile, true, true, true, true);
    }
}
