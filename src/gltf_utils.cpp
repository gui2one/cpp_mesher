#include "gltf_utils.h"

namespace msh{

    void WriteMeshToGLTF(const Mesh &mesh, const std::string &outputFile) {
        tinygltf::Model outputModel;
        tinygltf::Scene scene;
        outputModel.scenes.push_back(scene);
        scene.name = "Scene";
        tinygltf::Node *node = new tinygltf::Node();
        scene.nodes.push_back((int)outputModel.nodes.size());
        outputModel.nodes.push_back(*node);
        node->name = "Mesh";
        node->mesh = 0;
        outputModel.meshes.resize(1);
        tinygltf::Mesh& outputMesh = outputModel.meshes[0];
        outputMesh.primitives.resize(1);
        tinygltf::Primitive& outputPrimitive = outputMesh.primitives[0];
        outputPrimitive.mode = 4;
        outputPrimitive.attributes["POSITION"] = (int)outputModel.accessors.size();
        outputModel.accessors.resize(1);
        tinygltf::Accessor& outputAccessor = outputModel.accessors[0];
        outputAccessor.componentType = 5;
        outputAccessor.count = mesh.GetPoints().size();
        outputAccessor.type = TINYGLTF_TYPE_VEC3;
        outputAccessor.bufferView = 0;
        outputModel.bufferViews.resize(1);
        tinygltf::BufferView& outputBufferView = outputModel.bufferViews[0];
        outputBufferView.buffer = 0;
        outputBufferView.byteStride = 12;
        outputBufferView.target = 34962;
        outputModel.buffers.resize(1);
        tinygltf::Buffer& outputBuffer = outputModel.buffers[0];

        tinygltf::TinyGLTF gltfWriter;
        if (!gltfWriter.WriteGltfSceneToFile(&outputModel, outputFile, true, true, true, true)) {
            std::cerr << "Failed to write GLB file." << std::endl;
        } else {
            std::cout << "Successfully wrote GLB file: " << outputFile << std::endl;
        }
    }
}
