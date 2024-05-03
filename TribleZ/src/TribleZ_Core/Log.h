#pragma once
#include "tzpch.h"

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace TribleZ
{

	class TRIBLEZ_API Log
	{
	private:
		static std::shared_ptr<spdlog::logger> m_CoreLogger;
		static std::shared_ptr<spdlog::logger> m_ClientLogger;
	public:
		static void Init();

		inline static std::shared_ptr < spdlog::logger >& GetCoreLogger() { return m_CoreLogger; }
		inline static std::shared_ptr < spdlog::logger >& GetClientLogger() { return m_ClientLogger; }
	};


}


//Core层的日志输出等级宏定义
#define TZ_CORE_TRACE(...)		::TribleZ::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define TZ_CORE_INFO(...)		::TribleZ::Log::GetCoreLogger()->info(__VA_ARGS__)
#define TZ_CORE_WARN(...)		::TribleZ::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define TZ_CORE_ERROR(...)		::TribleZ::Log::GetCoreLogger()->error(__VA_ARGS__)
#define TZ_CORE_CRITICAL(...)	::TribleZ::Log::GetCoreLogger()->critical(__VA_ARGS__)


//Client层的日志输出等级宏定义
#define TZ_CLIENT_TRACE(...)	::TribleZ::Log::GetClientLogger()->trace(__VA_ARGS__)
#define TZ_CLIENT_INFO(...)		::TribleZ::Log::GetClientLogger()->info(__VA_ARGS__)
#define TZ_CLIENT_WARN(...)		::TribleZ::Log::GetClientLogger()->warn(__VA_ARGS__)
#define TZ_CLIENT_ERROR(...)	::TribleZ::Log::GetClientLogger()->error(__VA_ARGS__)
#define TZ_CLIENT_CRITICAL(...) ::TribleZ::Log::GetClientLogger()->critical(__VA_ARGS__)