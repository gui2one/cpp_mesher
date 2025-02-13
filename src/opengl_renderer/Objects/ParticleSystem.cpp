#include "ParticleSystem.h"

namespace GLR{

ParticleSystem::ParticleSystem()
{

}

ParticleSystem::~ParticleSystem()
{

}

void ParticleSystem::InitRenderData()
{
    m_InstanceBuffer->InitRenderData();
}

void ParticleSystem::BindRenderData()
{
    if(m_InstanceBuffer)
    {
        m_InstanceBuffer->BindRenderData();
    }

}

void ParticleSystem::Render()
{
    m_InstanceBuffer->BindRenderData();
    glDrawElementsInstanced(GL_TRIANGLES, m_InstanceBuffer->GetMesh()->GetIndices().size(), GL_UNSIGNED_INT, nullptr, m_InstanceBuffer->GetMesh()->GetIndices().size());
}

}