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
		//����ʼʱ�����ó�����ʱ��
		void Timer::Reset()		//�����������Ȼ������Timer::�Ǿ�һ�������ĵ���
		{
			m_Start = std::chrono::high_resolution_clock::now();
		}

		//�����
		float Timer::Elapsed()
		{
			return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - m_Start).count() * 0.001f * 0.001f * 0.001f;
		}

		//�����
		float Timer::ElapsedMillis()
		{
			return Elapsed() * 1000.0f;
		}

	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
	};

}