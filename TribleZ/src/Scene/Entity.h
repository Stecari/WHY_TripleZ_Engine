#pragma once

#include "Scene.h"
#include "entt.hpp"

namespace TribleZ
{
	class Scene;
	class Entity
	{
	private:
		entt::entity m_EntityHandle{ entt::null };
		//std::weak_ptr<Scene> m_Scene;
		/*TribleZ::*/Scene* m_Scene = nullptr;
	public:
		Entity(entt::entity handle, Scene* scene);
		Entity(const Entity& other) = default;
		Entity() = default;

		uint64_t GetUniqueID() const ;

		operator uint32_t() const { return (uint32_t)m_EntityHandle; }
		/*operator重载操作符，用在if(Entity) 判断Entity是否为空时*/
		operator bool() const { return m_EntityHandle != entt::null; }
		operator entt::entity() const { return m_EntityHandle; }

		bool operator ==(const Entity& other) const
		{
			return (m_EntityHandle == other.m_EntityHandle) && (m_Scene == other.m_Scene);
		}
		bool operator !=(const Entity& other) const
		{
			return !(*this == other);
		}


		//添加组件					/*转发表*/
		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)	/*这个forward_list转发表的参数表写法一定要记下来*/
		{
			TZ_CORE_ASSERT(!HasComponent<T>(), "Entity already has component!");
			T& Component = m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
																		  /*完美转发函数，好好查一查*/
			m_Scene->OnComponentAdding<T>(*this, Component);	//每个组件在添加时可以设定一个默认函数
			return Component;
		}

		//获取已存在组件
		template<typename T>
		T& GetComponent()
		{
			TZ_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		//检查是否存在组件
		template<typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.all_of<T>(m_EntityHandle);
		}

		//移除组件
		template<typename T>
		void RemoveComponent()
		{
			TZ_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
			m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}
	};
}