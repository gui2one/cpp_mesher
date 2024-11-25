#ifndef OPENMESH_OPERATORS_H
#define OPENMESH_OPERATORS_H
#pragma once
#include <ImGuiNode.h>



#include "openmesh_utils.h"

namespace NED {
    

    class OpenMeshOperator : public ImGuiNode<GMesh> {
        public:
        OpenMeshOperator() : ImGuiNode("default") {}
        virtual ~OpenMeshOperator() = default;
        virtual void Generate() = 0;
    };

    class OpenMeshNullOperator : public OpenMeshOperator {
    public:
    OpenMeshNullOperator() : OpenMeshOperator() {
        SetNumAvailableInputs(1);
        color = NODE_COLOR::ORANGE;
    };
    ~OpenMeshNullOperator() {};

    void Generate() override {
        if (GetInput(0) != nullptr) {
        auto op0 = static_cast<OpenMeshOperator *>(GetInput(0).get());
        m_DataCache = op0->m_DataCache;
        }
    }
    };

    class OpenMeshSubnetOperator : public SubnetNode<GMesh> {
    public:
    OpenMeshSubnetOperator() : SubnetNode() {};
    ~OpenMeshSubnetOperator() = default;

    void Generate() override {
        if (node_network.outuput_node != nullptr) {
        // node_network.outuput_node->Update();
        auto op = std::dynamic_pointer_cast<OpenMeshOperator>(node_network.outuput_node);
        if (op != nullptr) {
            std::cout << "Generate Subnet Data cache ????" << std::endl;
            m_DataCache = op->m_DataCache;
        }
        }
    }

    public:
    // msh::Mesh m_MeshCache;
    };

    class OpenMeshCubeGenerator : public OpenMeshOperator {
        public:
        OpenMeshCubeGenerator() : OpenMeshOperator() { 
            color = NODE_COLOR::DARK_GREEN; 
            SetNumAvailableInputs(0);    
        }
        ~OpenMeshCubeGenerator() {}
        void Generate() override { m_DataCache = openmeshutils::openmesh_cube(); }
    };

    class OpenMeshSquareGenerator : public OpenMeshOperator {
        public:
        OpenMeshSquareGenerator() : OpenMeshOperator() { 
            color = NODE_COLOR::MAROON; 
            SetNumAvailableInputs(0);    
        }
        ~OpenMeshSquareGenerator() {}
        void Generate() override { m_DataCache = openmeshutils::openmesh_square(); }
    };
};

#endif // OPENMESH_OPERATORS_H