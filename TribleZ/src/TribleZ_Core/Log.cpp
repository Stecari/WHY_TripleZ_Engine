#include "tzpch.h"
#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace TribleZ
{
	std::shared_ptr<spdlog::logger> Log::m_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::m_ClientLogger;

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
}