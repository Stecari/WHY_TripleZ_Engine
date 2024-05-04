#include "tzpch.h"

#include "OrthoGraphicCameraContraller.h"

#include "TribleZ_core/Input.h"

#include "TribleZ_core/Mouse_and_Key_Code/MouseButtonCodeTable.h"
#include "TribleZ_Core/Mouse_and_Key_Code/KeyCodeTable.h"

namespace TribleZ
{
	OrthoGraphicCameraContraller::OrthoGraphicCameraContraller(float aspect_ratio, bool if_rotation)
		:m_AspectRatio(aspect_ratio),m_Bound({-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel }),
		 Rotation_flag(if_rotation), CameraPosition{ 0.0f, 0.0f, 0.0f }, CameraRotation(0.0f),
		 Camera(m_Bound.Left, m_Bound.Right, m_Bound.Bottom, m_Bound.Top)
	{
		TZ_PROFILE_FUNCTION_SIG();

		/*----------相机参数设置------------*/
		Camera.SetPosition(CameraPosition);
		Camera.SetRotation(CameraRotation);
		/*----------相机参数设置------------*/
	}

	void OrthoGraphicCameraContraller::OnUpdata(TimeStep time_step)
	{
		TZ_PROFILE_FUNCTION_SIG();

		/*-------------------------相机变换----------------------------------------------------*/
		if (Input::IsKeyPressed(TZ_KEY_W))
		{
			//CameraPosition.y += CameraPositionSpeed * time_step;	//这里我们重载了一个float()

			CameraPosition.y += glm::cos(glm::radians(CameraRotation)) * CameraPositionSpeed * time_step;	//这里我们重载了一个float()
			CameraPosition.x -= glm::sin(glm::radians(CameraRotation)) * CameraPositionSpeed * time_step;
		}
		else if (Input::IsKeyPressed(TZ_KEY_S))
		{
			//CameraPosition.y -= CameraPositionSpeed * time_step;	//隐式转换后这里写的是float(time_step)

			CameraPosition.y -= glm::cos(glm::radians(CameraRotation)) * CameraPositionSpeed * time_step;	//隐式转换后这里写的是float(time_step)
			CameraPosition.x += glm::sin(glm::radians(CameraRotation)) * CameraPositionSpeed * time_step;
		}
		if (Input::IsKeyPressed(TZ_KEY_A))
		{
			CameraPosition.x -= glm::cos(glm::radians(CameraRotation)) * CameraPositionSpeed * time_step;//相当于是time_step.GetSecond()
			CameraPosition.y -= glm::sin(glm::radians(CameraRotation)) * CameraPositionSpeed * time_step;
		}
		else if (Input::IsKeyPressed(TZ_KEY_D))
		{
			CameraPosition.x += glm::cos(glm::radians(CameraRotation)) * CameraPositionSpeed * time_step;
			CameraPosition.y += glm::sin(glm::radians(CameraRotation)) * CameraPositionSpeed * time_step;
		}
		/*----------相机参数设置------------*/
		Camera.SetPosition(CameraPosition);
		CameraPositionSpeed = m_ZoomLevel;
		/*----------相机参数设置------------*/
		if (Rotation_flag)
		{
			if (Input::IsKeyPressed(TZ_KEY_E))
			{
				CameraRotation -= CameraRotationSpeed * time_step.GetSecond();
			}
			if (Input::IsKeyPressed(TZ_KEY_Q))
			{
				CameraRotation += CameraRotationSpeed * time_step.GetSecond();
			}
			/*----------相机参数设置------------*/
			Camera.SetRotation(CameraRotation);
			/*----------相机参数设置------------*/
		}
		/*-------------------------相机变换----------------------------------------------------*/
	}

	void OrthoGraphicCameraContraller::OnEvent(Event& event)
	{
		TZ_PROFILE_FUNCTION_SIG();

		EventDispatcher event_dispatcher(event);
		event_dispatcher.DisPatcher<EventMouseScrolled>(TZ_CORE_BIND_EVENT_Fn(OrthoGraphicCameraContraller::OnEventMouseScrolled));
		event_dispatcher.DisPatcher<EventWindowResize>(TZ_CORE_BIND_EVENT_Fn(OrthoGraphicCameraContraller::OnEventWindowResize));
	}

	void OrthoGraphicCameraContraller::CalculateView()
	{
		m_Bound = { -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel ,m_ZoomLevel };
		Camera.SetProjectMatrix(m_Bound.Left, m_Bound.Right, m_Bound.Bottom, m_Bound.Top);
	}

	void OrthoGraphicCameraContraller::ResizeView(float width, float height)
	{
		m_AspectRatio = width / height;
		CalculateView();
	}

	bool OrthoGraphicCameraContraller::OnEventMouseScrolled(EventMouseScrolled& event)
	{
		TZ_PROFILE_FUNCTION_SIG();

		m_ZoomLevel -= event.GetOffsetY() * 30.0f;
		m_ZoomLevel = std::max(m_ZoomLevel, 180.0f);		//这样写是基于m_ZoomLevel = 720时最大放大两倍
		//m_ZoomLevel = std::min(m_ZoomLevel, 320.0f);
		CalculateView();
		return false;
	}

	bool OrthoGraphicCameraContraller::OnEventWindowResize(EventWindowResize& event)
	{
		TZ_PROFILE_FUNCTION_SIG();

		//这有一个bug，左右缩放是正常的，但是上下缩放的时候会让渲染对象一起缩放
		//m_ZoomLevel = (float)event.GetWindowsHeight();		//但是打开这一句又会导致每一次调整窗口大小都刷新滚轮改过的大小
		ResizeView((float)event.GetWindowsWidth() , (float)event.GetWindowsHeight());
		return false;
	}
}