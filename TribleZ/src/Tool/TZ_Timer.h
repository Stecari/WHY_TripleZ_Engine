#pragma once

#include <chrono>

namespace Hazel {

	class Timer
	{
	public:
		Timer()
		{
			Reset();
		}
		//将开始时间设置成现在时间
		void Timer::Reset()		//额。。。。他既然加上了Timer::那就一定有它的道理
		{
			m_Start = std::chrono::high_resolution_clock::now();
		}

		//换算成
		float Timer::Elapsed()
		{
			return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - m_Start).count() * 0.001f * 0.001f * 0.001f;
		}

		//换算成
		float Timer::ElapsedMillis()
		{
			return Elapsed() * 1000.0f;
		}

	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
	};

}