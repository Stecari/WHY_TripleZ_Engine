#pragma once
#include "TribleZ_Core/Core.h"
#include "entt.hpp"

#include "TribleZ_Core/TimeStep.h"

namespace TribleZ
{
	class Entity;
	class Scene
	{
	private:
		friend class Entity;	//��Ԫ������ʹ���Լ���private��Ա���Դ�extern
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
		entt::registry m_Registry;  //ע��������ռ�ʵ����������ݺ�ʵ��ID

		uint32_t ViewWidth, ViewHeight;

	public:
		Scene();
		~Scene();

		void OnUpdata(TimeStep tiemstep);
		void ResizeView(uint32_t width, uint32_t height);

		Entity CreateEntity(const std::string& name = std::string());	//ÿ��ʵ�����һ��entt::entity,��Scene*
		//void DeleteEntity(const Entity& entity);		//��ϲ������
		void DeleteEntity(Entity entity);

	private:
		/*�������ֶν����������cpp�ļ������÷��Ƚ��٣����Թ�עһ��*/
		template<typename T>
		void OnComponentAdding(Entity entity, T& component);
	};
}