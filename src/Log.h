#pragma once


#include <spdlog/spdlog.h>
#include <memory>
#include <stdarg.h>
#include <ostream>
#include <sstream>
namespace msh {



class Log
{
public:
	static std::stringbuf m_Buffer;
	static std::ostream* s_Ostream;
public:
	static void Init();
	inline static std::ostream& custom_stdout() { return *s_Ostream;  }
	inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
	inline static std::shared_ptr<spdlog::logger>& GetSimpleLogger() { return s_SimpleLogger; }
private:
	static std::shared_ptr<spdlog::logger> s_CoreLogger;
	static std::shared_ptr<spdlog::logger> s_SimpleLogger;
};

}

#define LOG_TRACE(...) TCK::Log::GetSimpleLogger()->trace(__VA_ARGS__);
#define LOG_INFO(...) SPDLOG_INFO(__VA_ARGS__)
#define LOG_WARN(...) SPDLOG_WARN(__VA_ARGS__)
#define LOG_ERROR(...) SPDLOG_ERROR(__VA_ARGS__)
#define LOG_CRITICAL(...) SPDLOG_CRITICAL(__VA_ARGS__)
