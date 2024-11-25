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