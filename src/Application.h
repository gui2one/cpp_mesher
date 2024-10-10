#ifndef MSH_APPLICATION_H
#define MSH_APPLICATION_H

#pragma once
#include <chrono>
#include <thread>
#include "node_editor.h"
#include "nodes/MeshOperators.h"
namespace msh {

struct WindowData{
    int width = 1280;
    int height = 720;

    int mouseX = 0;
    int mouseY = 0;
    const char* title = "The Node Editor | Another one ?!!";
};

class Application
{
public:
    Application();
    ~Application();

    inline NodeEditor::NodeManager& GetNodeManager() { return m_NodeManager; }
    inline GLFWwindow* GetWindow() { return m_NativeWindow; }
    bool Init();
    void InitEvents();
    void Run();

public :
    ImVec2 m_Origin;
    WindowData m_WindowData;

    ImFont* m_RegularFont;
    ImFont* m_BoldFont;    

private:
    void ImGuiInit(GLFWwindow* window);
    void ImGuiBeginFrame();
    void ImGuiEndFrame();

private:
    GLFWwindow* m_NativeWindow;
    NodeEditor::NodeManager m_NodeManager;

};

};
#endif