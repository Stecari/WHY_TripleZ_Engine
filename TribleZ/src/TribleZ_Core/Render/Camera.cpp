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
		m_ViewProjectionMatrix = m_Projection_matrix * m_View_matrix;	//���¼����Ӿ�ͶӰ����
	}


	const void OrthoGraphicCamera::RecalculateViewMatrix()
	{
		TZ_PROFILE_FUNCTION_SIG();

		glm::mat4 translate = glm::translate(glm::mat4(1.0f), m_Position)
							* glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));
																							/*�����������ֱ����xyz��
																								ȡһ��ʾ�����������ת*/
		m_View_matrix = glm::inverse(translate);
		m_ViewProjectionMatrix = m_Projection_matrix * m_View_matrix;

	}

	const void OrthoGraphicCamera::RecalculateProjectionMatrix(float fov, float width, float height,
															   float near_view, float far_view)
	{
		TZ_PROFILE_FUNCTION_SIG();

								/*������Ұ�ǣ����ȣ�����ӽǣ���Զ�ӽǣ�����һ��͸�Ӿ���*/
		m_Projection_matrix = glm::perspective<float>(glm::radians(fov), width / height, near_view, far_view);
		m_ViewProjectionMatrix = m_Projection_matrix * m_View_matrix;
		
	}
}