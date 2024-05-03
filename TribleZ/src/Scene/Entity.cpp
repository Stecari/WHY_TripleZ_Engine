#include "tzpch.h"
#include "Entity.h"

namespace TribleZ
{
	Entity::Entity(entt::entity handle, Scene* scene)
		:m_EntityHandle(handle),m_Scene(scene)
	{}
}