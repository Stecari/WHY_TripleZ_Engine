#include "tzpch.h"
#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace TribleZ
{
	std::shared_ptr<spdlog::logger> Log::m_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::m_ClientLogger;

	void Log::Init()
	{
		spdlog::set_pattern(">> %^ [Process %p][Thread %t] |%T| %n : %v%$  <<");	//������ɫ������id,�߳�id,ʱ�������־���֣���Ϣ

		m_CoreLogger = spdlog::stdout_color_mt("TribleZ");		//�ܼǵþ�һ������
		m_CoreLogger->set_level(spdlog::level::trace);
		m_CoreLogger->debug("Loading...");

		m_ClientLogger = spdlog::stdout_color_mt("Client");
		m_ClientLogger->set_level(spdlog::level::trace);
		m_CoreLogger->debug("Loading...");
	}
}