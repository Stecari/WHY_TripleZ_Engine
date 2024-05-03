#pragma once
#include "glm/glm.hpp"

namespace TribleZ
{
	class BaseCamera
	{
	protected:
		glm::mat4 m_Projection = { 1.0f };
	public:
		BaseCamera() = default;
		BaseCamera(const BaseCamera& other) = default;
		BaseCamera(const glm::mat4& projection)
			:m_Projection(projection)	{}

		const glm::mat4& GetProjection() const { return m_Projection; }
	};
}