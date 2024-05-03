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
		friend class Entity;	//友元，可以使用自己的private成员，自带extern
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
		entt::registry m_Registry;  //注册表，负责收集实体组件的数据和实体ID

		uint32_t ViewWidth, ViewHeight;

	public:
		Scene();
		~Scene();

		void OnUpdata(TimeStep tiemstep);
		void ResizeView(uint32_t width, uint32_t height);

		Entity CreateEntity(const std::string& name = std::string());	//每个实体包含一个entt::entity,和Scene*
		//void DeleteEntity(const Entity& entity);		//我喜欢这样
		void DeleteEntity(Entity entity);

	private:
		/*用特殊手段将函数体放在cpp文件里，这个用法比较少，可以关注一下*/
		template<typename T>
		void OnComponentAdding(Entity entity, T& component);
	};
}