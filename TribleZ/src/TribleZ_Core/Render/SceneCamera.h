#pragma once
#include "BaseCamera.h"

namespace TribleZ
{
	class SceneCamera : public BaseCamera
	{
	public:     /*				  3D        2D													*/
				/*相机的类型： 投影相机和正交相机*/		//这两个类型的差别还是得找机会研究一下的
		enum CameraProjectionType{	Perspective = 0, OrthoGraphic = 1	};
	private:
		CameraProjectionType m_ProjectionType = CameraProjectionType::OrthoGraphic;

		float m_OrthoGraphicSize = 720.0f;
		float m_OrthoGraphicNear = -1.0f, m_OrthoGraphicFar = 1.0f;

		float m_PerspectiveFOV = glm::radians(45.0f);	//视野角度
		float m_PerspectiveNear = 0.01f, m_PerspectiveFar = 10000.0f;

		float m_AspectRatio = 1280.0f / 720.0f;

	public:
		SceneCamera();
		~SceneCamera() = default;

		/*-------获取和更改正交相机参数-----------------------------------------------------------------------------*/
		//我总觉得这个不如换成float& 连Set****函数都省了
		float GetOrthoGraphicNearClip()  const { return m_OrthoGraphicNear; }
		void SetOrthoGraphicNearClip(float nearClip) { m_OrthoGraphicNear = nearClip; RecalculateProjection(); }

		float GetOrthoGraphicFarClip() const { return m_OrthoGraphicFar; }
		void SetOrthoGraphicFarClip(float farClip) { m_OrthoGraphicFar = farClip; RecalculateProjection(); }

		float GetOrthoGraphicSize() const { return m_OrthoGraphicSize; }
		void SetOrthoGraphicSize(float size) { m_OrthoGraphicSize = size; RecalculateProjection(); }

		/*-------获取和更改正交相机参数-----------------------------------------------------------------------------*/

		/*-------获取和更改透视相机参数-----------------------------------------------------------------------------*/
		//我总觉得这个不如换成float& 连Set****函数都省了
		float GetPerspectiveNearClip()  const { return m_PerspectiveNear; }
		void SetPerspectiveNearClip(float nearClip) { m_PerspectiveNear = nearClip; RecalculateProjection(); }

		float GetPerspectiveFarClip() const { return m_PerspectiveFar; }
		void SetPerspectiveFarClip(float farClip) { m_PerspectiveFar = farClip; RecalculateProjection(); }

		float GetPerspectiveFOV() const { return m_PerspectiveFOV; }
		void SetPerspectiveFOV(float fov) { m_PerspectiveFOV = fov; RecalculateProjection(); }
		/*-------获取和更改透视相机参数-----------------------------------------------------------------------------*/
		
		CameraProjectionType GetProjType() const { return m_ProjectionType; }
		void SetProjectionType(CameraProjectionType type) { m_ProjectionType = type; RecalculateProjection(); }

		void SetOrthoGraphic(float size, float nearClip = -1.0f, float farClip = 1.0f);
		void SetPersipective(float verticalFOV, float nearClip = 0.01f, float farClip = 10000.0f);
		void OnResizeView(uint32_t width, uint32_t height );

	private:
		void RecalculateProjection();//工具函数
	};



}