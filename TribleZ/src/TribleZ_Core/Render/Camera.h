#pragma once
#include "glm/glm.hpp"


namespace TribleZ
{
	class OrthoGraphicCamera
	{
	private:
		glm::mat4 m_Projection_matrix;
		glm::mat4 m_View_matrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position;
		float m_Rotation;

	public:
		OrthoGraphicCamera(float left, float right, float bottom, float top);
		void SetProjectMatrix(float left, float right, float bottom, float top);

		const void SetPosition(glm::vec3 position) { m_Position = position; RecalculateViewMatrix(); }
		inline const glm::vec3 GetPosition() { return m_Position; }

		const void SetRotation(float rotation) { m_Rotation = rotation; RecalculateViewMatrix();}
		inline const float GetRotation() { return m_Rotation; }

		const void RecalculateViewMatrix();
		const void RecalculateProjectionMatrix(float fov, float width, float height, float near_view, float far_view);

		inline const glm::mat4& GetMatrix() const { return m_Projection_matrix; }
		inline const glm::mat4& GetViewMatrix() const { return m_View_matrix; }
		inline const glm::mat4& GetViewProjectMat() const { return m_ViewProjectionMatrix; }


	};
}