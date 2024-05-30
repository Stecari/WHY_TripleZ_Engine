#pragma once
#include "tzpch.h"

#include "Core.h"
/*		//Hazel2D里面有人加的
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"
*/
// This ignores all warnings raised inside External headers
#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

namespace TribleZ
{

	class TRIBLEZ_API Log
	{
	private:
		static Ref<spdlog::logger> m_CoreLogger;
		static Ref<spdlog::logger> m_ClientLogger;
	public:
		static void Init();

		static Ref<spdlog::logger>& GetCoreLogger() { return m_CoreLogger; }
		static Ref<spdlog::logger>& GetClientLogger() { return m_ClientLogger; }
	};


}

/*Hazel2D里面有人加的*/
/*
template<typename OStream, glm::length_t L, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::vec<L, T, Q>& vector)
{
	return os << glm::to_string(vector);
}

template<typename OStream, glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::mat<C, R, T, Q>& matrix)
{
	return os << glm::to_string(matrix);
}

template<typename OStream, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, glm::qua<T, Q> quaternion)
{
	return os << glm::to_string(quaternion);
}
*/

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