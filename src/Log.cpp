#include "Log.h"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/ostream_sink.h>

#define SPDLOG_DEBUG_ON
namespace msh{

	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_SimpleLogger;


	std::stringbuf Log::m_Buffer;

	void Log::Init()
	{

		auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		console_sink->set_level(spdlog::level::trace);
		//console_sink->set_pattern("[multi_sink_example] [%^%l%$] %v");
	
		auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("multisink.txt", true);
		file_sink->set_level(spdlog::level::trace);
		
		spdlog::logger logger("Core", { console_sink});
		s_CoreLogger = std::make_shared<spdlog::logger>(logger);
		s_CoreLogger->set_pattern("%^%v %$[%s:%#][%!]");
		s_CoreLogger->set_level(spdlog::level::trace);
		s_CoreLogger->flush_on(spdlog::level::trace);
		spdlog::set_default_logger(s_CoreLogger);

		s_SimpleLogger = spdlog::stdout_color_mt("Simple");
		s_SimpleLogger->set_pattern("%^[%s:%#] %v%$");
		s_SimpleLogger->set_level(spdlog::level::trace);
		//spdlog::set_level(spdlog::level::trace);

	}
}
