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
// #include "nodes/node_params.h"
namespace NodeEditor {

class MeshOperator : public ImGuiNode{
public:
    MeshOperator(): ImGuiNode("default"){};
    virtual ~MeshOperator() = default;
    virtual void Generate() = 0;

public :
    msh::Mesh m_MeshCache;
};

class MeshGenerator : public MeshOperator
{
public:
    MeshGenerator():MeshOperator(){
        SetNumAvailableInputs(0);
        color = NODE_COLOR::DARK_GREEN;
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
        length = std::make_shared<Param<float>>("length", 1.0f);
        cols = std::make_shared<Param<uint32_t>>("cols", 32);
        rows = std::make_shared<Param<uint32_t>>("rows", 32);

        m_ParamLayout.items = {
            { "width", width },
            { "length", length },
            { "cols", cols },
            { "rows", rows }
        };
        

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
        radius2 = std::make_shared<Param<float>>("radius2", 1.0f);
        height = std::make_shared<Param<float>>("height", 2.0f);
        cols = std::make_shared<Param<uint32_t>>("cols", 32);
        rows = std::make_shared<Param<uint32_t>>("rows", 32);

        m_ParamLayout.items = {
            { "radius1", radius1 },
            { "radius2", radius2 },
            { "height", height },
            { "cols", cols },
            { "rows", rows }
        };

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
    MeshModifier():MeshOperator(){
        color = NODE_COLOR::MAROON;
    };
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

        transform_order = std::make_shared<ParamComboBox>("transform order");
        transform_order->SetChoices({"T/R/S", "T/S/R", "R/T/S", "R/S/T", "S/T/R", "S/R/T"});
        translate = std::make_shared<Param<glm::vec3>>("translate", glm::vec3(0.0f, 0.0f, 0.0f));
        rotate = std::make_shared<Param<glm::vec3>>("rotate", glm::vec3(0.0f, 0.0f, 0.0f));
        scale = std::make_shared<Param<glm::vec3>>("scale", glm::vec3(1.0f, 1.0f, 1.0f));


        m_ParamLayout.items = {
            { "transform order", transform_order },
            { "translate", translate },
            { "rotate", rotate },
            { "scale", scale }
            };
    };

    ~TransformModifier(){};

