#ifndef MSH_APPLICATION_H
#define MSH_APPLICATION_H

#pragma once
#include <chrono>
#include <thread>

#include "BaseApplication.h"
#include "MeshExporter.h"
#include "custom_params.h"
#include "node_editor.h"
#include "nodes/MeshOperators.h"
#include "openmesh/openmesh_operators.h"
#include "yaml_serialize.h"

// #include "Serialize.h"
namespace msh {

struct WindowData {
  int width = 1280;
  int height = 720;

  int mouseX = 0;
  int mouseY = 0;
  const char* title = "The Node Editor | Another one ?!!";
};

struct ApplicationData {
  bool mouse_over_opengl_renderer = false;
};
class Application : public NED::BaseApplication {
 public:
  ApplicationData m_UserAppData;

 public:
  Application();

  bool Init();

  void ExportTempMesh();
};

};  // namespace msh
#endif