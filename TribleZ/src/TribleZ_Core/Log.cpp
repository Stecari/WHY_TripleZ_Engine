#include "tzpch.h"
#include "Log.h"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace TribleZ
{
	Ref<spdlog::logger> Log::m_CoreLogger;
	Ref<spdlog::logger> Log::m_ClientLogger;

	void Log::Init()
	{
		spdlog::set_pattern(">> %^ [Process %p][Thread %t] |%T| %n : %v%$  <<");	//设置颜色，过程id,线程id,时间戳，日志名字，信息

		m_CoreLogger = spdlog::stdout_color_mt("TribleZ");		//总记得就一次有用
		m_CoreLogger->set_level(spdlog::level::trace);
		m_CoreLogger->debug("Loading...");

		m_ClientLogger = spdlog::stdout_color_mt("Client");
		m_ClientLogger->set_level(spdlog::level::trace);
		m_CoreLogger->debug("Loading...");
	}

	/*		//cherno的
	void Log::Init()
	{
		std::vector<spdlog::sink_ptr> logSinks;
		logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
		logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("Hazel.log", true));

		logSinks[0]->set_pattern("%^[%T] %n: %v%$");
		logSinks[1]->set_pattern("[%T] [%l] %n: %v");

		s_CoreLogger = std::make_shared<spdlog::logger>("HAZEL", begin(logSinks), end(logSinks));
		spdlog::register_logger(s_CoreLogger);
		s_CoreLogger->set_level(spdlog::level::trace);
		s_CoreLogger->flush_on(spdlog::level::trace);

		s_ClientLogger = std::make_shared<spdlog::logger>("APP", begin(logSinks), end(logSinks));
		spdlog::register_logger(s_ClientLogger);
		s_ClientLogger->set_level(spdlog::level::trace);
		s_ClientLogger->flush_on(spdlog::level::trace);
	}*/
}