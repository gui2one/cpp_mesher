#ifndef WINDOW_H
#define WINDOW_H

#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
namespace GLR{

    

struct DebugData
{
    int value;
};

struct WindowData
{
    bool b_Resized = true; /* set to true so that first frame triggers resize */
    uint32_t m_Width = 1280;
    uint32_t m_Height = 720;
};

class Window
{
public:
    Window();
    ~Window();

    void Init(const char * title = "default window title ... change that...");

    GLFWwindow* GetNativeWindow(){ return m_Window;}
    WindowData& GetWindowData(){ return m_WindowData;}

    bool ShouldClose(){ return glfwWindowShouldClose(m_Window); }

    void SetTitle(const char* str);
private:

    GLFWwindow* m_Window;
    WindowData m_WindowData;


    DebugData * m_DebugData;
};
}

#endif