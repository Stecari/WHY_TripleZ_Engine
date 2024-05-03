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

		/*----------���������------------------------------------------------*/
		TimeStep operator+(float time) { return m_Time + time; };
		TimeStep operator-(float time) { return m_Time - time; };
		/*----------ת��������------------*/
		operator float() const { return m_Time; }//��ǿ����ʽת��������
		/*--------------------------------------------------------------------*/


		inline float GetSecond() const { return m_Time; }
		inline float GetMilliSecond() const { return m_Time * 1000; }
	
	};
}