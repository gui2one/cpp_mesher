#include "CameraOrbitController.h"


namespace GLR{
CameraOrbitController::CameraOrbitController(): m_Window(nullptr),
          m_Camera(nullptr),
          m_Activated(true),
          m_UvPos(glm::vec2(glm::pi<float>() / 2.0f, glm::pi<float>() * 0.22f)),
          m_Radius(5.0f),
          m_Center(glm::vec3(0.f)),
          m_CursorDelta(glm::vec2(0.f)),
          m_CursorOldPos(glm::vec2(0.f))
{

}

CameraOrbitController::~CameraOrbitController()
{

}

void CameraOrbitController::Init(GLFWwindow* window,std::shared_ptr<GLR::Camera> camera)
{
    m_Camera = camera;
    m_Window = window;
}
glm::vec3 CameraOrbitController::FromPolar(glm::vec2 uv_pos)
{
    glm::vec3 result;

    result.x = m_Radius * sin(uv_pos.y) * cos(uv_pos.x);
    result.y = m_Radius * cos(uv_pos.y);
    result.z = m_Radius * sin(uv_pos.y) * sin(uv_pos.x);

    return result;
}
void CameraOrbitController::Update(float delta_time)
{
       if (m_Activated)
        {
            m_CursorDelta = Input::GetMousePos(m_Window) - m_CursorOldPos;

            /*
                ORBIT
            */

            if ( Input::IsMouseButtonClicked(m_Window, 0))
            {
                m_UvPos += glm::vec2(m_CursorDelta.x * delta_time, m_CursorDelta.y * -delta_time);

                float radians_limit = (float)glm::pi<float>() * 0.05f;
                // printf("uv pos Y : %.3f\n", m_uvPos.y);
                if (m_UvPos.y > (float)glm::pi<float>() - radians_limit)
                    m_UvPos.y = (float)glm::pi<float>() - radians_limit;
                else if (m_UvPos.y < radians_limit)
                    m_UvPos.y = radians_limit;
            }


            /*
                PAN
            */

            if (Input::IsMouseButtonClicked(m_Window, 2)) 
            {
                //LOG_TRACE("panning");
                glm::vec3 up = glm::vec3(0.f, 1.f, 0.f);
                glm::vec3 dir = glm::normalize(m_Camera->m_Position - m_Camera->target_position);
                glm::vec3 cross = glm::normalize(glm::cross(dir, up));

                glm::vec3 crossY = glm::rotate(glm::vec4(cross, 1.0f), glm::radians(-90.0f), dir);

                m_Center += (cross * m_CursorDelta.x * delta_time);
                m_Center += (crossY * m_CursorDelta.y * delta_time);
                m_Camera->target_position = m_Center;

            }

            m_CursorOldPos = Input::GetMousePos(m_Window);
        }
        m_Camera->m_Position = FromPolar(m_UvPos) + m_Center;    
}
}
