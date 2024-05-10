#include "tzpch.h"
#include "Editor_Camera.h"

#include "TribleZ_Core/Input.h"
#include "TribleZ_Core/Mouse_and_Key_Code/KeyCodeTable.h"
#include "TribleZ_Core/Mouse_and_Key_Code/MouseButtonCodeTable.h"

#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace TribleZ
{
	Editor_Camera::Editor_Camera(float fov, float aspectRatio, float nearClip, float farClip)
		:m_FOV(fov), m_AspectRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(farClip),
		BaseCamera(glm::perspective(glm::radians(fov),aspectRatio, nearClip, farClip))
	{
		UpdataView();
	}

	void Editor_Camera::OnUpdata(TimeStep timestep)
	{
		if (Input::IsKeyPressed(TZ_KEY_LEFT_ALT))
		{
			const glm::vec2& mousepos{ Input::GetMouseX(), Input::GetMouseY() };
			glm::vec2 delta = (mousepos - m_InitialMousePosition) * 0.003f;	//这个乘上0.003就不知道什么作用
			m_InitialMousePosition = mousepos;

			//按下左ALT
			if (Input::IsMouseButtonPressed(TZ_MOUSE_BUTTON_MIDDLE)){		//中键移动
				MousePan(delta);
			}
			else if (Input::IsMouseButtonPressed(TZ_MOUSE_BUTTON_LEFT)){	//左键旋转
				MouseRotate(delta);
			}
			else if (Input::IsMouseButtonPressed(TZ_MOUSE_BUTTON_RIGHT)){	//右键缩放
				MouseZoom(delta.y);
			}
		}
		UpdataView();
	}
	void Editor_Camera::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.DisPatcher<EventMouseScrolled>(TZ_CORE_BIND_EVENT_Fn(Editor_Camera::OnMouseScroll));
	}

	void Editor_Camera::UpdataProjection()
	{
		m_AspectRatio = m_ViewportWidth / m_ViewportHeight;
		m_Projection = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip);
	}

	void Editor_Camera::UpdataView()
	{
		//m_Yaw = m_Pitch = 0.0f;		//锁定相机旋转
		m_Position = CalculatePositoin();

		glm::quat orientation = GetOrientation();
		m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Position) * glm::toMat4(orientation);
		m_ViewMatrix = glm::inverse(m_ViewMatrix);
	}

	bool Editor_Camera::OnMouseScroll(EventMouseScrolled& event)
	{
		float delta = event.GetOffsetY() * 0.1f;
		MouseZoom(delta);
		UpdataView();
		return false;		//不结束事件传递，也就是其他的OnEvent依旧可以接收到我们的鼠标滚动
	}
	void Editor_Camera::MousePan(const glm::vec2& delta)		//鼠标平移
	{
		auto [speedX, speedY] = PanSpeed();
		m_FocalPoint += -GetRightDirection() * speedX * delta.x * m_Distance;
		m_FocalPoint += GetUpDirection() * speedY * delta.y * m_Distance;

	}
	void Editor_Camera::MouseRotate(const glm::vec2& delta)
	{
		float yawSign = GetUpDirection().y > 0.0f? 1.0f : -1.0f;
		m_Yaw += yawSign * delta.x * RotationSpeed();
		m_Pitch += delta.y * RotationSpeed();
	}
	void Editor_Camera::MouseZoom(float delta)
	{
		m_Distance -= delta * ZoomSpeed();
		if (m_Distance < 1.0f)
		{
			m_FocalPoint += GetForwordDirection();
			m_Distance = 1.0f;
		}

	}

	glm::vec3 Editor_Camera::CalculatePositoin() const
	{
		return m_FocalPoint - GetForwordDirection() * m_Distance;
	}

	std::pair<float, float> Editor_Camera::PanSpeed() const		//平移速度
	{
		//这个是Cherno当年自己算出来的一个计算方程，我不知道我们参数不一样会不有问题，到时候可能自己还要试着算一次
		float x = std::min(m_ViewportWidth / 1000.0f, 2.40f);	//最大为2.4		标准库std里面的max和min真的很奇怪
		//float xFactor = 0.0366 * (x * x) - 0.1778f * x + 0.3021f;
		float xFactor = 4.1366 * (x * x) - 1.1778f * x + 0.3021f;

		float y = std::min(m_ViewportHeight / 1000.0f, 2.4f);
		//float yFactor = 0.0366 * (y * y) - 0.1778f * y + 0.3021f;
		float yFactor = 4.1366 * (y * y) - 1.1778f * y + 0.3021f;

		return { 10 * xFactor, 10 * yFactor };
	}
	float Editor_Camera::RotationSpeed() const
	{
		return 0.8f;
	}
	float Editor_Camera::ZoomSpeed() const
	{
		//没看懂为什么这么算，distance 可能算是一条边的变化速度？
		float distance = m_Distance * 0.2;
		distance = std::max(distance, 0.0f);		//位置不能为负
		float speed = distance * distance;
		speed = std::min(speed, 100.0f);	//速度最大100
		return speed;
	}

	glm::vec3 Editor_Camera::GetUpDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3{ 0.0f, 1.0f, 0.0f });
	}
	glm::vec3 Editor_Camera::GetRightDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3{ 1.0f, 0.0f, 0.0f });
	}
	glm::vec3 Editor_Camera::GetForwordDirection() const
	{																/*这里是-1，之前是1*/
		return glm::rotate(GetOrientation(), glm::vec3{ 0.0f, 0.0f, -1.0f });
	}
	glm::quat Editor_Camera::GetOrientation() const	//获取方向
	{
		return glm::quat(glm::vec3(-m_Pitch, -m_Yaw, 0.0f));
	}

}