#ifndef APPLICATION_H
#define APPLICATION_H

#pragma once


#include "Window.h"
#include "Render/OpenGLRenderer.h"
#include "GLFW/glfw3.h"
namespace GLR{
class Application
{
public:
    ~Application();

    void Init(const char * title = "Glander v0.03 Alpha | Glandage V0.01 Alpha");

    OpenGLRenderer& GetRenderer(){ return m_Renderer; }
    Window& GetWindow() { return m_Window; }
    GLFWwindow* GetGLFWWindow() { return m_Window.GetNativeWindow(); }

    static Application* GetInstance();
private:

    static Application* s_Instance;
    Window m_Window;
    OpenGLRenderer m_Renderer;

protected:
    Application();
    Application(const Application& other) = delete;
};
}

#endif