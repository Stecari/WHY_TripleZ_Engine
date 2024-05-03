#pragma once
#include "Event.h"

namespace TribleZ
{
	/*-------------��갴��------------------------------------------------------------------------------------------------------------------------------------------------*/
	class TRIBLEZ_API EventMouseButton : public Event
	{
	protected:
		int m_Button;

		EventMouseButton(int button)
			:m_Button(button)	{}

	public:

		inline int GetMouseButton() { return m_Button; }

	EVENT_CLASS_CATEGORY_def(EventCategoryInput | EventCategoryMouse | EventCategoryMouseBotten)
	};




	/*-------------�������------------------------------------------------------------------------------------------------------------------------------------------------*/
	class TRIBLEZ_API EventMouseButtonPressed : public EventMouseButton
	{
	public:
		EventMouseButtonPressed(int button)
			:EventMouseButton(button) 
		{}

		std::string ToString() const override
		{
			std::stringstream str_stream;
			str_stream << "EventMouseButtonPrssed[ " << m_Button << " ]";
			return str_stream.str();
		}

		EVENT_CLASS_TYPE_def(MouseButtonPressed)
	};




	/*-------------�ɿ����------------------------------------------------------------------------------------------------------------------------------------------------*/
	class TRIBLEZ_API EventMouseButtonReleased : public EventMouseButton
	{
	public:
		EventMouseButtonReleased(int button)
			:EventMouseButton(button)
		{}

		std::string ToString() const override
		{
			std::stringstream str_stream;
			str_stream << "EventMouseButtonReleased[ " << m_Button << " ]";
			return str_stream.str();
		}

		EVENT_CLASS_TYPE_def(MouseButtonReleased)
	};




	/*-------------�ƶ����------------------------------------------------------------------------------------------------------------------------------------------------*/
	class TRIBLEZ_API EventMouseMoved : public Event
	{
	private:
		float MouseX, MouseY;

	public:

		EventMouseMoved(float x, float y)
			:MouseX(x), MouseY(y)
		{}

		inline float GetMouseX() const { return MouseX; }
		inline float GetMouseY() const { return MouseY; }

		std::string ToString() const override
		{
			std::stringstream str_stream;
			str_stream << "EventMouseMoved[ " << "X: " << MouseX << "  Y: " << MouseY << " ]";
			return str_stream.str();
		}

		EVENT_CLASS_CATEGORY_def(EventCategoryInput | EventCategoryMouse)

		EVENT_CLASS_TYPE_def(MouseMoved)
	};



	/*-------------������------------------------------------------------------------------------------------------------------------------------------------------------*/
	class TRIBLEZ_API EventMouseScrolled : public Event
	{
	private:
		float OffsetX, OffsetY;		//������ƫ����

	public:
		EventMouseScrolled(float offset_x, float offset_y)
			:OffsetX(offset_x), OffsetY(offset_y)
		{}

		inline float GetOffsetX() const { return OffsetX; }
		inline float GetOffsetY() const { return OffsetY; }

		std::string ToString() const override
		{
			std::stringstream str_stream;
			str_stream << "EventMouseScrolled[ " << "X: " << OffsetX << "  Y: " << OffsetY << " ]";
			return str_stream.str();
		}

		EVENT_CLASS_CATEGORY_def(EventCategoryInput | EventCategoryMouse)

		EVENT_CLASS_TYPE_def(MouseScrolled)
	};
}