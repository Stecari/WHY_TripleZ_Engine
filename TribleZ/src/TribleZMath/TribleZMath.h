#pragma once

#include <glm/glm.hpp>

namespace TribleZ::Math
{
	bool Decomposed(glm::mat4& transform, glm::vec3& outTranslate, glm::vec3& outRotation, glm::vec3& outScale);
}