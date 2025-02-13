#ifndef SCENE_H
#define SCENE_H

#pragma once

#include "Objects/BoxLineObject.h"
#include "Objects/Entity3d.h"
#include "Objects/LightObject.h"
#include "Objects/MeshObject.h"
#include "Objects/MeshObjectDebugDraw.h"
#include "Objects/ParticleSystem.h"
#include "Render/OpenGLHDRSkybox.h"
#include "Render/OpenGLSkybox.h"

namespace GLR {

class Scene {
 public:
  Scene();
  ~Scene();

  void Add(std::shared_ptr<MeshObject> object);
  void Add(std::vector<std::shared_ptr<GLR::MeshObject>> objects);

  void Add(std::shared_ptr<LightObject> light);
  void Add(std::shared_ptr<BoxLineObject> box);
  void Add(std::shared_ptr<MeshObjectDebugDraw> debug_draw);
  void Add(std::shared_ptr<Camera> camera);
  void Add(std::shared_ptr<ParticleSystem> psystem);

  void SetSkybox(std::shared_ptr<Skybox> skybox) { m_Skybox = skybox; }
  std::shared_ptr<Skybox> GetSkybox() const { return m_Skybox; }

  void SetDisplaySkyBox(bool value) { m_DisplaySkybox = value; }
  bool GetDisplaySkybox() const { return m_DisplaySkybox; }

  void SetHDRSkybox(std::shared_ptr<HDRSkybox> hdr_skybox) { m_HDRSkybox = hdr_skybox; }
  std::shared_ptr<HDRSkybox> GetHDRSkybox() const { return m_HDRSkybox; }

  std::vector<std::shared_ptr<Entity3d>> GetEntities() const { return m_Entities; }

  std::vector<std::shared_ptr<MeshObject>> GetObjects() const { return m_MeshObjects; }
  std::vector<std::shared_ptr<LightObject>> GetLights() const { return m_Lights; }
  std::vector<std::shared_ptr<BoxLineObject>> GetBoxLineObjects() const { return m_BoxLineObjects; }
  std::vector<std::shared_ptr<MeshObjectDebugDraw>> GetDebugDraws() const { return m_DebugDraws; }
  std::vector<std::shared_ptr<ParticleSystem>> GetParticleSystems() const { return m_ParticleSystems; }
  std::vector<std::shared_ptr<Camera>> GetCameras() const { return m_Cameras; }

  std::vector<std::shared_ptr<Entity3d>> GetSelectedEntities();

 private:
  std::vector<std::shared_ptr<Entity3d>> m_Entities;
  std::vector<std::shared_ptr<LightObject>> m_Lights;
  std::vector<std::shared_ptr<MeshObject>> m_MeshObjects;
  std::vector<std::shared_ptr<BoxLineObject>> m_BoxLineObjects;
  std::vector<std::shared_ptr<MeshObjectDebugDraw>> m_DebugDraws;
  std::vector<std::shared_ptr<ParticleSystem>> m_ParticleSystems;
  std::vector<std::shared_ptr<Camera>> m_Cameras;

  std::shared_ptr<Skybox> m_Skybox = nullptr;
  std::shared_ptr<HDRSkybox> m_HDRSkybox = nullptr;
  bool m_DisplaySkybox;
};
}  // namespace GLR

#endif