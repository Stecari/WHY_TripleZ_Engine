#pragma once
#include "Event.h"

namespace TribleZ
{
	/*-------------关闭窗口------------------------------------------------------------------------------------------------------------------------------------------------*/
	class TRIBLEZ_API EventWindowClose : public Event
	{
	public:
		EventWindowClose(){}

		EVENT_CLASS_CATEGORY_def(EventCategoryApplication)

		EVENT_CLASS_TYPE_def(WindowClose)
	};


	
	/*-------------缩放窗口------------------------------------------------------------------------------------------------------------------------------------------------*/
	class TRIBLEZ_API EventWindowResize : public Event
	{
	private:
		unsigned int m_Width, m_Height;
	public:
		EventWindowResize(unsigned int width, unsigned int height)
			:m_Width(width), m_Height(height)
		{}

		inline unsigned int GetWindowsWidth() const { return m_Width; }
		inline unsigned int GetWindowsHeight() const { return m_Height; }

		std::string ToString() const override
		{
			std::stringstream str_stream;
			str_stream << "EventWindowResize[ " << "Width: " << m_Width << "  Height: " << m_Height << " ]";
			return str_stream.str();
		}

		EVENT_CLASS_CATEGORY_def(EventCategoryApplication)

		EVENT_CLASS_TYPE_def(WindowResize)
	};



	///*-------------窗口------------------------------------------------------------------------------------------------------------------------------------------------*/
	//class TRIBLEZ_API EventWindowFocus : public Event
	//{
	//public:


	//};



	///*-------------窗口------------------------------------------------------------------------------------------------------------------------------------------------*/
	//class TRIBLEZ_API EventWindowLostFocus : public Event
	//{
	//public:


	//};




	///*-------------移动窗口------------------------------------------------------------------------------------------------------------------------------------------------*/
	//class TRIBLEZ_API EventWindowMove : public Event
	//{
	//public:


	//};



	/*-------------应用时钟响应------------------------------------------------------------------------------------------------------------------------------------------------*/
	class TRIBLEZ_API EventAppTick : public Event
	{
	public:
		EventAppTick(){}
		
		EVENT_CLASS_CATEGORY_def(EventCategoryApplication)

		EVENT_CLASS_TYPE_def(AppTick)
	};



	/*-------------数据传输------------------------------------------------------------------------------------------------------------------------------------------------*/
	class TRIBLEZ_API EventAppUpData : public Event
	{
	public:
		EventAppUpData() {}

		EVENT_CLASS_CATEGORY_def(EventCategoryApplication)

		EVENT_CLASS_TYPE_def(AppUpData)
	};



	/*-------------应用渲染------------------------------------------------------------------------------------------------------------------------------------------------*/
	class TRIBLEZ_API EventAppRender : public Event
	{
	public:
		EventAppRender() {}

		EVENT_CLASS_CATEGORY_def(EventCategoryApplication)

		EVENT_CLASS_TYPE_def(AppRender)
	};

}
