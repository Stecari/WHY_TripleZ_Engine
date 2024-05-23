#include "tzpch.h"
#include "SceneCamera.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace TribleZ
{
	SceneCamera::SceneCamera()
	{
		RecalculateProjection();
	}


	void SceneCamera::SetPersipective(float verticalFOV, float nearClip, float farClip)
	{
		m_ProjectionType = CameraProjectionType::Perspective;	//真的需要么。。。

		m_PerspectiveFOV = verticalFOV;
		m_PerspectiveNear = nearClip;
		m_PerspectiveFar = farClip;
		RecalculateProjection();
	}
	void SceneCamera::SetOrthoGraphic(float size, float nearClip, float farClip)
	{
		m_ProjectionType = CameraProjectionType::OrthoGraphic;	//真的需要么。。。

		m_OrthoGraphicSize = size;
		m_OrthoGraphicNear = nearClip;
		m_OrthoGraphicSize = farClip;
		RecalculateProjection();
	}
	void SceneCamera::OnResizeView(uint32_t width, uint32_t height)
	{
		TZ_CORE_ASSERT(width > 0 && height > 0);
		m_AspectRatio = (float)width / (float)height;
		RecalculateProjection();
	}

	void SceneCamera::RecalculateProjection()
	{
		if (m_ProjectionType == CameraProjectionType::OrthoGraphic)
		{
			float ortholeft = -m_OrthoGraphicSize * m_AspectRatio * 0.5;
			float orthoright = m_OrthoGraphicSize * m_AspectRatio * 0.5;
			float orthobottom = -m_OrthoGraphicSize * 0.5;
			float orthotop = m_OrthoGraphicSize * 0.5;

			m_Projection = glm::ortho(ortholeft, orthoright, orthobottom, orthotop, m_OrthoGraphicNear, m_OrthoGraphicFar);
		}
		else if (m_ProjectionType == CameraProjectionType::Perspective)
		{
			m_Projection = glm::perspective(m_PerspectiveFOV, m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
		}
		
	}
}