#include "Scene.h"

namespace GLR {

Scene::Scene() : m_DisplaySkybox(true) {}

Scene::~Scene() {}

void Scene::Add(std::shared_ptr<MeshObject> object) {
  // LOG_INFO("adding Mesh object");
  m_Entities.push_back(object);
  m_MeshObjects.push_back(object);
}

void Scene::Add(std::vector<std::shared_ptr<GLR::MeshObject>> objects) {
  for (auto gltf : objects) {
    Add(gltf);
  }
}

void Scene::Add(std::shared_ptr<LightObject> light) {
  // LOG_INFO("adding Light object");
  m_Entities.push_back(light);
  m_Lights.push_back(light);
}

void Scene::Add(std::shared_ptr<BoxLineObject> box) {
  // LOG_INFO("adding Light object");
  m_Entities.push_back(box);
  m_BoxLineObjects.push_back(box);
}

void Scene::Add(std::shared_ptr<MeshObjectDebugDraw> debug_draw) {
  // LOG_INFO("adding Light object");
  m_Entities.push_back(debug_draw);
  m_DebugDraws.push_back(debug_draw);
}

void Scene::Add(std::shared_ptr<ParticleSystem> psystem) {
  // LOG_INFO("adding Particle System");
  m_Entities.push_back(psystem);
  m_ParticleSystems.push_back(psystem);
}

void Scene::Add(std::shared_ptr<Camera> camera) {
  // LOG_INFO("adding Light object");
  m_Entities.push_back(camera);
  m_Cameras.push_back(camera);
}

std::vector<std::shared_ptr<Entity3d>> Scene::GetSelectedEntities() {
  std::vector<std::shared_ptr<Entity3d>> entities;
  for (const auto entity : GetEntities()) {
    if (entity->m_Selected) entities.push_back(entity);
  }
  return entities;
}
}  // namespace GLR