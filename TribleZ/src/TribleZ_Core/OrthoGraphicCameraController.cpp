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

		/*----------�����������------------*/
		Camera.SetPosition(CameraPosition);
		Camera.SetRotation(CameraRotation);
		/*----------�����������------------*/
	}

	void OrthoGraphicCameraContraller::OnUpdata(TimeStep time_step)
	{
		TZ_PROFILE_FUNCTION_SIG();

		/*-------------------------����任----------------------------------------------------*/
		if (Input::IsKeyPressed(TZ_KEY_W))
		{
			//CameraPosition.y += CameraPositionSpeed * time_step;	//��������������һ��float()

			CameraPosition.y += glm::cos(glm::radians(CameraRotation)) * CameraPositionSpeed * time_step;	//��������������һ��float()
			CameraPosition.x -= glm::sin(glm::radians(CameraRotation)) * CameraPositionSpeed * time_step;
		}
		else if (Input::IsKeyPressed(TZ_KEY_S))
		{
			//CameraPosition.y -= CameraPositionSpeed * time_step;	//��ʽת��������д����float(time_step)

			CameraPosition.y -= glm::cos(glm::radians(CameraRotation)) * CameraPositionSpeed * time_step;	//��ʽת��������д����float(time_step)
			CameraPosition.x += glm::sin(glm::radians(CameraRotation)) * CameraPositionSpeed * time_step;
		}
		if (Input::IsKeyPressed(TZ_KEY_A))
		{
			CameraPosition.x -= glm::cos(glm::radians(CameraRotation)) * CameraPositionSpeed * time_step;//�൱����time_step.GetSecond()
			CameraPosition.y -= glm::sin(glm::radians(CameraRotation)) * CameraPositionSpeed * time_step;
		}
		else if (Input::IsKeyPressed(TZ_KEY_D))
		{
			CameraPosition.x += glm::cos(glm::radians(CameraRotation)) * CameraPositionSpeed * time_step;
			CameraPosition.y += glm::sin(glm::radians(CameraRotation)) * CameraPositionSpeed * time_step;
		}
		/*----------�����������------------*/
		Camera.SetPosition(CameraPosition);
		CameraPositionSpeed = m_ZoomLevel;
		/*----------�����������------------*/
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
			/*----------�����������------------*/
			Camera.SetRotation(CameraRotation);
			/*----------�����������------------*/
		}
		/*-------------------------����任----------------------------------------------------*/
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
		m_ZoomLevel = std::max(m_ZoomLevel, 180.0f);		//����д�ǻ���m_ZoomLevel = 720ʱ���Ŵ�����
		//m_ZoomLevel = std::min(m_ZoomLevel, 320.0f);
		CalculateView();
		return false;
	}

	bool OrthoGraphicCameraContraller::OnEventWindowResize(EventWindowResize& event)
	{
		TZ_PROFILE_FUNCTION_SIG();

		//����һ��bug�����������������ģ������������ŵ�ʱ�������Ⱦ����һ������
		//m_ZoomLevel = (float)event.GetWindowsHeight();		//���Ǵ���һ���ֻᵼ��ÿһ�ε������ڴ�С��ˢ�¹��ָĹ��Ĵ�С
		ResizeView((float)event.GetWindowsWidth() , (float)event.GetWindowsHeight());
		return false;
	}
}