#ifndef MESH_OPERATORS_H
#define MESH_OPERATORS_H
#pragma once

#include "ImguiNode.h"
#include "NodeParam.h"
#include <type_traits>
#include <iostream>
#include <string>
#include <stdint.h>

#include "mesh.h"
#include "mesh_utils.h"
#include "nodes/node_params.h"
namespace NodeEditor {

class MeshOperator : public ImGuiNode{
public:
    MeshOperator(): ImGuiNode("default"){};
    virtual ~MeshOperator() = default;
    virtual void Generate() = 0;

public :
    msh::Mesh m_MeshCache;

    // std::array<MeshOperator*, 4> inputs = { nullptr, nullptr, nullptr, nullptr }; 
};

class MeshGenerator : public MeshOperator
{
public:
    MeshGenerator():MeshOperator(){
        SetNumAvailableInputs(0);
    };
    ~MeshGenerator(){};
    virtual void Generate() = 0;
private:

};

class SquareGenerator : public MeshGenerator
{
public:
    SquareGenerator():MeshGenerator(){};
    ~SquareGenerator(){};

    void Generate() override{
        m_MeshCache = msh::meshutils::generateGrid(1.0f, 1.0f, 1, 1);
    }
private:
};


class GridGenerator : public MeshGenerator
{
public:
    GridGenerator():MeshGenerator(){

        width = std::make_shared<Param<float>>("width", 1.0f);
        m_Params.push_back(width);
        length = std::make_shared<Param<float>>("length", 1.0f);
        m_Params.push_back(length);

        cols = std::make_shared<Param<uint32_t>>("cols", 32);
        m_Params.push_back(cols);
        rows = std::make_shared<Param<uint32_t>>("rows", 32);
        m_Params.push_back(rows);
    };
    ~GridGenerator(){};

    void Generate() override{
        m_MeshCache = msh::meshutils::generateGrid(width->Eval(), length->Eval(), cols->Eval(), rows->Eval());
    }

    std::shared_ptr<Param<float>> width;
    std::shared_ptr<Param<float>> length;
    std::shared_ptr<Param<uint32_t>> cols;
    std::shared_ptr<Param<uint32_t>> rows;
private:
};
class TubeGenerator : public MeshGenerator
{
public:
    TubeGenerator():MeshGenerator(){

        radius1 = std::make_shared<Param<float>>("radius1", 1.0f);
        m_Params.push_back(radius1);
        radius2 = std::make_shared<Param<float>>("radius2", 1.0f);
        m_Params.push_back(radius2);
        height = std::make_shared<Param<float>>("height", 2.0f);
        m_Params.push_back(height);
        cols = std::make_shared<Param<uint32_t>>("cols", 32);
        m_Params.push_back(cols);
        rows = std::make_shared<Param<uint32_t>>("rows", 32);
        m_Params.push_back(rows);
    };
    ~TubeGenerator(){};

    void Generate() override{
        m_MeshCache = msh::meshutils::generateTube(radius1->Eval(), radius2->Eval(), height->Eval(), cols->Eval(), rows->Eval());
    }

    std::shared_ptr<Param<float>> radius1;
    std::shared_ptr<Param<float>> radius2;
    std::shared_ptr<Param<float>> height;
    std::shared_ptr<Param<uint32_t>> cols;
    std::shared_ptr<Param<uint32_t>> rows;
private:
};

class MeshModifier : public MeshOperator
{
public:
    MeshModifier():MeshOperator(){};
    ~MeshModifier(){};
    virtual void Generate() = 0;
private:

};

class NormalModifier : public MeshModifier
{
public:
    NormalModifier() : MeshModifier() {
        SetNumAvailableInputs(1);
    };
    ~NormalModifier(){};

    void Generate() override{
        if( GetInput(0) != nullptr) {
            auto op0 = static_cast<MeshOperator*>(GetInput(0).get());
            
            m_MeshCache = op0->m_MeshCache;
            m_MeshCache.ComputeNormals();
        }
    }
};

class TransformModifier : public MeshModifier
{
public:
    TransformModifier() : MeshModifier() {
        SetNumAvailableInputs(1);

        translate = std::make_shared<Param<glm::vec3>>("translate", glm::vec3(0.0f, 0.0f, 0.0f));
        m_Params.push_back(translate);

        rotate = std::make_shared<Param<glm::vec3>>("rotate", glm::vec3(0.0f, 0.0f, 0.0f));
        m_Params.push_back(rotate);

        scale = std::make_shared<Param<glm::vec3>>("scale", glm::vec3(1.0f, 1.0f, 1.0f));
        m_Params.push_back(scale);
    };

    ~TransformModifier(){};

