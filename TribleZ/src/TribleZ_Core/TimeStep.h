#pragma once

namespace TribleZ
{
	class TimeStep
	{
	private:
		float m_Time;
	public:
		TimeStep(float time = 0.0f)
			:m_Time(time)
		{

		}

		/*----------运算符重载------------------------------------------------*/
		TimeStep operator+(float time) { return m_Time + time; };
		TimeStep operator-(float time) { return m_Time - time; };
		/*----------转换符重载------------*/
		operator float() const { return m_Time; }//增强了隐式转换的能力
		/*--------------------------------------------------------------------*/


		inline float GetSecond() const { return m_Time; }
		inline float GetMilliSecond() const { return m_Time * 1000; }
	
	};
}