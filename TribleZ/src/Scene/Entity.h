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
		/*operator���ز�����������if(Entity) �ж�Entity�Ƿ�Ϊ��ʱ*/
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


		//������					/*ת����*/
		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)	/*���forward_listת����Ĳ�����д��һ��Ҫ������*/
		{
			TZ_CORE_ASSERT(!HasComponent<T>(), "Entity already has component!");
			T& Component = m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
																		  /*����ת���������úò�һ��*/
			m_Scene->OnComponentAdding<T>(*this, Component);	//ÿ����������ʱ�����趨һ��Ĭ�Ϻ���
			return Component;
		}

		//��ȡ�Ѵ������
		template<typename T>
		T& GetComponent()
		{
			TZ_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		//����Ƿ�������
		template<typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.all_of<T>(m_EntityHandle);
		}

		//�Ƴ����
		template<typename T>
		void RemoveComponent()
		{
			TZ_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
			m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}
	};
}