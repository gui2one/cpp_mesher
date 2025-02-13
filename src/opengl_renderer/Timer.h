#ifndef TIMER_H
#define TIMER_H


using TimePoint = std::chrono::time_point<std::chrono::system_clock>;
namespace GLR{
	class Timer{
	public:
		Timer();

		void Start();
		void Update();
		float GetDeltaTime();
		float GetElapsedTime();
	private:
		TimePoint m_start{}; /* {} to initialize to ZERO */
		TimePoint m_now;
		TimePoint m_now_old;
		int m_elapsed; 
		int m_delta;

		bool b_Started = false;

	};
}
#endif /* TIMER_H */