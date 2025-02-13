#include "Application.h"

namespace GLR{

Application::Application()
{

}

Application::~Application()
{

}

void Application::Init(const char * title)
{
    GLR::Log::Init();

    m_Window.Init(title);
    
    m_Renderer.Init();
}

Application* Application::s_Instance = nullptr;

Application *GLR::Application::GetInstance()
{
    if( s_Instance == nullptr)
    {
        s_Instance = new Application();
    }
    return s_Instance;
}

}