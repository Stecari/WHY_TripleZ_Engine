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

		Entity m_SelectionContext;	//Ϊʲô��������֣�

	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& context);

		void SetContext(const Ref<Scene>& context);

		void OnImGuiRender();

	private:
		//void DrawEntityNode(Entity& entity);	//chernoд����û��&�������Ǹ��������Ҿ��ü���&Ҳ���ԣ��ȷ��ź���
		//void DrawComponent(Entity& entity);
		void DrawEntityNode(Entity entity);
		void DrawComponent(Entity entity);
	};
}