    void Generate() override{
        if( GetInput(0) != nullptr) {
            auto op0 = static_cast<MeshOperator*>(GetInput(0).get());
            
            m_MeshCache = op0->m_MeshCache;

            msh::meshutils::translate(m_MeshCache, translate->Eval());
            msh::meshutils::rotate(m_MeshCache, rotate->Eval());
            msh::meshutils::scale(m_MeshCache, scale->Eval());
        }
    }

public:
    std::shared_ptr<Param<glm::vec3>> translate;
    std::shared_ptr<Param<glm::vec3>> rotate;
    std::shared_ptr<Param<glm::vec3>> scale;
};

class NoiseDisplaceModifier : public MeshModifier
{
public:
    NoiseDisplaceModifier() : MeshModifier() {
        SetNumAvailableInputs(1);
        lacunarity = std::make_shared<Param<float>>("lacunarity", 2.7f);
        m_Params.push_back(lacunarity);
        gain = std::make_shared<Param<float>>("gain", 0.65f);
        m_Params.push_back(gain);
        amplitude = std::make_shared<Param<float>>("amplitude", 0.1f);
        m_Params.push_back(amplitude);
        frequency = std::make_shared<Param<float>>("frequency", 2.35f);
        m_Params.push_back(frequency);
        weightedStrength = std::make_shared<Param<float>>("weightedStrength", 1.0f);
        m_Params.push_back(weightedStrength);
        offset = std::make_shared<Param<glm::vec3>>("offset", glm::vec3(0.0f, 0.0f, 0.0f));
        m_Params.push_back(offset);
        seed = std::make_shared<Param<uint32_t>>("seed", 0);
        m_Params.push_back(seed);
        octaves = std::make_shared<Param<uint32_t>>("octaves", 4);
        m_Params.push_back(octaves);
    };
    ~NoiseDisplaceModifier(){};

    void Generate() override{
        if( GetInput(0) != nullptr) {
            auto op0 = static_cast<MeshOperator*>(GetInput(0).get());
            
            m_MeshCache = op0->m_MeshCache;
            msh::meshutils::NoiseParams noiseParams;
            noiseParams.lacunarity = lacunarity->Eval();
            noiseParams.gain = gain->Eval();
            noiseParams.amplitude = amplitude->Eval();
            noiseParams.frequency = frequency->Eval();
            noiseParams.weightedStrength = weightedStrength->Eval();
            noiseParams.offset = offset->Eval();
            noiseParams.seed = seed->Eval();
            noiseParams.octaves = octaves->Eval();
            msh::meshutils::NoiseDisplace(m_MeshCache, noiseParams);
            // m_MeshCache.ComputeNormals();
        }
    }

    std::shared_ptr<Param<float>> lacunarity;
    std::shared_ptr<Param<float>> gain;
    std::shared_ptr<Param<float>> amplitude;
    std::shared_ptr<Param<float>> frequency;
    std::shared_ptr<Param<float>> weightedStrength;
    std::shared_ptr<Param<glm::vec3>> offset;
    std::shared_ptr<Param<uint32_t>> seed;
    std::shared_ptr<Param<uint32_t>> octaves;
};

class MeshMerger : public MeshModifier
{
public:
    MeshMerger() : MeshModifier() {
        SetNumAvailableInputs(2);
    };
    ~MeshMerger(){};

    void Generate() override{
        if( GetInput(0) != nullptr && GetInput(1) != nullptr) {
            auto op0 = static_cast<MeshOperator*>(GetInput(0).get());
            auto op1 = static_cast<MeshOperator*>(GetInput(1).get());
            m_MeshCache = msh::meshutils::merge(op0->m_MeshCache, op1->m_MeshCache);
        }
    }
};


class NullMeshOperator : public MeshModifier{
public:
    NullMeshOperator():MeshModifier(){
        SetNumAvailableInputs(1);
    };
    ~NullMeshOperator(){};

    void Generate() override{
        if( GetInput(0) != nullptr) {
            auto op0 = static_cast<MeshOperator*>(GetInput(0).get());
            m_MeshCache = op0->m_MeshCache;
        }
    }
};

template <typename T>
class Node : public T
{
    static_assert(std::is_base_of<MeshOperator, T>::value, "T must be derived from MeshOperator");
public:
    Node(const char * _title) {
        auto node = static_cast<ImGuiNode*>(this);
        if(std::is_base_of<MeshGenerator, T>::value) {
            node->color = NODE_COLOR::MAROON;
            node->title = _title;
        }else if(std::is_base_of<NullMeshOperator, T>::value) {
            node->color = NODE_COLOR::ORANGE;
            node->title = _title;
        }else if(std::is_base_of<MeshModifier, T>::value) {
            node->color = NODE_COLOR::DARK_GREEN;
            node->title = _title;
        }
    }

    void Update() {
        auto node = static_cast<ImGuiNode*>(this);
        auto op = static_cast<MeshOperator*>(this);
        for(uint32_t i = 0; i < MAX_N_INPUTS; i++) {
            if(node->GetInput(i) != nullptr) {
                node->GetInput(i)->Update(); /* Important !!*/
                auto opinput = static_cast<MeshOperator*>(node->GetInput(i).get());
                
                // op->SetInput(i, node->GetInput(i));
                // opinput->Generate();
            }
        }
        op->Generate();

        // 'final' output
        // std::cout << op->m_MeshCache << std::endl;
    }

    T* ToOperator() {
        return static_cast<T*>(this);
    }
};
};

#endif