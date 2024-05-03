#pragma once
#include "BaseCamera.h"

namespace TribleZ
{
	class SceneCamera : public BaseCamera
	{
	public:     /*				  3D        2D													*/
				/*��������ͣ� ͶӰ������������*/		//���������͵Ĳ���ǵ��һ����о�һ�µ�
		enum CameraProjectionType{	Perspective = 0, OrthoGraphic = 1	};
	private:
		CameraProjectionType m_ProjectionType = CameraProjectionType::OrthoGraphic;

		float m_OrthoGraphicSize = 720.0f;
		float m_OrthoGraphicNear = -1.0f, m_OrthoGraphicFar = 1.0f;

		float m_PerspectiveFOV = glm::radians(45.0f);	//��Ұ�Ƕ�
		float m_PerspectiveNear = 0.01f, m_PerspectiveFar = 10000.0f;

		float m_AspectRatio = 1280.0f / 720.0f;

	public:
		SceneCamera();
		~SceneCamera() = default;

		/*-------��ȡ�͸��������������-----------------------------------------------------------------------------*/
		//���ܾ���������绻��float& ��Set****������ʡ��
		float GetOrthoGraphicNearClip()  const { return m_OrthoGraphicNear; }
		void SetOrthoGraphicNearClip(float nearClip) { m_OrthoGraphicNear = nearClip; RecalculateProjection(); }

		float GetOrthoGraphicFarClip() const { return m_OrthoGraphicFar; }
		void SetOrthoGraphicFarClip(float farClip) { m_OrthoGraphicFar = farClip; RecalculateProjection(); }

		float GetOrthoGraphicSize() const { return m_OrthoGraphicSize; }
		void SetOrthoGraphicSize(float size) { m_OrthoGraphicSize = size; RecalculateProjection(); }

		/*-------��ȡ�͸��������������-----------------------------------------------------------------------------*/

		/*-------��ȡ�͸���͸���������-----------------------------------------------------------------------------*/
		//���ܾ���������绻��float& ��Set****������ʡ��
		float GetPerspectiveNearClip()  const { return m_PerspectiveNear; }
		void SetPerspectiveNearClip(float nearClip) { m_PerspectiveNear = nearClip; RecalculateProjection(); }

		float GetPerspectiveFarClip() const { return m_PerspectiveFar; }
		void SetPerspectiveFarClip(float farClip) { m_PerspectiveFar = farClip; RecalculateProjection(); }

		float GetPerspectiveFOV() const { return m_PerspectiveFOV; }
		void SetPerspectiveFOV(float fov) { m_PerspectiveFOV = fov; RecalculateProjection(); }
		/*-------��ȡ�͸���͸���������-----------------------------------------------------------------------------*/
		
		CameraProjectionType GetProjType() const { return m_ProjectionType; }
		void SetProjectionType(CameraProjectionType type) { m_ProjectionType = type; RecalculateProjection(); }

		void SetOrthoGraphic(float size, float nearClip = -1.0f, float farClip = 1.0f);
		void SetPersipective(float verticalFOV, float nearClip = 0.01f, float farClip = 10000.0f);
		void OnResizeView(uint32_t width, uint32_t height );

	private:
		void RecalculateProjection();//���ߺ���
	};



}