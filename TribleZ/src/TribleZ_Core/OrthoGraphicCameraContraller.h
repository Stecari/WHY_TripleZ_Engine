#pragma once
#include "TribleZ_Core/Core.h"
#include "TribleZ_core/Render/Camera.h"

#include "TribleZ_Core/Event/MouseEvent.h"
#include "TribleZ_core/Event/ApplicationEvent.h"

#include "TribleZ_Core/TimeStep.h"


namespace TribleZ
{
	struct OrthographicCameraBounds
	{
		float Left, Right;
		float Bottom, Top;

		float GetWidth() { return Right - Left; };
		float GetHeight() { return Top - Bottom; };
	};

	class OrthoGraphicCameraContraller
	{
	private:
		float m_AspectRatio;
		float m_ZoomLevel = 720.0f;

		bool Rotation_flag;

		OrthographicCameraBounds m_Bound;
		OrthoGraphicCamera Camera;				//���CameraҪ����Bound�����棬���ʼ������Ȼ�ĵ�һ֡��������ģ����԰�������m_Bound�����濴һ�»ᷢ��ʲô
	public:
		OrthoGraphicCameraContraller(float aspect_ratio, bool if_rotation = 0);

		void OnUpdata(TimeStep timestep);
		void OnEvent(Event& event);

		inline OrthoGraphicCamera& GetCamera() { return Camera; }
		inline const OrthoGraphicCamera& GetCamera() const { return Camera; }

		void ResizeView(float width, float height);
		void CalculateView();

		inline void SetZoomLevel(unsigned int zoom_level) { m_ZoomLevel = zoom_level; CalculateView(); }
		inline float GetZoomLevel() { return m_ZoomLevel; }

		const OrthographicCameraBounds& GetBound() const { return m_Bound; }
	private:
		bool OnEventMouseScrolled(EventMouseScrolled& event);
		bool OnEventWindowResize(EventWindowResize& event);


	private:
		/*-------------����任����--------------------------*/
		/*---------�ƶ��ٶ�---------*/
		glm::vec3 CameraPosition = {0.0f, 0.0f, 0.0f};
		float CameraPositionSpeed = 600;
		/*---------ת���ٶ�---------*/
		float CameraRotation = 0.0f;
		float CameraRotationSpeed = 40;
		/*-------------����任����--------------------------*/
	};
}