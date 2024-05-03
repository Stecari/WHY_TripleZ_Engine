#pragma once
#include "iostream"
#include "chrono"
#include "BenchMarking.h"

/*
*	��׼�����õļ�ʱ��
* 
*/
template<typename Fn>
class Time
{
private:
	std::chrono::time_point<std::chrono::steady_clock> Start_time;
	const char* Name;	//��̬�ַ�����std::stringΪ�ѷ��䣬���������ջ���䣬���Լ���Ǳ�ڵĶѷ��俪��
	bool Stop_flag;
	Fn m_Func;
public:
	Time(const char* name, Fn&& func)	//&&����������ֵ
		:Name(name), Stop_flag(false), m_Func(func)
	{
		Start_time = std::chrono::high_resolution_clock::now();
	}

	~Time()
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

		m_Func({ Name, D_time });
		//std::cout << Name << ": " << D_time << " ms" << std::endl;

		Stop_flag = true;
	}
};
