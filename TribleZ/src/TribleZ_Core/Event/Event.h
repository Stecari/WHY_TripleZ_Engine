#pragma once
#include "tzpch.h"
#include "../Core.h"


namespace TribleZ
{
	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMove,	//���ڣ��رգ����ţ��۽�����ʧ�������ƶ�
		AppTick, AppUpData, AppRender,											//APP 
		KeyPressed, KeyReleased, KeyTypes,										//���̣����£��ɿ�, ���֣�
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled		//��꣺���£��ɿ����ƶ�������
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

/*##��ʾ���������ǰ��Ĳ���(EventType::)����һ�����壬#��ʾת�ַ���*/
#define EVENT_CLASS_TYPE_def(type)		static EventType GetStaticType() { return EventType::##type; }\
										virtual EventType GetEventType() const override { return GetStaticType(); }\
										virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY_def(category)		virtual int GetCategoryFlag() const override { return category; }



	/*-------------�¼�-----------------------------------------------------------------------------------------------------------------------------------------------------*/
	class Event
	{
		friend class EventDispatcher;
	public:
		virtual EventType GetEventType() const = 0;
		virtual int GetCategoryFlag() const = 0;
		virtual const char* GetName() const = 0;						//Debug
		virtual std::string ToString() const { return GetName(); }		//Debug
	
		
		inline bool Filter_IsInCategory(EventCategory target_category)	//��λ����������
		{
			return ( GetCategoryFlag() & target_category );
		}
		bool m_Handle = false;	//�����־λ����¼�¼��Ƿ��Ѿ�����
	protected:					//��д��ջʱ������Ҫ��ĳһ��������������¼�Ϊ�Ѵ�������������Ĳ�Ͳ��ô�������
	};						


	/*-----------�����¼����ͣ��������-----------------------------------------------------------------------------------------------------------------------------------------------------*/
	class EventDispatcher
	{
	private:
		Event& m_Event;

		template<typename T>
		using EvenFn = std::function<bool(T&)>;	//������һ���ض��ĺ���ָ��

	public:
		EventDispatcher(Event& event)
			:m_Event(event)
		{}

		template<typename T>
		bool DisPatcher(EvenFn<T> func)
		{
			if (m_Event.GetEventType() == T::GetStaticType())//����������ľ�̬���ͺʹ�������ĳ�Ա��̬����һ��
			{						/*������һ����������EvenFn���ͣ�Ȼ����m_Event����������*/
				m_Event.m_Handle = func(*(T*)&m_Event);	//��ת���������ָ�룬��ȡ*
				return true;
			}
			return false;
		}


	};

	//����־׼���ģ����������ǵ��¼����Ը��õĵ���Tostring
	inline std::ostream& operator<<(std::ostream& os, const Event& E)
	{
		return  os << E.ToString();
	}

}