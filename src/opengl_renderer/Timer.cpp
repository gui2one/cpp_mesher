#include "Timer.h"
namespace GLR{
	Timer::Timer()
	{		

		m_elapsed = 0;
		m_delta = 0;
		m_now = m_start;
		m_now_old = m_start;	
	}

    void Timer::Start()
    {
		b_Started = true;
		m_start = std::chrono::system_clock::now();
		m_elapsed = 0;
		m_delta = 0;
		m_now = m_start;
		m_now_old = m_start;		
    }

    void Timer::Update()
    {

        if( b_Started)
		{
			m_now = std::chrono::system_clock::now();
			auto delay = std::chrono::duration_cast<std::chrono::milliseconds>(m_now - m_start);
			m_elapsed = (int)delay.count(); // holy graal !!!

			auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(m_now - m_now_old);
			m_delta = (int)delta.count();
			m_now_old = m_now;
		}
    }

    float Timer::GetDeltaTime()
	{

		return (float)m_delta / 1000.0f;
	}

	float Timer::GetElapsedTime()
	{
		return (float)m_elapsed / 1000.0f;
	}
}
