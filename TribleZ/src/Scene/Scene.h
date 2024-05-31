#pragma once
#include "TribleZ_Core/Core.h"
#include "entt.hpp"

#include "TribleZ_Core/TimeStep.h"
#include "TribleZ_Core/Render/Editor_Camera.h"

struct b2WorldId;
#ifdef BOX2D_V2_4_CODE_REFERENCE
class b2World;	//不能声明在namespace TribleZ里面，不然就变成了TribleZ::b2World()了，就不是Box2D的东西了
#endif

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

		uint32_t m_ViewWidth, m_ViewHeight;

		b2WorldId* m_PhysicsWorldID = {}/*b2_nullWorldId*/;

#ifdef BOX2D_V2_4_CODE_REFERENCE
		b2World* m_PhysicsWorld = nullptr;		//这里创建一个b2World对象会被视为不完整的实例，但是创建一个b2World指针就可以
#endif


	public:
		Scene();
		~Scene();

		/*OnUpdata更新函数分为编辑时和运行时，运行时就是相当于按下了编辑器上的“游玩”或者“运行”按钮后
		* 差别就是编辑时需要将相机作为一个可以被渲染实体，这样子就可以在编辑器中拖动相机，让他处于我们想要的位置，
		* 然后再点击"游玩"或者"试玩"进入运行时，视角进入指定相机，不再渲染所有相机的虚拟实体*/
		void OnUpdataEditor(TimeStep tiemstep, Editor_Camera Edi_Camera);
		void OnUpdataRuntime(TimeStep tiemstep);
		void ResizeView(uint32_t width, uint32_t height);

		void OnRuntimeStop();
		void OnRuntimeStart();

		Entity CreateEntity(const std::string& name = std::string());	//每个实体包含一个entt::entity,和Scene*
		void DeleteEntity(Entity entity);

		//z这里我发现我一直有有些盲区，就是我特别喜欢加&，这个根本不用加&，而且加不了的啊
		Entity GetPrimaryCameraEntity();

	private:
		/*用特殊手段将函数体放在cpp文件里，这个用法比较少，可以关注一下*/
		template<typename T>
		void OnComponentAdding(Entity entity, T& component);
	};
}