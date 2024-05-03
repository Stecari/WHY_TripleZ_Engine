#pragma once
#include "Event.h"

namespace TribleZ
{
	/*-------------����(ֵ)------------------------------------------------------------------------------------------------------------------------------------------------*/
	class TRIBLEZ_API EventKey : public Event
	{
	protected:
		int m_KeyCode;

		EventKey(int code)		//�ܱ����Ĺ��캯�����㲻��ã�Ҳ����ʵ����EventKey����
			:m_KeyCode(code){}

	public:
		inline int GetKeyCode() { return m_KeyCode; }

		EVENT_CLASS_CATEGORY_def(EventCategoryInput | EventCategoryKeyboard)

	};


	/*-------------���°���------------------------------------------------------------------------------------------------------------------------------------------------*/
	class TRIBLEZ_API EventKeyPress : public EventKey
	{
	private:
		int m_RepeatCount;

	public:
		EventKeyPress(int keycode, int repeatcount)
			:EventKey(keycode), m_RepeatCount(repeatcount) {}	//������Ĵ����г�ʼ������(ֻҪ��������಻�ᱻ����ʵ���������Ҳ���ô�����Ͳ����ܻᱻ��ʼ����)

		inline int GetRepeatCount() { return m_RepeatCount; }

		std::string ToString() const override
		{
			std::stringstream str_stream;
			str_stream << "EventKeyPress: " << m_KeyCode << "  (" << m_RepeatCount << "repeats)";
			return str_stream.str();
		}

		EVENT_CLASS_TYPE_def(KeyPressed)
			
	};



	/*--------------�����ظ�����------------------------------------------------------------------------------------------------------------------------------------------*/
	class TRIBLEZ_API EventKeyRepeat : public EventKey
	{
	private:
		int m_RepeatCount;
	public:
		EventKeyRepeat(int code, int count)
			:EventKey(code), m_RepeatCount(count)
		{
		}

		std::string ToString() const override
		{
			std::stringstream str_stream;
			str_stream << "EventKeyRepeat: " << m_KeyCode << "  (" << m_RepeatCount << "repeats)";
			return str_stream.str();
		}

		EVENT_CLASS_TYPE_def(KeyPressed)
	};



	/*--------------�ɿ�����-----------------------------------------------------------------------------------------------------------------------------------------------*/
	class TRIBLEZ_API EventKeyReleased : public EventKey
	{
	public:
		EventKeyReleased(int keycode)
			:EventKey(keycode)
		{
		}


		std::string ToString() const override
		{
			std::stringstream str_stream;
			str_stream << " EventKeyReleased! ";
			return str_stream.str();
		}

		EVENT_CLASS_TYPE_def(KeyReleased)

	};

	class TRIBLEZ_API EventKeyTypes : public EventKey
	{
	public:
		EventKeyTypes(int keycode)
			:EventKey(keycode)
		{
		}

		std::string ToString() const override
		{
			std::stringstream str_stream;
			str_stream << " EventKeyTypes! " << m_KeyCode;
			return str_stream.str();
		}

		EVENT_CLASS_TYPE_def(KeyTypes)

	};
}