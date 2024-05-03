#pragma once
#include "tzpch.h"
#include "../Core.h"


namespace TribleZ
{
	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMove,	//窗口：关闭，缩放，聚焦？，失焦？，移动
		AppTick, AppUpData, AppRender,											//APP 
		KeyPressed, KeyReleased, KeyTypes,										//键盘：按下，松开, 打字？
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled		//鼠标：按下，松开，移动，滚轮
	};

	enum EventCategory
	{
		None = 0,
		EventCategoryApplication	= BIT(0),
		EventCategoryInput			= BIT(1),
		EventCategoryKeyboard		= BIT(2),
		EventCategoryMouse			= BIT(3),
		EventCategoryMouseBotten	= BIT(4)

	};

/*##表示将宏参数和前面的部分(EventType::)连成一个整体，#表示转字符串*/
#define EVENT_CLASS_TYPE_def(type)		static EventType GetStaticType() { return EventType::##type; }\
										virtual EventType GetEventType() const override { return GetStaticType(); }\
										virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY_def(category)		virtual int GetCategoryFlag() const override { return category; }



	/*-------------事件-----------------------------------------------------------------------------------------------------------------------------------------------------*/
	class Event
	{
		friend class EventDispatcher;
	public:
		virtual EventType GetEventType() const = 0;
		virtual int GetCategoryFlag() const = 0;
		virtual const char* GetName() const = 0;						//Debug
		virtual std::string ToString() const { return GetName(); }		//Debug
	
		
		inline bool Filter_IsInCategory(EventCategory target_category)	//按位与分类过滤器
		{
			return ( GetCategoryFlag() & target_category );
		}
		bool m_Handle = false;	//处理标志位，记录事件是否已经处理
	protected:					//在写层栈时，我们要在某一个层中设置这个事件为已处理，这样子下面的层就不用处理它了
	};						


	/*-----------检验事件类型，交付句柄-----------------------------------------------------------------------------------------------------------------------------------------------------*/
	class EventDispatcher
	{
	private:
		Event& m_Event;

		template<typename T>
		using EvenFn = std::function<bool(T&)>;	//代表了一个特定的函数指针

	public:
		EventDispatcher(Event& event)
			:m_Event(event)
		{}

		template<typename T>
		bool DisPatcher(EvenFn<T> func)
		{
			if (m_Event.GetEventType() == T::GetStaticType())//如果传经来的静态类型和存在里面的成员静态类型一样
			{						/*这里是一个函数，即EvenFn类型，然后传入m_Event变量做参数*/
				m_Event.m_Handle = func(*(T*)&m_Event);	//先转换成子类的指针，再取*
				return true;
			}
			return false;
		}


	};

	//给日志准备的，这样子我们的事件可以更好的调用Tostring
	inline std::ostream& operator<<(std::ostream& os, const Event& E)
	{
		return  os << E.ToString();
	}

}