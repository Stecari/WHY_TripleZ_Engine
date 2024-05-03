#pragma once
#include "Event.h"

namespace TribleZ
{
	/*-------------按键(值)------------------------------------------------------------------------------------------------------------------------------------------------*/
	class TRIBLEZ_API EventKey : public Event
	{
	protected:
		int m_KeyCode;

		EventKey(int code)		//受保护的构造函数，你不因该，也不能实例化EventKey对象
			:m_KeyCode(code){}

	public:
		inline int GetKeyCode() { return m_KeyCode; }

		EVENT_CLASS_CATEGORY_def(EventCategoryInput | EventCategoryKeyboard)

	};


	/*-------------按下按键------------------------------------------------------------------------------------------------------------------------------------------------*/
	class TRIBLEZ_API EventKeyPress : public EventKey
	{
	private:
		int m_RepeatCount;

	public:
		EventKeyPress(int keycode, int repeatcount)
			:EventKey(keycode), m_RepeatCount(repeatcount) {}	//在子类的创建中初始化基类(只要是这个基类不会被我们实例化，而且不这么搞他就不可能会被初始化了)

		inline int GetRepeatCount() { return m_RepeatCount; }

		std::string ToString() const override
		{
			std::stringstream str_stream;
			str_stream << "EventKeyPress: " << m_KeyCode << "  (" << m_RepeatCount << "repeats)";
			return str_stream.str();
		}

		EVENT_CLASS_TYPE_def(KeyPressed)
			
	};



	/*--------------长按重复按键------------------------------------------------------------------------------------------------------------------------------------------*/
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



	/*--------------松开按键-----------------------------------------------------------------------------------------------------------------------------------------------*/
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