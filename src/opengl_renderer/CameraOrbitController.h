#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#pragma once

#include "GLFW/glfw3.h"
#include "Input.h"
#include "Render/Camera.h"

namespace GLR {
class CameraOrbitController {
 public:
  std::shared_ptr<GLR::Camera> m_Camera;
  GLFWwindow* m_Window;
  bool m_Activated;

  glm::vec2 m_CursorDelta;
  glm::vec2 m_CursorOldPos;

  glm::vec2 m_UvPos;
  float m_Radius;
  glm::vec3 m_Center;

 public:
  CameraOrbitController();
  ~CameraOrbitController();

  void Init(GLFWwindow* window, std::shared_ptr<GLR::Camera> camera);

  glm::vec3 FromPolar(glm::vec2 uv_pos);
  void Activate(bool val) { m_Activated = val; }
  void Reset();
  void Update(float delta_time);

 private:
};
}  // namespace GLR

#endif