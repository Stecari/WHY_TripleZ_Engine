#pragma once
#include "BaseCamera.h"
#include "TribleZ_Core/TimeStep.h"
#include "TribleZ_Core/Event/Event.h"
#include "TribleZ_Core/Event/MouseEvent.h"

#include <glm/glm.hpp>

/*
* ������е���֮ǰд��OrthoGraphicCameraController*/
namespace TribleZ
{
	class Editor_Camera : public BaseCamera
	{
	private:
		float m_Distance = 10.0f;		//�����������ǰ�������������ж�Զ��
		float m_ViewportWidth = 720.0f, m_ViewportHeight = 1280.0f;

		glm::mat4 m_ViewMatrix;		//˵Ƭ����ʱ�������λ��
		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_FocalPoint = { 0.0f, 0.0f, 0.0f };		//����

		glm::vec2 m_InitialMousePosition;

		float m_FOV = 45.0f, m_AspectRatio = 1.778f, m_NearClip = 0.01f, m_FarClip = 10000.0f;

		float m_Pitch = 0.0f, m_Yaw = 0.0f/*, m_Roll = 0.0f*/;		//��̬��

	public:
		Editor_Camera() = default;
		Editor_Camera(float fov, float aspectRatio, float nearClip, float farClip);

		void OnUpdata(TimeStep timestep);
		void OnEvent(Event& event);

		inline float GetDistance() const { return m_Distance; }
		inline void SetDistance(float distance) { m_Distance = distance; }

		inline void SetViewportSize(float width, float height) { m_ViewportWidth = width; m_ViewportHeight = height; UpdataProjection(); }

		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
													/*m_Projection ����BaseCamera��(������ܸĳ�Camera)*/
		glm::mat4 GetViewProjection() const { return m_Projection * m_ViewMatrix; }

		glm::vec3 GetUpDirection() const;
		glm::vec3 GetRightDirection() const;
		glm::vec3 GetForwordDirection() const;
		const glm::vec3& GetPosition() const { return m_Position; }
		glm::quat GetOrientation() const;	//��ȡ����

		float GetPitch() const { return m_Pitch; }
		float GetYaw() const { return m_Yaw; }
	private:
		void UpdataProjection();
		void UpdataView();

		bool OnMouseScroll(EventMouseScrolled& event);

		void MousePan(const glm::vec2& delta);		//���ƽ��
		void MouseRotate(const glm::vec2& delta);
		void MouseZoom(float delta);

		glm::vec3 CalculatePositoin() const;
		
		std::pair<float, float> PanSpeed() const;
		float RotationSpeed() const;
		float ZoomSpeed() const;

	};
}