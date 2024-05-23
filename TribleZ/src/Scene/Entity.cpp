#include "tzpch.h"
#include "Entity.h"
#include <shaderc/shaderc.h>
#include <spirv_cross/GLSL.std.450.h>

namespace TribleZ
{
	Entity::Entity(entt::entity handle, Scene* scene)
		:m_EntityHandle(handle),m_Scene(scene)
	{}

	uint64_t Entity::GetUniqueID() const
	{
		return (uint64_t)m_EntityHandle;
	}
}