#pragma once

#include "Scene.h"
#include "entt.hpp"

#include "TribleZ_Core/UUID.h"
#include "Component.h"			
//这里记录一个bug问题：递归包含,直接编译会报一个“TribleZ::ScriptableEntity::m_Entity”使用未定义的 class“TribleZ::Entity”的错
//由于Component.h里面包含了ScriptableEntity.h，ScriptableEntity.h里面又用到了Entity,但是Entity.h中又包含了Component.h
//所以最后这个Component.h复制进来的时候，ScriptableEntity写在了Entity的上面，就未定义了
//一种解决方法是不在Component.h中包含完整的ScriptableEntity.h
// 改用对ScriptableEntity的声明，Component.h中也就不存在对Entity的定义了


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

		/*在我们有了UUID组件后就不需要这个了*/
		uint64_t GetUniqueHandleID() const;
		UUID GetUUID() { return GetComponent<IDComponent>().ID; };
		const std::string& GetName() { return GetComponent<TagComponent>().Tag; };

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

		//添加或者替换
		template<typename T, typename... Args>
		T& AddOrRepalceComponent(Args&&... args)
		{
			T& Component = m_Scene->m_Registry.emplace_or_replace<T>(m_EntityHandle, std::forward<Args>(args)...);
			m_Scene->OnComponentAdding<T>(*this, Component);
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