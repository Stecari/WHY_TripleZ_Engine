#pragma once

#include "algorithm"
#include "fstream"
#include "string"
#include "chrono"
#include "thread"
#include "functional"

namespace TribleZ
{
	/*
	*	基准测试用的结构体数据
	*
	*/
	struct BenchMarking_Timer_Profile
	{
		std::string name;
		long long start, end;
		unsigned int thread_id;
	};

	struct InstrumentationSession
	{
		std::string name;
	};

	class Instrumentor
	{
	private:
		InstrumentationSession* m_CurrentSession;
		std::ofstream m_OutputStream;
		int m_ProfileCount;
	public:
		Instrumentor()
			:m_CurrentSession(nullptr), m_ProfileCount(0)
		{}

		void BeginSession(const std::string& name, const std::string& filepath = "result.json")
		{
			m_OutputStream.open(filepath);
			WriteHeader();
			m_CurrentSession = new InstrumentationSession({ name });
		}

		void EndSession()
		{
			WriteFooter();
			m_OutputStream.close();
			delete m_CurrentSession;
			m_CurrentSession = nullptr;
			m_ProfileCount = 0;

		}

		void WriteProfile(const BenchMarking_Timer_Profile& result)
		{
			if (m_ProfileCount++ > 0)	m_OutputStream << ",";

			std::string name = result.name;
			std::replace(name.begin(), name.end(), '"', '\'');

			m_OutputStream << "{";
			m_OutputStream << "\"cat\":\"function\",";	//"cat":"function"
			m_OutputStream << "\"dur\":" << (result.end - result.start) << ",";	//"dur":result.end - result.start，
			m_OutputStream << "\"name\" :\"" << name << "\",";
			m_OutputStream << "\"ph\":\"X\",";
			m_OutputStream << "\"pid\":0,";
			m_OutputStream << "\"tid\":" << result.thread_id << ",";
			m_OutputStream << "\"ts\":" << result.start;
			m_OutputStream << "}";

			m_OutputStream.flush();
		}

		void WriteHeader()
		{
			m_OutputStream << "{\"otherData\":{},\"traceEvents\":[";
			m_OutputStream.flush();
		}

		void WriteFooter()
		{
			m_OutputStream << "]}";
			m_OutputStream.flush();
		}

		static Instrumentor& Get()
		{
			static Instrumentor* instence = new Instrumentor();
			return *instence;
		}
	};



	/*
	*	基准测试用的计时器
	*
	*/
	class TracingTime
	{
	private:
		std::chrono::time_point<std::chrono::steady_clock> Start_time;
		const char* Name;
		bool Stop_flag;
	public:
		TracingTime(const char* name)
			:Name(name), Stop_flag(false)
		{
			Start_time = std::chrono::high_resolution_clock::now();
		}

		~TracingTime()
		{
			if (!Stop_flag)
			{
				Stop();
			}
		}

	private:
		void Stop()
		{
			auto End_time = std::chrono::high_resolution_clock::now();

			long long start = std::chrono::time_point_cast<std::chrono::microseconds>(Start_time).time_since_epoch().count();
			long long end = std::chrono::time_point_cast<std::chrono::microseconds>(End_time).time_since_epoch().count();

			float D_time = (end - start) * 0.001f;
			unsigned int thread_id = std::hash<std::thread::id>{}(std::this_thread::get_id());

			Instrumentor::Get().WriteProfile({ Name, start, end, thread_id });

			Stop_flag = true;
		}
	};
}

#define TZ_PROFILING_TURN_ON 1		//用于打开内存跟踪，基准化测试的宏

#ifdef  TZ_PROFILING_TURN_ON
	#define TZ_PROFILE_BEGIN_SESSION(name, filepath)	::TribleZ::Instrumentor::Get().BeginSession(name, filepath);
	#define TZ_PROFILE_END_SESSION()					::TribleZ::Instrumentor::Get().EndSession();
	#define TZ_PROFILE_SCOPE(name)						::TribleZ::TracingTime time##__LINE__(name);	//将代码中使用这段宏的行数当作TracingTime变量名字的后缀
	#define TZ_PROFILE_FUNCTION()						TZ_PROFILE_SCOPE(__FUNCTION__)			//将代码中使用这段宏的函数的名字当作PROFILE_SCOPE（name）中的name
	#define TZ_PROFILE_FUNCTION_SIG()					TZ_PROFILE_SCOPE(__FUNCSIG__)			//将代码中使用这段宏的函数的名签名(就是加上参数列表)当作PROFILE_SCOPE（name）中的name
#else
	#define TZ_PROFILE_BEGIN_SESSION(name, filepath)	
	#define TZ_PROFILE_END_SESSION()					
	#define TZ_PROFILE_SCOPE(name)
	#define TZ_PROFILE_FUNCTION()
	#define TZ_PROFILE_FUNCTION_SIG()
#endif // TZ_PROFILING_TURN_ON