    void Generate() override{
        if( GetInput(0) != nullptr) {
            auto op0 = static_cast<MeshOperator*>(GetInput(0).get());
            
            m_MeshCache = op0->m_MeshCache;
            if(transform_order->GetChoice() == 0){

                msh::meshutils::translate(m_MeshCache, translate->Eval());
                msh::meshutils::rotate(m_MeshCache, glm::radians(rotate->Eval()));
                msh::meshutils::scale(m_MeshCache, scale->Eval());
            }else if(transform_order->GetChoice() == 1){

                msh::meshutils::translate(m_MeshCache, translate->Eval());
                msh::meshutils::scale(m_MeshCache, scale->Eval());
                msh::meshutils::rotate(m_MeshCache, glm::radians(rotate->Eval()));
            }else if(transform_order->GetChoice() == 2){

                msh::meshutils::rotate(m_MeshCache, glm::radians(rotate->Eval()));
                msh::meshutils::translate(m_MeshCache, translate->Eval());
                msh::meshutils::scale(m_MeshCache, scale->Eval());
            }else if(transform_order->GetChoice() == 3){

                msh::meshutils::rotate(m_MeshCache, glm::radians(rotate->Eval()));
                msh::meshutils::scale(m_MeshCache, scale->Eval());
                msh::meshutils::translate(m_MeshCache, translate->Eval());
            }else if(transform_order->GetChoice() == 4){

                msh::meshutils::scale(m_MeshCache, scale->Eval());
                msh::meshutils::translate(m_MeshCache, translate->Eval());
                msh::meshutils::rotate(m_MeshCache, glm::radians(rotate->Eval()));
            }else if(transform_order->GetChoice() == 5){

                msh::meshutils::scale(m_MeshCache, scale->Eval());
                msh::meshutils::rotate(m_MeshCache, glm::radians(rotate->Eval()));
                msh::meshutils::translate(m_MeshCache, translate->Eval());
            }
        }
    }

public:
    std::shared_ptr<Param<glm::vec3>> translate;
    std::shared_ptr<Param<glm::vec3>> rotate;
    std::shared_ptr<Param<glm::vec3>> scale;
    std::shared_ptr<ParamComboBox> transform_order;
};

class NoiseDisplaceModifier : public MeshModifier
{
public:
    NoiseDisplaceModifier() : MeshModifier() {
        SetNumAvailableInputs(1);
        lacunarity = std::make_shared<Param<float>>("lacunarity", 2.7f);
        gain = std::make_shared<Param<float>>("gain", 0.65f);
        amplitude = std::make_shared<Param<float>>("amplitude", 0.1f);
        frequency = std::make_shared<Param<float>>("frequency", 2.35f);
        weightedStrength = std::make_shared<Param<float>>("weightedStrength", 1.0f);
        offset = std::make_shared<Param<glm::vec3>>("offset", glm::vec3(0.0f, 0.0f, 0.0f));
        seed = std::make_shared<Param<uint32_t>>("seed", 0);
        octaves = std::make_shared<Param<uint32_t>>("octaves", 4);

        precompute_normals = std::make_shared<Param<bool>>("precompute normals", false);

        m_ParamLayout.items = {
            { "label", std::make_shared<ParamLabel>("Noise") },
            { "lacunarity", lacunarity },
            { "gain", gain },
            { "amplitude", amplitude },
            { "frequency", frequency },
            { "weightedStrength", weightedStrength },
            { "offset", offset },
            { "seed", seed },
            { "octaves", octaves },
            { "separator", std::make_shared<ParamSeparator>("-------------")},
            { "precompute normals", precompute_normals }
        };

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

    std::shared_ptr<Param<bool>> precompute_normals;
};

class MeshMerger : public MeshModifier
{
public:
    MeshMerger() : MeshModifier() {
        SetNumAvailableInputs(2);
        color = NODE_COLOR::DARK_GREY;
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

class MeshMergerMulti : public MeshModifier
{
public:
    MeshMergerMulti() : MeshModifier() {
        ActivateMultiInput();
        SetNumAvailableInputs(0);
        color = NODE_COLOR::DARK_GREY;
    };
    ~MeshMergerMulti(){};

    void Generate() override{
        msh::Mesh result;
        for(size_t i = 0; i < GetMultiInputCount(); i++) {
            if( GetMultiInput(i) != nullptr) {
                result = msh::meshutils::merge(result, static_cast<MeshOperator*>(GetMultiInput(i).get())->m_MeshCache);   
            }
            
        }

        m_MeshCache = result;
        
        // if( GetInput(0) != nullptr && GetInput(1) != nullptr) {
        //     auto op0 = static_cast<MeshOperator*>(GetInput(0).get());
        //     auto op1 = static_cast<MeshOperator*>(GetInput(1).get());
        //     m_MeshCache = msh::meshutils::merge(op0->m_MeshCache, op1->m_MeshCache);
        // }
    }
};

class MeshTwister : public MeshModifier
{
public:
    MeshTwister() : MeshModifier() {
        SetNumAvailableInputs(1);
        color = NODE_COLOR::DARK_GREY;

        turns = std::make_shared<Param<float>>("turns", 1.0f);

        m_ParamLayout.items = {
            {"Turns", turns}
        };
    }

    ~MeshTwister(){};

    void Generate() override{
        if( GetInput(0) != nullptr) {
            auto op0 = static_cast<MeshOperator*>(GetInput(0).get());
            m_MeshCache = op0->m_MeshCache;
            msh::meshutils::Twist(m_MeshCache, turns->Eval());
        }
    }
public:
 std::shared_ptr<Param<float>> turns;
}; 


class NullMeshOperator : public MeshModifier{
public:
    NullMeshOperator():MeshModifier(){
        SetNumAvailableInputs(1);
        color = NODE_COLOR::ORANGE;
    };
    ~NullMeshOperator(){};

    void Generate() override{
        if( GetInput(0) != nullptr) {
            auto op0 = static_cast<MeshOperator*>(GetInput(0).get());
            m_MeshCache = op0->m_MeshCache;
        }
    }
};

}; // end namespace NodeEditor

#endif // MESHOPERATORS_H