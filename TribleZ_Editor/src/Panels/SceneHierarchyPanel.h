#pragma once
#include "TribleZ_Core/Core.h"
#include "TribleZ_Core/Log.h"
#include "Scene/Entity.h"
#include "Scene/Scene.h"

namespace TribleZ
{
	class SceneHierarchyPanel
	{
	private:
		Ref<Scene> m_Context;

		Entity m_SelectionContext;	//为什么叫这个名字？

	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& context);

		void SetContext(const Ref<Scene>& context);

		void OnImGuiRender();

	private:
		//void DrawEntityNode(Entity& entity);	//cherno写的是没有&的下面那个，但是我觉得加上&也可以，先放着好了
		//void DrawComponent(Entity& entity);
		void DrawEntityNode(Entity entity);
		void DrawComponent(Entity entity);
	};
}