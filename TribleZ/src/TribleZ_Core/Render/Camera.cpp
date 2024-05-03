#include "tzpch.h"
#include "Camera.h"
//#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace TribleZ
{
	OrthoGraphicCamera::OrthoGraphicCamera(float left, float right, float bottom, float top)
		:m_Projection_matrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_View_matrix(glm::mat4(1.0f)),
		m_Position(glm::vec3(0.0f, 0.0f, 0.0f)), m_Rotation(0.0f)
	{
		TZ_PROFILE_FUNCTION_SIG();

		m_ViewProjectionMatrix = m_Projection_matrix * m_View_matrix;

	}

	void OrthoGraphicCamera::SetProjectMatrix(float left, float right, float bottom, float top)
	{
		TZ_PROFILE_FUNCTION_SIG();

		m_Projection_matrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_ViewProjectionMatrix = m_Projection_matrix * m_View_matrix;	//重新计算视觉投影矩阵
	}


	const void OrthoGraphicCamera::RecalculateViewMatrix()
	{
		TZ_PROFILE_FUNCTION_SIG();

		glm::mat4 translate = glm::translate(glm::mat4(1.0f), m_Position)
							* glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));
																							/*名义上三个分别代表xyz，
																								取一表示绕着这跟轴旋转*/
		m_View_matrix = glm::inverse(translate);
		m_ViewProjectionMatrix = m_Projection_matrix * m_View_matrix;

	}

	const void OrthoGraphicCamera::RecalculateProjectionMatrix(float fov, float width, float height,
															   float near_view, float far_view)
	{
		TZ_PROFILE_FUNCTION_SIG();

								/*根据视野角，宽长比，最近视角，最远视角，创建一个透视矩阵*/
		m_Projection_matrix = glm::perspective<float>(glm::radians(fov), width / height, near_view, far_view);
		m_ViewProjectionMatrix = m_Projection_matrix * m_View_matrix;
		
	}
}